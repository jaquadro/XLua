#include "extobject.h"
#include "object.h"

int ExtSurface::IndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_OBJECT)
		return 0;

	// Check that object is valid
	if (!ObjectCheck(L))
		return 0;

	int ret;
	
	ret = Common::CommonIndex(L);
	if (ret > -1) return ret;

	// Our fields
	ret = StandardIndex(L, ExtSurfaceRead, ExtSurfaceWrite);
	if (ret > -1) return ret;

	// Simulate some inheritance
	return Object::InheritCommon(L);
}

int ExtSurface::NewIndexMetamethod (lua_State* L) {
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
	ret = StandardNewIndex(L, ExtSurfaceRead, ExtSurfaceWrite);
	if (ret > -1) return ret;

	// It's some untracked value, let them add it
	lua_rawset(L, 1);

	return 0;
}

// -----

int ExtSurface::GetSurface (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), ExtSurface::GetSurfaceFunc);
}

int ExtSurface::GetSurfaceFunc (lua_State* L) {
	RDSurface* o = (RDSurface*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	int index = lua_tointeger(L, 1);

	if (index < 0 || index >= o->imageCount(o))
		return 0;

	SurfaceObjSurfaceRefFactory factory;
	XSurface::NewSurfaceImpl(L, &factory);

	int top = lua_gettop(L);
	XSurfaceUD* xud = XSurface::GetSurface(L, -1);
	
	SurfaceObjSurfaceRef* sr = dynamic_cast<SurfaceObjSurfaceRef*>(xud->ref);
	sr->bind(o, index);
	
	lua_settop(L, top);
	return 1;
}

int ExtSurface::GetSurfaceCount (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), ExtSurface::GetSurfaceCountFunc);
}

int ExtSurface::GetSurfaceCountFunc (lua_State* L) {
	RDSurface* o = (RDSurface*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	lua_pushinteger(L, o->imageCount(o));
	return 1;
}

int ExtSurface::CloneSurface (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), ExtSurface::GetSurfaceFunc);
}

int ExtSurface::CloneSurfaceFunc (lua_State* L) {
	RDSurface* o = (RDSurface*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	int index = lua_tointeger(L, 1);

	if (index < 0 || index >= o->imageCount(o))
		return 0;

	cSurface* surf = o->imageAt(o, index);

	AutoSurfaceRefFactory factory;
	XSurface::NewSurfaceImpl(L, &factory);

	XSurfaceUD* xud = XSurface::GetSurface(L, -1);
	
	/*LPRH rh = xlua_get_run_header(L);
	LPSURFACE wSurf = WinGetSurface((int)rh->rh4.rh4Mv->mvIdEditWin);

	if (!wSurf) {
		luaL_error(L, "Error creating surface object");
	}

	xud->ref->getSurface()->Create(surf->GetWidth(), surf->GetHeight(), surf->Get);*/

	xud->ref->getSurface()->Clone(*surf);
	
	return 1;
}

// -----

int ExtSurface::Update (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), ExtSurface::UpdateFunc);
}

int ExtSurface::UpdateFunc (lua_State* L) {
	RDSurface* o = (RDSurface*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	o->rc.rcChanged = 1;
	
	return 0;
}
