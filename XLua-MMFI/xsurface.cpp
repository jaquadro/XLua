#include "xsurface.h"
#include "array.h"

#include <new>

static int NewXLuaSurface (lua_State* L, BaseSurfaceRefFactory* factory, bool init = false);

int XSurface::IndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_SURFACE)
		return 0;

	return StandardIndex(L, SurfaceRead, SurfaceWrite);
}

int XSurface::NewIndexMetamethod (lua_State* L) {
	if (lua_tonumber(L, lua_upvalueindex(UV_TYPE)) != TYPE_SURFACE)
		return 0;

	return StandardNewIndex(L, SurfaceRead, SurfaceWrite);
}

int XSurface::NewSurface (lua_State* L) {
	AutoSurfaceRefFactory factory;
	return NewSurfaceImpl(L, &factory, true);
}

int XSurface::NewSurfaceImpl (lua_State* L, BaseSurfaceRefFactory* factory, bool init) {
	//LPRH rh = xlua_get_run_header(L);

	int base = lua_gettop(L);								// +0

	lua_checkstack(L, base + 6);

	lua_CFunction index_method = XSurface::IndexMetamethod;
	lua_CFunction newindex_method = XSurface::NewIndexMetamethod;

	NewXLuaSurface(L, factory, init);						// +1 = Surface Userdata

	// Create object table
	lua_createtable(L, 0, 0);								// +2 = Object Table

	// Create object metatable
	lua_createtable(L, 0, 3);								// +3 = Object Metatable

	// Add __index
	lua_pushnumber(L, TYPE_SURFACE);						// +4
	lua_pushvalue(L, base + 1);								// +5
	lua_pushcclosure(L, index_method, 2);					// +4
	lua_setfield(L, -2, "__index");							// +3

	// Add __newindex
	lua_pushnumber(L, TYPE_SURFACE);						// +4
	lua_pushvalue(L, base + 1);								// +5
	lua_pushcclosure(L, newindex_method, 2);				// +4
	lua_setfield(L, -2, "__newindex");						// +3

	// Add __metatable
	lua_pushstring(L, "Access violation");					// +4
	lua_setfield(L, -2, "__metatable");						// +3
	
	// Bind metatable
	lua_setmetatable(L, -2);								// +2

	return 1;
}

XSurfaceUD* XSurface::GetSurface (lua_State* L, int index) {
	if (!lua_istable(L, index))
		return 0;

	int base = lua_gettop(L);								// +0
	lua_checkstack(L, base + 4);

	lua_getmetatable(L, index);								// +1 = Object metatable
	lua_pushstring(L, "__index");							// +2
	lua_rawget(L, -2);										// +2 = __index method

	lua_getupvalue(L, base + 2, UV_TYPE);					// +3 = "TYPE" upvalue
	if (lua_tointeger(L, base + 3) != TYPE_SURFACE)
		return 0;

	lua_getupvalue(L, base + 2, UV_SURFACE_USF);			// +4 = "SurfaceRef*" upvalue

	XSurfaceUD* xud = (XSurfaceUD*)lua_touserdata(L, base + 4);

	lua_pop(L, 4);
	return xud;
}

int XSurface::MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc) {
	return ::MemoClosure(L, key, lfunc, 2);
}

// -----

int XSurface::Width (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	lua_pushnumber(L, xud->ref->getSurface()->GetWidth());
	return 1;
}

int XSurface::Height (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	lua_pushnumber(L, xud->ref->getSurface()->GetHeight());
	return 1;
}

// -----

int XSurface::SetInnerColor1 (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->inColor1 = lua_tointeger(L, PARAM1);
	}
	else if (lua_istable(L, PARAM1)) {
		int n = luaL_getn(L, PARAM1);
		if (n != 3)
			return 0;

		int colors[3];
		for (int i = 0; i < 3; i++) {
			lua_rawgeti(L, PARAM1, i + 1);
			colors[i] = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}

		xud->state->inColor1 = RGB(colors[0], colors[1], colors[2]);
	}

	return 0;
}

int XSurface::SetInnerColor2 (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->inColor2 = lua_tointeger(L, PARAM1);
	}
	else if (lua_istable(L, PARAM1)) {
		int n = luaL_getn(L, PARAM1);
		if (n != 3)
			return 0;

		int colors[3];
		for (int i = 0; i < 3; i++) {
			lua_rawgeti(L, PARAM1, i + 1);
			colors[i] = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}

		xud->state->inColor2 = RGB(colors[0], colors[1], colors[2]);
	}

	return 0;
}

int XSurface::SetInnerFillMode (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->inFillMode = lua_tointeger(L, PARAM1);

		if (xud->state->inFillMode < 1 || xud->state->inFillMode > 3)
			xud->state->inFillMode = XSurfaceState::FILL_SOLID;
	}

	return 0;
}

int XSurface::SetInnerGradientDir (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->inGradDir = lua_tointeger(L, PARAM1);

		if (xud->state->inGradDir < 1 || xud->state->inGradDir > 2)
			xud->state->inGradDir = XSurfaceState::GRAD_HORIZ;
	}

	return 0;
}

int XSurface::SetInnerPattern (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");

	if (lua_isnil(L, PARAM1) && xud->state->inPattern != 0) {
		delete xud->state->inPattern;
		xud->state->inPattern = 0;
		return 0;
	}

	XSurfaceUD* pat = GetSurface(L, PARAM1);
	if (!pat)
		return 0;

	xud->state->inPattern = pat->ref->clone(); //new SurfaceRef(*pat->ref);

	return 0;
}

int XSurface::SetInnerOffsetX (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->inOffX = lua_tointeger(L, PARAM1);
	}

	return 0;
}

int XSurface::SetInnerOffsetY (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->inOffY = lua_tointeger(L, PARAM1);
	}

	return 0;
}

int XSurface::SetOuterColor1 (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->outColor1 = lua_tointeger(L, PARAM1);
	}
	else if (lua_istable(L, PARAM1)) {
		int n = luaL_getn(L, PARAM1);
		if (n != 3)
			return 0;

		int colors[3];
		for (int i = 0; i < 3; i++) {
			lua_rawgeti(L, PARAM1, i + 1);
			colors[i] = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}

		xud->state->outColor1 = RGB(colors[0], colors[1], colors[2]);
	}

	return 0;
}

int XSurface::SetOuterColor2 (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->outColor2 = lua_tointeger(L, PARAM1);
	}
	else if (lua_istable(L, PARAM1)) {
		int n = luaL_getn(L, PARAM1);
		if (n != 3)
			return 0;

		int colors[3];
		for (int i = 0; i < 3; i++) {
			lua_rawgeti(L, PARAM1, i + 1);
			colors[i] = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}

		xud->state->outColor2 = RGB(colors[0], colors[1], colors[2]);
	}

	return 0;
}

int XSurface::SetOuterFillMode (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->outFillMode = lua_tointeger(L, PARAM1);

		if (xud->state->outFillMode < 1 || xud->state->outFillMode > 3)
			xud->state->outFillMode = XSurfaceState::FILL_SOLID;
	}

	return 0;
}

int XSurface::SetOuterGradientDir (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->outGradDir = lua_tointeger(L, PARAM1);

		if (xud->state->outGradDir < 1 || xud->state->outGradDir > 2)
			xud->state->outGradDir = XSurfaceState::GRAD_HORIZ;
	}

	return 0;
}

int XSurface::SetOuterPattern (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");

	if (lua_isnil(L, PARAM1) && xud->state->outPattern != 0) {
		delete xud->state->outPattern;
		xud->state->outPattern = 0;
		return 0;
	}

	XSurfaceUD* pat = GetSurface(L, PARAM1);
	if (!pat)
		return 0;

	xud->state->outPattern = pat->ref->clone(); //new SurfaceRef(*pat->ref);

	return 0;
}

int XSurface::SetOuterOffsetX (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->outOffX = lua_tointeger(L, PARAM1);
	}

	return 0;
}

int XSurface::SetOuterOffsetY (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->outOffY = lua_tointeger(L, PARAM1);
	}

	return 0;
}

int XSurface::SetUseInnerFill (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	xud->state->useInFill = (bool)lua_toboolean(L, PARAM1);

	return 0;
}

int XSurface::SetUseOuterFill (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	xud->state->useOutFill = (bool)lua_toboolean(L, PARAM1);

	return 0;
}

int XSurface::SetUseAA (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	xud->state->useAA = (bool)lua_toboolean(L, PARAM1);

	return 0;
}

int XSurface::SetTransparent (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	xud->state->transparent = (bool)lua_toboolean(L, PARAM1);

	return 0;
}

int XSurface::SetLineWidth (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->thickness = lua_tointeger(L, PARAM1);

		if (xud->state->thickness < 0)
			xud->state->thickness = 0;
	}

	return 0;
}

int XSurface::SetInkEffect (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->inkEffect = (BlitOp)lua_tointeger(L, PARAM1);

		if (xud->state->inkEffect < 0 ||
			xud->state->inkEffect >= BOP_MAX ||
			xud->state->inkEffect == BOP_BLEND_REPLACETRANSP ||
			xud->state->inkEffect == BOP_DWROP ||
			xud->state->inkEffect == BOP_BLEND_DONTREPLACECOLOR)
			xud->state->inkEffect = BOP_COPY;
	}

	return 0;
}

int XSurface::SetInkEffectParam (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	
	if (lua_isnumber(L, PARAM1)) {
		xud->state->inkEffectParam = lua_tointeger(L, PARAM1);

		if (xud->state->inkEffectParam < 0)
			xud->state->inkEffectParam = 0;
		if (xud->state->inkEffectParam > 128)
			xud->state->inkEffectParam = 128;
	}

	return 0;
}

// -----

int XSurface::Copy (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::CopyFunc);
}

int XSurface::CopyFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;
	
	AutoSurfaceRefFactory factory;
	XSurface::NewSurfaceImpl(L, &factory, false);

	XSurfaceUD* cxud = XSurface::GetSurface(L, -1);

	cxud->ref->getSurface()->Clone(*xud->ref->getSurface());

	return 1;
}

int XSurface::CopyBlock (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::CopyBlockFunc);
}

int XSurface::CopyBlockFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	cSurface* src = xud->ref->getSurface();

	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)))
		return 0;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);

	if (x + w > src->GetWidth())
		w = src->GetWidth() - x;
	if (y + h > src->GetHeight())
		h = src->GetHeight() - y;
	
	AutoSurfaceRefFactory factory;
	XSurface::NewSurfaceImpl(L, &factory, false);

	XSurfaceUD* cxud = XSurface::GetSurface(L, -1);

	cSurface* dst = cxud->ref->getSurface();

	LPRH rh = xlua_get_run_header(L);
	LPSURFACE wSurf = WinGetSurface((int)rh->rh4.rh4Mv->mvIdEditWin);

	if (!wSurf) {
		luaL_error(L, "Error creating surface object");
	}

	dst->Create(w, h, wSurf);

	BlitMode bm = (xud->state->transparent) ? BMODE_TRANSP : BMODE_OPAQUE;
	DWORD bltf = BLTF_COPYALPHA | (xud->state->useAA ? BLTF_ANTIA : 0);

	src->Blit(*dst, 0, 0, x, y, w, h, bm, xud->state->inkEffect, xud->state->inkEffectParam, bltf);

	return 1;
}

// -----

int XSurface::CopyInto (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::CopyIntoFunc);
}

int XSurface::CopyIntoFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	cSurface* src = xud->ref->getSurface();

	XSurfaceUD* dxud = XSurface::GetSurface(L, 1);
	if (!dxud)
		return 0;

	cSurface* dst = dxud->ref->getSurface();

	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		return 0;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);

	BlitMode bm = (xud->state->transparent) ? BMODE_TRANSP : BMODE_OPAQUE;
	DWORD bltf = BLTF_COPYALPHA | (xud->state->useAA ? BLTF_ANTIA : 0);

	src->Blit(*dst, x, y, bm, xud->state->inkEffect, xud->state->inkEffectParam, bltf);

	return 0;
}

int XSurface::CopyBlockInto (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::CopyBlockIntoFunc);
}

int XSurface::CopyBlockIntoFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	cSurface* src = xud->ref->getSurface();

	XSurfaceUD* dxud = XSurface::GetSurface(L, 1);
	if (!dxud)
		return 0;

	cSurface* dst = dxud->ref->getSurface();

	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) &&
		  lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6)))
		return 0;

	int dx = lua_tointeger(L, 1);
	int dy = lua_tointeger(L, 2);
	int sx = lua_tointeger(L, 3);
	int sy = lua_tointeger(L, 4);
	int w = lua_tointeger(L, 5);
	int h = lua_tointeger(L, 6);

	if (sx + w > src->GetWidth())
		w = src->GetWidth() - sx;
	if (sy + h > src->GetHeight())
		h = src->GetHeight() - sy;

	BlitMode bm = (xud->state->transparent) ? BMODE_TRANSP : BMODE_OPAQUE;
	DWORD bltf = BLTF_COPYALPHA | (xud->state->useAA ? BLTF_ANTIA : 0);

	src->Blit(*dst, dx, dy, sx, sy, w, h, bm, xud->state->inkEffect, xud->state->inkEffectParam, bltf);

	return 0;
}

int XSurface::SetPixel (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::SetPixelFunc);
}

int XSurface::SetPixelFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;
	
	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		return 0;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);

	if (lua_isnumber(L, 3)) {
		xud->ref->getSurface()->SetPixel(x, y, lua_tointeger(L, 3));
	}
	else if (lua_istable(L, 3)) {
		int n = luaL_getn(L, 3);
		if (n != 3)
			return 0;

		int colors[3];
		for (int i = 0; i < 3; i++) {
			lua_rawgeti(L, 3, i + 1);
			colors[i] = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}

		xud->ref->getSurface()->SetPixel(x, y, RGB(colors[0], colors[1], colors[2]));
	}

	return 0;
}

int XSurface::Ellipse (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::EllipseFunc);
}

int XSurface::EllipseFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)))
		return 0;

	if (!xud->state->useInFill && !xud->state->useOutFill)
		return 0;

	int x1 = lua_tointeger(L, 1);
	int y1 = lua_tointeger(L, 2);
	int x2 = lua_tointeger(L, 3);
	int y2 = lua_tointeger(L, 4);

	if (xud->state->inFillMode == XSurfaceState::FILL_SOLID &&
		xud->state->useAA == false &&
		xud->state->transparent == false &&
		xud->state->inkEffect == BOP_COPY) {

		int t = (xud->state->useOutFill) ? xud->state->thickness : 0;
		xud->ref->getSurface()->Ellipse(x1, y1, x2, y2, xud->state->inColor1, t, xud->state->outColor1, xud->state->useInFill);
	}
	else {
		CFillData* ifd = 0;
		if (xud->state->useInFill) {
			if (xud->state->inFillMode == XSurfaceState::FILL_SOLID)
				ifd = new CFillFlat(xud->state->inColor1);
			else if (xud->state->inFillMode == XSurfaceState::FILL_GRAD)
				ifd = new CFillDir(xud->state->inColor1, xud->state->inColor2, (xud->state->inGradDir == XSurfaceState::GRAD_VERT), xud->state->inOffX, xud->state->inOffY);
			else if (xud->state->inFillMode == XSurfaceState::FILL_PATTERN)
				ifd = new CFillMosaic(xud->state->inPattern->getSurface(), xud->state->inOffX, xud->state->inOffY);
		}

		CFillData* ofd = 0;
		if (xud->state->useOutFill) {
			if (xud->state->outFillMode == XSurfaceState::FILL_SOLID)
				ofd = new CFillFlat(xud->state->outColor1);
			else if (xud->state->outFillMode == XSurfaceState::FILL_GRAD)
				ofd = new CFillDir(xud->state->outColor1, xud->state->outColor2, (xud->state->outGradDir == XSurfaceState::GRAD_VERT), xud->state->outOffX, xud->state->outOffY);
			else if (xud->state->outFillMode == XSurfaceState::FILL_PATTERN)
				ofd = new CFillMosaic(xud->state->outPattern->getSurface(), xud->state->outOffX, xud->state->outOffY);
		}

		int t = (xud->state->useOutFill) ? xud->state->thickness : 0;
		BlitMode bm = (xud->state->transparent) ? BMODE_TRANSP : BMODE_OPAQUE;

		xud->ref->getSurface()->Ellipse(x1, y1, x2, y2, ifd, t, ofd, xud->state->useAA, bm, 
			xud->state->inkEffect, xud->state->inkEffectParam, xud->state->useInFill);

		if (ifd)
			delete ifd;
		if (ofd)
			delete ofd;
	}

	return 0;
}

int XSurface::Rectangle (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::RectangleFunc);
}

int XSurface::RectangleFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)))
		return 0;

	if (!xud->state->useInFill && !xud->state->useOutFill)
		return 0;

	int x1 = lua_tointeger(L, 1);
	int y1 = lua_tointeger(L, 2);
	int x2 = lua_tointeger(L, 3);
	int y2 = lua_tointeger(L, 4);

	if (xud->state->inFillMode == XSurfaceState::FILL_SOLID &&
		xud->state->useAA == false &&
		xud->state->transparent == false &&
		xud->state->inkEffect == BOP_COPY) {

		int t = (xud->state->useOutFill) ? xud->state->thickness : 0;
		xud->ref->getSurface()->Rectangle(x1, y1, x2, y2, xud->state->inColor1, t, xud->state->outColor1, xud->state->useInFill);
	}
	else {
		CFillData* ifd = 0;
		if (xud->state->useInFill) {
			if (xud->state->inFillMode == XSurfaceState::FILL_SOLID)
				ifd = new CFillFlat(xud->state->inColor1);
			else if (xud->state->inFillMode == XSurfaceState::FILL_GRAD)
				ifd = new CFillDir(xud->state->inColor1, xud->state->inColor2, (xud->state->inGradDir == XSurfaceState::GRAD_VERT), xud->state->inOffX, xud->state->inOffY);
			else if (xud->state->inFillMode == XSurfaceState::FILL_PATTERN)
				ifd = new CFillMosaic(xud->state->inPattern->getSurface(), xud->state->inOffX, xud->state->inOffY);
		}

		CFillData* ofd = 0;
		if (xud->state->useOutFill) {
			if (xud->state->outFillMode == XSurfaceState::FILL_SOLID)
				ofd = new CFillFlat(xud->state->outColor1);
			else if (xud->state->outFillMode == XSurfaceState::FILL_GRAD)
				ofd = new CFillDir(xud->state->outColor1, xud->state->outColor2, (xud->state->outGradDir == XSurfaceState::GRAD_VERT), xud->state->outOffX, xud->state->outOffY);
			else if (xud->state->outFillMode == XSurfaceState::FILL_PATTERN)
				ofd = new CFillMosaic(xud->state->outPattern->getSurface(), xud->state->outOffX, xud->state->outOffY);
		}

		int t = (xud->state->useOutFill) ? xud->state->thickness : 0;
		BlitMode bm = (xud->state->transparent) ? BMODE_TRANSP : BMODE_OPAQUE;

		xud->ref->getSurface()->Rectangle(x1, y1, x2, y2, ifd, t, ofd, xud->state->useAA, bm, 
			xud->state->inkEffect, xud->state->inkEffectParam, xud->state->useInFill);

		if (ifd)
			delete ifd;
		if (ofd)
			delete ofd;
	}

	return 0;
}

int XSurface::Polygon (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::PolygonFunc);
}

int XSurface::PolygonFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	if (!(lua_istable(L, 1)))
		return 0;

	if (!xud->state->useInFill && !xud->state->useOutFill)
		return 0;

	long* points = 0;
	int numPoints = 0;

	int dims = XSDK::getDimensions(L, 1);
	if (dims == 1) {
		if (!XSDK::getArray1D(L, 1, XSDK::TYPE_INTEGER, &points, &numPoints))
			return 0;

		if (!numPoints || numPoints % 2 != 0)
			return 0;

		numPoints /= 2;
	}
	else if (dims == 2) {
		int n2 = 2;
		if (!XSDK::getArray2D(L, 1, XSDK::TYPE_INTEGER, &points, &numPoints, &n2))
			return 0;

		if (!numPoints)
			return 0;
	}

	if (xud->state->inFillMode == XSurfaceState::FILL_SOLID &&
		xud->state->useAA == false &&
		xud->state->transparent == false &&
		xud->state->inkEffect == BOP_COPY) {

		int t = (xud->state->useOutFill) ? xud->state->thickness : 0;
		xud->ref->getSurface()->Polygon((LPPOINT)points, numPoints, xud->state->inColor1, t, xud->state->outColor1, xud->state->useInFill);
	}
	else {
		CFillData* ifd = 0;
		if (xud->state->useInFill) {
			if (xud->state->inFillMode == XSurfaceState::FILL_SOLID)
				ifd = new CFillFlat(xud->state->inColor1);
			else if (xud->state->inFillMode == XSurfaceState::FILL_GRAD)
				ifd = new CFillDir(xud->state->inColor1, xud->state->inColor2, (xud->state->inGradDir == XSurfaceState::GRAD_VERT), xud->state->inOffX, xud->state->inOffY);
			else if (xud->state->inFillMode == XSurfaceState::FILL_PATTERN)
				ifd = new CFillMosaic(xud->state->inPattern->getSurface(), xud->state->inOffX, xud->state->inOffY);
		}

		CFillData* ofd = 0;
		if (xud->state->useOutFill) {
			if (xud->state->outFillMode == XSurfaceState::FILL_SOLID)
				ofd = new CFillFlat(xud->state->outColor1);
			else if (xud->state->outFillMode == XSurfaceState::FILL_GRAD)
				ofd = new CFillDir(xud->state->outColor1, xud->state->outColor2, (xud->state->outGradDir == XSurfaceState::GRAD_VERT), xud->state->outOffX, xud->state->outOffY);
			else if (xud->state->outFillMode == XSurfaceState::FILL_PATTERN)
				ofd = new CFillMosaic(xud->state->outPattern->getSurface(), xud->state->outOffX, xud->state->outOffY);
		}

		int t = (xud->state->useOutFill) ? xud->state->thickness : 0;
		BlitMode bm = (xud->state->transparent) ? BMODE_TRANSP : BMODE_OPAQUE;

		xud->ref->getSurface()->Polygon((LPPOINT)points, numPoints, ifd, t, ofd, xud->state->useAA, bm, 
			xud->state->inkEffect, xud->state->inkEffectParam, xud->state->useInFill);

		if (ifd)
			delete ifd;
		if (ofd)
			delete ofd;
	}

	if (points)
		delete[] points;

	return 0;
}

int XSurface::Line (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::LineFunc);
}

int XSurface::LineFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)))
		return 0;

	if (!xud->state->useOutFill)
		return 0;

	int x1 = lua_tointeger(L, 1);
	int y1 = lua_tointeger(L, 2);
	int x2 = lua_tointeger(L, 3);
	int y2 = lua_tointeger(L, 4);

	if (xud->state->outFillMode == XSurfaceState::FILL_SOLID &&
		xud->state->useAA == false &&
		xud->state->transparent == false &&
		xud->state->inkEffect == BOP_COPY) {

		xud->ref->getSurface()->Line(x1, y1, x2, y2, xud->state->thickness, xud->state->outColor1);
	}
	else {
		CFillData* ofd = 0;
		if (xud->state->useOutFill) {
			if (xud->state->outFillMode == XSurfaceState::FILL_SOLID)
				ofd = new CFillFlat(xud->state->outColor1);
			else if (xud->state->outFillMode == XSurfaceState::FILL_GRAD)
				ofd = new CFillDir(xud->state->outColor1, xud->state->outColor2, (xud->state->outGradDir == XSurfaceState::GRAD_VERT), xud->state->outOffX, xud->state->outOffY);
			else if (xud->state->outFillMode == XSurfaceState::FILL_PATTERN)
				ofd = new CFillMosaic(xud->state->outPattern->getSurface(), xud->state->outOffX, xud->state->outOffY);
		}

		BlitMode bm = (xud->state->transparent) ? BMODE_TRANSP : BMODE_OPAQUE;

		xud->ref->getSurface()->Line(x1, y1, x2, y2, xud->state->thickness, ofd, xud->state->useAA, bm, 
			xud->state->inkEffect, xud->state->inkEffectParam);

		if (ofd)
			delete ofd;
	}

	return 0;
}

int XSurface::Fill (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::FillFunc);
}

int XSurface::FillFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	if (xud->state->inFillMode == XSurfaceState::FILL_SOLID) {
		xud->ref->getSurface()->Fill(xud->state->inColor1);
	}
	else {
		CFillData* fd;
		if (xud->state->inFillMode == XSurfaceState::FILL_GRAD)
			fd = new CFillDir(xud->state->inColor1, xud->state->inColor2, (xud->state->inGradDir == XSurfaceState::GRAD_VERT), xud->state->inOffX, xud->state->inOffY);
		else if (xud->state->inFillMode == XSurfaceState::FILL_PATTERN)
			fd = new CFillMosaic(xud->state->inPattern->getSurface(), xud->state->inOffX, xud->state->inOffY);

		xud->ref->getSurface()->Fill(fd);
		delete fd;
	}

	return 0;
}

int XSurface::FillBlock (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::FillBlockFunc);
}

int XSurface::FillBlockFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)))
		return 0;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);

	if (xud->state->inFillMode == XSurfaceState::FILL_SOLID) {
		xud->ref->getSurface()->Fill(x, y, w, h, xud->state->inColor1);
	}
	else {
		CFillData* fd;
		if (xud->state->inFillMode == XSurfaceState::FILL_GRAD)
			fd = new CFillDir(xud->state->inColor1, xud->state->inColor2, (xud->state->inGradDir == XSurfaceState::GRAD_VERT), xud->state->inOffX, xud->state->inOffY);
		else if (xud->state->inFillMode == XSurfaceState::FILL_PATTERN)
			fd = new CFillMosaic(xud->state->inPattern->getSurface(), xud->state->inOffX, xud->state->inOffY);

		xud->ref->getSurface()->Fill(x, y, w, h, fd);
		delete fd;
	}

	return 0;
}

int XSurface::FloodFill (lua_State* L) {
	return XSurface::MemoClosure(L, lua_tostring(L, 2), XSurface::FloodFillFunc);
}

int XSurface::FloodFillFunc (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, lua_upvalueindex(UV_SURFACE_USF), "xlua_mmfi_surface");
	if (!xud->ref->valid())
		return 0;

	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2)))
		return 0;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);

	int t = 0;
	if (lua_isnumber(L, 3)) {
		t = lua_tointeger(L, 3);
		if (t < 0)
			t = 0;
	}

	BlitMode bm = (xud->state->transparent) ? BMODE_TRANSP : BMODE_OPAQUE;

	xud->ref->getSurface()->FloodFill(x, y, xud->state->inColor1, xud->state->useAA, t, bm, xud->state->inkEffect, xud->state->inkEffectParam);

	return 0;
}


// --- Surface Raw Object ---

static int NewXLuaSurface (lua_State* L, BaseSurfaceRefFactory* factory, bool init) {
	if (init && lua_gettop(L) < 2) {
		luaL_error(L, "Invalid parameters to mmf.newSurface");
	}

	int width = lua_tointeger(L, 1);
	int height = lua_tointeger(L, 2);

	XSurfaceUD* xud = (XSurfaceUD*)lua_newuserdata(L, sizeof(XSurfaceUD));
	xud->ref = factory->create(); //new AutoSurfaceRef();
	xud->state = new XSurfaceState();

	xud->state->inFillMode = XSurfaceState::FILL_SOLID;
	xud->state->inColor1 = RGB(255, 255, 255);
	xud->state->inColor2 = RGB(16, 16, 16);
	xud->state->inGradDir = XSurfaceState::GRAD_HORIZ;
	xud->state->inPattern = 0;
	xud->state->inOffX = 0;
	xud->state->inOffY = 0;

	xud->state->outFillMode = XSurfaceState::FILL_SOLID;
	xud->state->outColor1 = RGB(16, 16, 16);
	xud->state->outColor2 = RGB(255, 255, 255);
	xud->state->outGradDir = XSurfaceState::GRAD_HORIZ;
	xud->state->outPattern = 0;
	xud->state->outOffX = 0;
	xud->state->outOffY = 0;

	xud->state->useInFill = true;
	xud->state->useOutFill = true;
	xud->state->useAA = true;
	xud->state->transparent = true;
	xud->state->thickness = 1;
	xud->state->inkEffect = BOP_COPY;
	xud->state->inkEffectParam = 0;

	xud->state->textAlign = 0;
	xud->state->textColor = RGB(16, 16, 16);

	xud->state->angleMode = XSurfaceState::ANGLE_DEGREES;

	//SurfaceRef* sr = new (buf) SurfaceRef();

	if (init) {
		LPRH rh = xlua_get_run_header(L);
		LPSURFACE wSurf = WinGetSurface((int)rh->rh4.rh4Mv->mvIdEditWin);

		if (!wSurf) {
			luaL_error(L, "Error creating surface object");
		}

		xud->ref->getSurface()->Create(width, height, wSurf);
	}

	luaL_getmetatable(L, "xlua_mmfi_surface");
	lua_setmetatable(L, -2);

	return 1;
}

static int GCXLuaSurface (lua_State* L) {
	XSurfaceUD* xud = (XSurfaceUD*)luaL_checkudata(L, 1, "xlua_mmfi_surface");

	if (xud) {
		delete xud->ref;
		delete xud->state;
	}

	delete xud;

	return 0;
}

static const struct luaL_Reg xlua_surface_m[] = {
	{"__gc", GCXLuaSurface},
	{NULL, NULL}
};

// --- ---

int XSurface::Register (lua_State* L) {
	luaL_newmetatable(L, "xlua_mmfi_surface");
	luaL_register(L, NULL, xlua_surface_m);

	lua_getglobal(L, "mmf");

	//PushStandardClosure(L, ObjectClass::NewObjectClass);

	lua_pushcfunction(L, XSurface::NewSurface);
	lua_setfield(L, -2, "newSurface");

	lua_pop(L, 1);

	return 0;
}

/*extern "C" __declspec(dllexport) int luaopen_surface (lua_State *L) {
	luaL_newmetatable(L, "xlua_mmfi_surface");
	luaL_register(L, NULL, arraylib_m);
	//luaL_register(L, "array", arraylib_f);
	return 0;
}*/

