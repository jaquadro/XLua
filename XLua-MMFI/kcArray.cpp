#include "kcobject.h"
#include "object.h"

int KCOArray::IndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_OBJECT)
		return 0;

	// Check that object is valid
	if (!ObjectCheck(L))
		return 0;

	int ret;
	
	ret = Common::CommonIndex(L);
	if (ret > -1) return ret;

	// Our fields
	ret = StandardIndex(L, KCOArrayRead, KCOArrayWrite);
	if (ret > -1) return ret;

	// Simulate some inheritance
	return Object::InheritCommon(L);
}

int KCOArray::NewIndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_OBJECT)
		return 0;

	// Check that object is valid
	if (!ObjectCheck(L)) {
		return 0;
	}

	int ret;
	
	ret = Common::CommonNewIndex(L);
	if (ret > -1) return ret;

	// Our fields
	ret = StandardNewIndex(L, KCOArrayRead, KCOArrayWrite);
	if (ret > -1) return ret;

	// It's some untracked value, let them add it
	lua_rawset(L, 1);

	return 0;
}

// -----

int KCOArray::XDim (lua_State *L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushinteger(L, o->data->xDimension);
	return 1;
}

int KCOArray::YDim (lua_State *L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushinteger(L, o->data->yDimension);
	return 1;
}

int KCOArray::ZDim (lua_State *L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushinteger(L, o->data->zDimension);
	return 1;
}

int KCOArray::XIndex (lua_State *L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushinteger(L, o->data->xIndex);
	return 1;
}

int KCOArray::YIndex (lua_State *L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushinteger(L, o->data->yIndex);
	return 1;
}

int KCOArray::ZIndex (lua_State *L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	lua_pushinteger(L, o->data->zIndex);
	return 1;
}

int KCOArray::SetXIndex (lua_State* L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	int idx = lua_tointeger(L, PARAM1);
	if (idx <= 0 || idx > o->data->xDimension)
		return 0;

	o->data->xIndex = idx - 1;
	return 0;
}

int KCOArray::SetYIndex (lua_State* L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	int idx = lua_tointeger(L, PARAM1);
	if (idx <= 0 || idx > o->data->yDimension)
		return 0;

	o->data->yIndex = idx - 1;
	return 0;
}

int KCOArray::SetZIndex (lua_State* L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	int idx = lua_tointeger(L, PARAM1);
	if (idx <= 0 || idx > o->data->zDimension)
		return 0;

	o->data->zIndex = idx - 1;
	return 0;
}

int KCOArray::Get (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), KCOArray::GetFunc);
}

int KCOArray::GetFunc (lua_State* L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	int x = o->data->xIndex;
	int y = o->data->yIndex;
	int z = o->data->zIndex;

	if (lua_isnumber(L, 1)) {
		x = lua_tointeger(L, 1) - 1;
		if (x < 0 || x >= o->data->xDimension)
			return 0;
	}

	if (lua_isnumber(L, 2)) {
		y = lua_tointeger(L, 2) - 1;
		if (y < 0 || y >= o->data->yDimension)
			return 0;
	}

	if (lua_isnumber(L, 3)) {
		z = lua_tointeger(L, 3) - 1;
		if (z < 0 || z >= o->data->zDimension)
			return 0;
	}

	int xjmp = 1;
	int yjmp = xjmp * o->data->xDimension;
	int zjmp = yjmp * o->data->yDimension;

	if (o->data->flags & KCARRAY_ISNUMERIC) {
		lua_pushinteger(L, o->data->intData[zjmp * z + yjmp * y + x * xjmp]);
		return 1;
	}

	if (o->data->flags & KCARRAY_ISTEXT) {
		char* str = o->data->strData[zjmp * z + yjmp * y + x * xjmp];
		if (!str)
			lua_pushstring(L, "");
		else
			lua_pushstring(L, o->data->strData[zjmp * z + yjmp * y + x * xjmp]);
		return 1;
	}

	return 0;
}

int KCOArray::Set (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), KCOArray::SetFunc);
}

int KCOArray::SetFunc (lua_State* L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	int x = o->data->xIndex;
	int y = o->data->yIndex;
	int z = o->data->zIndex;

	if (!lua_isnumber(L, 1))
		return 0;

	int val = lua_tointeger(L, 1);

	if (lua_isnumber(L, 2)) {
		x = lua_tointeger(L, 2) - 1;
		if (x < 0 || x >= o->data->xDimension)
			return 0;
	}

	if (lua_isnumber(L, 3)) {
		y = lua_tointeger(L, 3) - 1;
		if (y < 0 || y >= o->data->yDimension)
			return 0;
	}

	if (lua_isnumber(L, 4)) {
		z = lua_tointeger(L, 4) - 1;
		if (z < 0 || z >= o->data->zDimension)
			return 0;
	}

	int xjmp = 1;
	int yjmp = xjmp * o->data->xDimension;
	int zjmp = yjmp * o->data->yDimension;

	if (o->data->flags & KCARRAY_ISNUMERIC) {
		o->data->intData[zjmp * z + yjmp * y + x * xjmp] = val;
	}

	return 0;
}

int KCOArray::Clear (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), KCOArray::ClearFunc);
}

int KCOArray::ClearFunc (lua_State* L) {
	KCArray* o = (KCArray*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	
	if (!(o->data->flags & KCARRAY_ISNUMERIC))
		return 0;

	for (int* val = o->data->intData; val != o->data->intData + o->data->dataLen; val++) {
		*val = 0;
	}

	return 0;
};
