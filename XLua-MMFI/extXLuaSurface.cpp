#include "extobject.h"
#include "object.h"

int ExtXLuaSurface::IndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_OBJECT)
		return 0;

	// Check that object is valid
	if (!ObjectCheck(L))
		return 0;

	int ret;
	
	ret = Common::CommonIndex(L);
	if (ret > -1) return ret;

	// Our fields
	ret = StandardIndex(L, ExtXLuaSurfaceRead, ExtXLuaSurfaceWrite);
	if (ret > -1) return ret;

	// Simulate some inheritance
	return Object::InheritCommon(L);
}

int ExtXLuaSurface::NewIndexMetamethod (lua_State* L) {
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
	ret = StandardNewIndex(L, ExtXLuaSurfaceRead, ExtXLuaSurfaceWrite);
	if (ret > -1) return ret;

	// It's some untracked value, let them add it
	lua_rawset(L, 1);

	return 0;
}

// -----

int ExtXLuaSurface::ReleaseSurface (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), ExtXLuaSurface::ReleaseSurfaceFunc);
}

int ExtXLuaSurface::ReleaseSurfaceFunc (lua_State* L) {
	RDXLuaSurface* o = (RDXLuaSurface*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	if (o->refSf) {
		delete o->refSf;
		o->refSf = 0;
	}

	o->rc.rcChanged = 1;
	
	return 0;
}

int ExtXLuaSurface::SetSurface (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), ExtXLuaSurface::SetSurfaceFunc);
}

int ExtXLuaSurface::SetSurfaceFunc (lua_State* L) {
	RDXLuaSurface* o = (RDXLuaSurface*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	XSurfaceUD* xud = XSurface::GetSurface(L, 1);
	if (!xud || !xud->ref)
		return 0;

	if (!o->refSf)
		o->refSf = xud->ref->clone();
		//o->refSf = new SurfaceRef(*xud->ref);
	else  {
		delete o->refSf;
		o->refSf = xud->ref->clone();
		//*o->refSf = *xud->ref;
	}
	
	o->rc.rcChanged = 1;
	
	return 0;
}

int ExtXLuaSurface::Update (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), ExtXLuaSurface::UpdateFunc);
}

int ExtXLuaSurface::UpdateFunc (lua_State* L) {
	RDXLuaSurface* o = (RDXLuaSurface*)GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));

	o->rc.rcChanged = 1;
	
	return 0;
}
