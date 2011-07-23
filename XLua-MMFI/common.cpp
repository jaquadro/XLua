#include "common.h"

int ReadOnlyViolation (lua_State* L, const char* key) {
	char ebuf[255];
	sprintf_s((char*)&ebuf, 255, "Object field '%s' is read-only", key);
	lua_pushstring(L, (char*)&ebuf);
	lua_error(L);
	return 0;
}

int FunctionViolation (lua_State* L, const char* key) {
	char ebuf[255];
	sprintf_s((char*)&ebuf, 255, "Object function '%s' is protected", key);
	lua_pushstring(L, (char*)&ebuf);
	lua_error(L);
	return 0;
}

int MinParamCount (lua_State* L, const char* func, int count) {
	if (lua_gettop(L) < count) {
		char ebuf[255];
		sprintf_s((char*)&ebuf, 255, "Function '%s' expects %d parameters", func, count);
		lua_pushstring(L, (char*)&ebuf);
		lua_error(L);
		return 0;
	}

	return 1;
}

// -----

lua_CFunction FindFunction (const char* key, const FunctionPair* fp, int fpSize) {
	int rangeMin = 0;
	int rangeMax = fpSize - 1;
	int index = rangeMax / 2;
	
	while (rangeMax >= rangeMin) {
		if (fp[index].key == 0)
			break;

		int test = strcmp(key, fp[index].key);

		if (test > 0) {
			rangeMin = index + 1;
			index = rangeMin + (rangeMax - rangeMin) / 2;
		}
		else if (test < 0) {
			rangeMax = index - 1;
			index = rangeMin + (rangeMax - rangeMin) / 2;
		}
		else {
			return fp[index].func;
		}
	}
	
	return 0;
}

// -----

int ObjectCheck (LPHO ho, LPRH rh, DWORD fixed) {
	LPOBL objList = rh->rhObjectList;
	objectsList e = objList[0x0000FFFF & fixed];
	if (e.oblOffset != ho || ho->hoFlags & HOF_DESTROYED) {
		return 0;
	}

	return 1;
}

int ObjectCheck (lua_State* L) {
	LPHO obj = (LPHO)lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO));
	int fixed = lua_tointeger(L, lua_upvalueindex(UV_OBJECT_FIXED));
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPRH));

	return (obj, rh, fixed);
}

int ObjectCheck (lua_State* L, int index) {
	lua_getmetatable(L, index);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return 0;
	}

	lua_pushstring(L, "__index");
	lua_rawget(L, -2);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 2);
		return 0;
	}

	lua_getupvalue(L, -1, UV_OBJECT_LPHO);
	lua_getupvalue(L, -2, UV_OBJECT_FIXED);
	lua_getupvalue(L, -3, UV_OBJECT_LPRH);
	LPHO obj = (LPHO)lua_touserdata(L, -3);	
	int fixed = lua_tointeger(L, -2);
	LPRH rh = (LPRH)lua_touserdata(L, -1);

	lua_pop(L, 5);

	if (obj == 0 || rh == 0) {
		return 0;
	}

	return ObjectCheck(obj, rh, fixed);
}

// -----

int MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc, int closureSize) {
	lua_getmetatable(L, 1);
	lua_pushstring(L, key);
	lua_rawget(L, -2);
	if (lua_isfunction(L, -1)) {
		return 1;
	}

	lua_pop(L, 1);

	lua_pushstring(L, key);
	//lua_pushvalue(L, lua_upvalueindex(UV_OBJECT_LPHO));
	for (int i = 1; i <= closureSize; i++) {
		lua_pushvalue(L, lua_upvalueindex(i));
	}
	lua_pushcclosure(L, lfunc, closureSize);
	lua_rawset(L, -3);

	lua_pushstring(L, key);
	lua_rawget(L, -2);
	return 1;
}

// -----

headerObject* GetHO (LPRO obj) {
	return (headerObject*) obj;
}

tagRCOM* GetRCOM (LPHO ho) {
	if (!ho) return 0;

	DWORD flags = ho->hoOEFlags;

	if (flags & OEFLAG_MOVEMENTS || flags & OEFLAG_SPRITES || flags & OEFLAG_ANIMATIONS) {
		return (tagRCOM*)((char*)ho + sizeof(headerObject));
	}

	return 0;
}

tagRM* GetRM (LPHO ho) {
	if (!ho) return 0;

	DWORD flags = ho->hoOEFlags;
	
	if (flags & OEFLAG_MOVEMENTS) {
		return (tagRM*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM));
	}

	return 0;
}

tagRA* GetRA (LPHO ho) {
	if (!ho) return 0;

	DWORD flags = ho->hoOEFlags;
	
	if (flags & OEFLAG_MOVEMENTS && flags & OEFLAG_ANIMATIONS) {
		return (tagRA*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRM));
	}
	else if (flags & OEFLAG_ANIMATIONS) {
		return (tagRA*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM));
	}

	return 0;
}

tagRSPR* GetRSPR (LPHO ho) {
	if (!ho) return 0;

	DWORD flags = ho->hoOEFlags;
	
	if (flags & OEFLAG_MOVEMENTS) {
		if (flags & OEFLAG_SPRITES) {
			if (flags & OEFLAG_ANIMATIONS) {
				return (tagRSPR*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRM) + sizeof(tagRA));
			}
			else {
				return (tagRSPR*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRM));
			}
		}
	}
	else if (flags & OEFLAG_SPRITES) {
		if (flags & OEFLAG_ANIMATIONS) { // This should never be valid
			return (tagRSPR*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRA));
		}
		else {
			return (tagRSPR*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM));
		}
	}

	return 0;
}

tagRV* GetRV (LPHO ho) {
	if (!ho) return 0;

	DWORD flags = ho->hoOEFlags;

	if ((flags & OEFLAG_VALUES) == 0) {
		return 0;
	}
	
	if (flags & OEFLAG_MOVEMENTS) {
		const size_t base = sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRM);
		if (flags & OEFLAG_ANIMATIONS && flags & OEFLAG_SPRITES) {
			return (tagRV*)((char*)ho + base + sizeof(tagRA) + sizeof(tagRSPR));
		}
		else if (flags & OEFLAG_SPRITES) {
			return (tagRV*)((char*)ho + base + sizeof(tagRSPR));
		}
		else {
			return (tagRV*)((char*)ho + base);
		}
	}
	else if (flags & OEFLAG_ANIMATIONS && flags & OEFLAG_SPRITES) {
		return (tagRV*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRA) + sizeof(tagRSPR)); // should never be valid
	}
	else if (flags & OEFLAG_ANIMATIONS) {
		return (tagRV*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRA));
	}
	else if (flags & OEFLAG_SPRITES) {
		return (tagRV*)((char*)ho + sizeof(headerObject) + sizeof(tagRCOM) + sizeof(tagRSPR));
	}
	else {
		return (tagRV*)((char*)ho + sizeof(headerObject));
	}
}
