#include "class.h"
#include "object.h"
#include "collisions.h"

int ObjectClass::IndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_CLASS)
		return 0;

	//const char* key = lua_tostring(L, 2);

	int ret = StandardIndex(L, ObjectClassRead, ObjectClassWrite);
	if (ret > -1) return ret;

	return 0;
}

int ObjectClass::NewIndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_CLASS)
		return 0;

	//const char* key = lua_tostring(L, 2);

	int ret = StandardNewIndex(L, ObjectClassRead, ObjectClassWrite);
	if (ret > -1) return ret;

	// It's some untracked value, let them add it
	lua_rawset(L, 1);

	return 0;
}

int ObjectClass::MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc) {
	return ::MemoClosure(L, key, lfunc, 3);
}

LPOIL ObjectClass::GetObjectClass (lua_State* L, int index) {
	if (!lua_istable(L, index))
		return 0;

	int base = lua_gettop(L);								// +0
	lua_checkstack(L, base + 4);

	lua_getmetatable(L, index);								// +1 = Object metatable
	lua_pushstring(L, "__index");							// +2
	lua_rawget(L, -2);										// +2 = __index method

	lua_getupvalue(L, base + 2, UV_TYPE);					// +3 = "TYPE" upvalue
	if (lua_tointeger(L, base + 3) != TYPE_CLASS)
		return 0;

	lua_getupvalue(L, base + 2, UV_CLASS_LPOIL);			// +4 = "OIL" upvalue

	LPOIL oi = (LPOIL)lua_touserdata(L, base + 4);

	lua_pop(L, 4);
	return oi;
}

int ObjectClass::NewObjectClass (lua_State* L) {
	LPRH rhPtr = xlua_get_run_header(L);
	if (!rhPtr)
		return 0;

	int rtHWA = rhPtr->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0);

	const char* name = lua_tostring(L, 1);

	LPOIL oiList = rhPtr->rhOiList;
	LPOIL oi = 0;

	for (int i = 0; i < rhPtr->rhNumberOi; i++) {
		LPOIL currentOi;
		if (rtHWA)
			currentOi = (LPOIL)((char*)oiList + i * (sizeof(objInfoList) + 4));
		else
			currentOi = oiList + i;
		
		if (strcmp((char*)(&currentOi->oilName), name) == 0) {
			oi = currentOi;
			break;
		}
	}

	if (!oi)
		return 0;

	// Check if we cached the Class
	lua_pushstring(L, KEY_POOL_CLASS);
	lua_rawget(L, LUA_REGISTRYINDEX);
	lua_pushinteger(L, oi->oilOi);
	lua_rawget(L, -2);

	if (lua_istable(L, -1))
		return 1;
	else
		lua_pop(L, 1);

	int (*index_method)(lua_State*L) = ObjectClass::IndexMetamethod;
	int (*newindex_method)(lua_State*L) = ObjectClass::IndexMetamethod;

	// Create object class table
	lua_createtable(L, 0, 0);

	// Create and set common table
	lua_pushstring(L, "common");
	lua_createtable(L, 0, 0);
	lua_rawset(L, -3);

	// Create object metatable
	lua_createtable(L, 0, 3);

	// Add __index
	lua_pushnumber(L, TYPE_CLASS);
	lua_pushlightuserdata(L, (void*)oi);
	lua_pushlightuserdata(L, (void*)rhPtr);
	lua_pushcclosure(L, index_method, 3);
	lua_setfield(L, -2, "__index");

	// Add __newindex
	lua_pushnumber(L, TYPE_CLASS);
	lua_pushlightuserdata(L, (void*)oi);
	lua_pushlightuserdata(L, (void*)rhPtr);
	lua_pushcclosure(L, newindex_method, 3);
	lua_setfield(L, -2, "__newindex");

	// Add __metatable
	lua_pushstring(L, "Access violation");
	lua_setfield(L, -2, "__metatable");
	
	// Bind metatable
	lua_setmetatable(L, -2);

	// Cache table
	lua_pushinteger(L, oi->oilOi);
	lua_pushvalue(L, -2);
	lua_rawset(L, -4);

	return 1;
}

int	ObjectClass::ReturnObjects (lua_State* L, GenList& list) {
	lua_createtable(L, list.size(), 0);

	int i = 1;
	for (GenList::iterator it = list.begin(); it != list.end(); it++) {
		LPHO ho = (LPHO)(*it);

		lua_pushcfunction(L, Object::NewObject);
		lua_pushinteger(L, ho->hoOi << 16 | ho->hoNumber);
		lua_call(L, 1, 1);

		lua_rawseti(L, -2, i++);
	}

	return 1;
}

void ObjectClass::ColFlagCheck (lua_State* L, LPRH rh, LPOIL oi) {
	LPOBL objList = rh->rhObjectList;

	if (!(oi->oilLimitFlags & OILIMITFLAGS_QUICKCOL)) {
		oi->oilLimitFlags |= OILIMITFLAGS_QUICKCOL;

		LPHO cur = objList[oi->oilObject].oblOffset;
	
		while (cur->hoNumPrev >= 0) {
			cur = objList[cur->hoNumPrev].oblOffset;
		}

		npWin win = rh->rh4.rh4Mv->mvIdEditWin;

		while (true) {
			tagRCOM* com = GetRCOM(cur);

			if (com) {
				DWORD flags = GetSpriteFlags(win, com->rcSprite);

				if (!(flags & SF_RAMBO))
					SetSpriteFlags(win, com->rcSprite, flags | SF_RAMBO);
			}

			if (cur->hoNumNext < 0)
				break;
			cur = objList[cur->hoNumNext].oblOffset;
		}
	}
}

// Getters

int ObjectClass::ClassID (lua_State* L) {
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));

	lua_pushnumber(L, oi->oilOi);
	return 1;
}

int ObjectClass::ClassName (lua_State* L) {
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));

	lua_pushstring(L, (char*)oi->oilName);
	return 1;
}

int ObjectClass::TypeID (lua_State* L) {
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));

	lua_pushnumber(L, oi->oilType);
	return 1;
}

// Queries

int ObjectClass::ObjectCount (lua_State* L) {
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));

	lua_pushinteger(L, oi->oilNObjects);
	return 1;
}

int ObjectClass::ObjectList (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPRH));
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));
	LPOBL objList = rh->rhObjectList;

	lua_createtable(L, 0, oi->oilNObjects);
	if (oi->oilNObjects == 0)
		return 1;

	LPHO cur = objList[oi->oilObject].oblOffset;
	
	while (cur->hoNumPrev >= 0) {
		cur = objList[cur->hoNumPrev].oblOffset;
	}

	int i = 1;
	while (true) {
		lua_pushcfunction(L, Object::NewObject);
		lua_pushinteger(L, cur->hoOi << 16 | cur->hoNumber);
		lua_call(L, 1, 1);
		lua_rawseti(L, -2, i++);

		if (cur->hoNumNext < 0)
			break;
		cur = objList[cur->hoNumNext].oblOffset;
	}

	return 1;
}

int ObjectClass::RandomObject (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::RandomObjectFunc);
}

int ObjectClass::RandomObjectFunc (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPRH));
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));
	LPOBL objList = rh->rhObjectList;

	if (oi->oilNObjects == 0)
		return 0;

	int randId = rand() % oi->oilNObjects;

	LPHO cur = objList[oi->oilObject].oblOffset;
	
	while (cur->hoNumPrev >= 0) {
		cur = objList[cur->hoNumPrev].oblOffset;
	}

	for (int i = 0; i < randId; i++) {
		cur = objList[cur->hoNumNext].oblOffset;
	}

	lua_pushcfunction(L, Object::NewObject);
	lua_pushinteger(L, cur->hoOi << 16 | cur->hoNumber);
	lua_call(L, 1, 1);

	return 1;
}

// Test Backdrop

int ObjectClass::TestBackdrop (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestBackdropFunc);
}

int ObjectClass::TestBackdropFunc (lua_State* L) {
	return ObjectClass::TestBackdropUnified(L, true);
}

int ObjectClass::TestBackdropGet (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestBackdropGetFunc);
}

int ObjectClass::TestBackdropGetFunc (lua_State* L) {
	return ObjectClass::TestBackdropUnified(L, false);
}

int ObjectClass::TestBackdropUnified (lua_State* L, bool testOnly) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPRH));
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));
	LPOBL objList = rh->rhObjectList;

	int layer = Collisions::LayerParam(L, 1);

	LPHO cur = objList[oi->oilObject].oblOffset;
	
	while (cur->hoNumPrev >= 0) {
		cur = objList[cur->hoNumPrev].oblOffset;
	}

	GenList list;

	while (true) {
		tagRCOM* com = GetRCOM(cur);
		if (Collisions::BackdropTestSprite(rh, com->rcSprite, 0, 0, layer)) {
			if (testOnly) {
				lua_pushboolean(L, true);
				return 1;
			}

			list.push_back(cur);
		}

		if (cur->hoNumNext < 0)
			break;
		cur = objList[cur->hoNumNext].oblOffset;
	}

	if (testOnly) {
		lua_pushboolean(L, false);
		return 1;
	}
	
	return ReturnObjects(L, list);
}

// Test Class

int ObjectClass::TestClass (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestClassFunc);
}

int ObjectClass::TestClassFunc (lua_State* L) {
	return ObjectClass::TestClassUnified(L, true);
}

int ObjectClass::TestClassGet (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestClassGetFunc);
}

int ObjectClass::TestClassGetFunc (lua_State* L) {
	return ObjectClass::TestClassUnified(L, false);
}

int ObjectClass::TestClassUnified (lua_State* L, bool testOnly) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPRH));
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));
	LPOBL objList = rh->rhObjectList;

	LPOIL oi2 = ObjectClass::GetObjectClass(L, 1);
	if (!oi2)
		return 0;

	int layer = Collisions::LayerParam(L, 1);

	// Flag check
	ColFlagCheck(L, rh, oi);
	ColFlagCheck(L, rh, oi2);

	LPHO cur = objList[oi->oilObject].oblOffset;
	
	while (cur->hoNumPrev >= 0) {
		cur = objList[cur->hoNumPrev].oblOffset;
	}

	GenList list;

	while (true) {
		tagRCOM* com = GetRCOM(cur);
		if (Collisions::ClassTestSprite(rh, oi2, com->rcSprite, 0, 0, layer, SAME_CLASS)) {
			if (testOnly) {
				lua_pushboolean(L, true);
				return 1;
			}
			else
				list.push_back(cur);
		}

		if (cur->hoNumNext < 0)
			break;
		cur = objList[cur->hoNumNext].oblOffset;
	}

	if (testOnly) {
		lua_pushboolean(L, false);
		return 1;
	}
	
	return ReturnObjects(L, list);
}

int ObjectClass::TestClassGetEx (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestClassGetExFunc);
}

int ObjectClass::TestClassGetExFunc (lua_State* L) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPRH));
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));
	LPOBL objList = rh->rhObjectList;

	LPOIL oi2 = ObjectClass::GetObjectClass(L, 1);
	if (!oi2)
		return 0;

	int layer = Collisions::LayerParam(L, 1);

	// Flag check
	ColFlagCheck(L, rh, oi);
	ColFlagCheck(L, rh, oi2);

	LPHO cur = objList[oi->oilObject].oblOffset;
	
	while (cur->hoNumPrev >= 0) {
		cur = objList[cur->hoNumPrev].oblOffset;
	}

	GenList tempList;
	lua_createtable(L, 0, 0);

	while (true) {
		tagRCOM* com = GetRCOM(cur);
		tempList.clear();

		Collisions::ObjectListBySprite(tempList, rh, oi2, com->rcSprite, 0, 0, layer, SAME_CLASS);

		if (tempList.size() > 0) {
			lua_pushcfunction(L, Object::NewObject);
			lua_pushinteger(L, cur->hoOi << 16 | cur->hoNumber);
			lua_call(L, 1, 1);

			ReturnObjects(L, tempList);
			lua_rawset(L, -3);
		}

		if (cur->hoNumNext < 0)
			break;
		cur = objList[cur->hoNumNext].oblOffset;
	}

	return 1;
}


// Test Point

int ObjectClass::TestObject (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestObjectFunc);
}

int ObjectClass::TestObjectFunc (lua_State* L) {
	return ObjectClass::TestObjectUnified(L, true);
}

int ObjectClass::TestObjectGet (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestObjectGetFunc);
}

int ObjectClass::TestObjectGetFunc (lua_State* L) {
	return ObjectClass::TestObjectUnified(L, false);
}

int ObjectClass::TestObjectUnified (lua_State* L, bool testOnly) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPRH));
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));
	LPOBL objList = rh->rhObjectList;

	LPHO ho = Object::GetObject(L, 1);
	tagRCOM* com2 = GetRCOM(ho);

	if (!ho || !com2)
		return 0;
	
	int layer = Collisions::LayerParam(L, 2);

	// Flag check
	Sprites::ColFlagCheck(L, rh, ho);
	ColFlagCheck(L, rh, oi);

	if (testOnly) {
		lua_pushboolean(L, Collisions::ClassTestSprite(rh, oi, com2->rcSprite, 0, 0, layer, SAME_CLASS));
		return 1;
	}

	GenList list;
	Collisions::ObjectListBySprite(list, rh, oi, com2->rcSprite, 0, 0, layer, SAME_CLASS);
	
	return ReturnObjects(L, list);
}

// Test Point

int ObjectClass::TestPoint (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestPointFunc);
}

int ObjectClass::TestPointFunc (lua_State* L) {
	return ObjectClass::TestPointUnified(L, true);
}

int ObjectClass::TestPointGet (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestPointGetFunc);
}

int ObjectClass::TestPointGetFunc (lua_State* L) {
	return ObjectClass::TestPointUnified(L, false);
}

int ObjectClass::TestPointUnified (lua_State* L, bool testOnly) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPRH));
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));
	LPOBL objList = rh->rhObjectList;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	
	int layer = Collisions::LayerParam(L, 3);

	if (testOnly) {
		lua_pushboolean(L, Collisions::ClassTestPoint(rh, oi, x, y, layer, SAME_CLASS));
		return 1;
	}

	GenList list;
	Collisions::ObjectListByPoint(list, rh, oi, x, y, layer, SAME_CLASS);
	
	return ReturnObjects(L, list);
}

// Test Rect

int ObjectClass::TestRect (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestRectFunc);
}

int ObjectClass::TestRectFunc (lua_State* L) {
	return ObjectClass::TestRectUnified(L, true);
}

int ObjectClass::TestRectGet (lua_State* L) {
	return ObjectClass::MemoClosure(L, lua_tostring(L, 2), ObjectClass::TestRectGetFunc);
}

int ObjectClass::TestRectGetFunc (lua_State* L) {
	return ObjectClass::TestRectUnified(L, false);
}

int ObjectClass::TestRectUnified (lua_State* L, bool testOnly) {
	LPRH rh = (LPRH)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPRH));
	LPOIL oi = (LPOIL)lua_touserdata(L, lua_upvalueindex(UV_CLASS_LPOIL));
	LPOBL objList = rh->rhObjectList;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	
	int layer = Collisions::LayerParam(L, 5);

	if (testOnly) {
		lua_pushboolean(L, Collisions::ClassTestRect(rh, oi, x, y, w, h, layer, SAME_CLASS));
		return 1;
	}

	GenList list;
	Collisions::ObjectListByRect(list, rh, oi, x, y, w, h, layer, SAME_CLASS);
	
	return ReturnObjects(L, list);
}
