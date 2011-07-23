#include "object.h"
#include "class.h"
#include "collisions.h"

int Sprites::SpritesIndex (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	if ((o->hoOEFlags & OEFLAG_SPRITES) == 0)
		return -1;

	return StandardIndex(L, SpritesRead, SpritesWrite);
}

int Sprites::SpritesNewIndex (lua_State* L) {
	LPHO o = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	if ((o->hoOEFlags & OEFLAG_SPRITES) == 0)
		return -1;

	return StandardNewIndex(L, SpritesRead, SpritesWrite);
}

void Sprites::ColFlagCheck (lua_State* L, LPRH rh, LPHO ho) {
	tagRCOM* com = GetRCOM(ho);
	if (!com)
		return;

	npWin win = rh->rh4.rh4Mv->mvIdEditWin;

	DWORD flags = GetSpriteFlags(win, com->rcSprite);

	if (!(flags & SF_RAMBO))
		SetSpriteFlags(win, com->rcSprite, flags | SF_RAMBO);
}

// Getters

int Sprites::ActionX (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (com->rcSprite) {
		Img info;
		npAppli app = ho->hoAdRunHeader->rh4.rh4Mv->mvIdAppli;
		GetImageInfoEx(app, com->rcSprite->sprImg, 0, 1, 1, &info);
		lua_pushnumber(L, info.imgXAction);
		return 1;
	}

	return 0;
}

int Sprites::ActionY (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (com->rcSprite) {
		Img info;
		npAppli app = ho->hoAdRunHeader->rh4.rh4Mv->mvIdAppli;
		GetImageInfoEx(app, com->rcSprite->sprImg, 0, 1, 1, &info);
		lua_pushnumber(L, info.imgYAction);
		return 1;
	}

	return 0;
}

int Sprites::Angle (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	int rtHWA = ho->hoAdRunHeader->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0);

	if (rtHWA) {
		float val = *(float*) &com->rcAngle;
		lua_pushnumber(L, val);
	}
	else {
		lua_pushinteger(L, com->rcAngle);
	}

	return 1;
}

int Sprites::EdgeAA (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (com->rcSprite) {
		lua_pushboolean(L, com->rcSprite->sprEffect & EFFECTFLAG_ANTIALIAS);
		return 1;
	}

	return 0;
}

int Sprites::InkCoef (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (com->rcSprite) {
		lua_pushnumber(L, com->rcSprite->sprEffectParam);
		return 1;
	}

	return 0;
}

int Sprites::InkEffect (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (com->rcSprite) {
		lua_pushnumber(L, com->rcSprite->sprEffect & EFFECT_MASK);
		return 1;
	}

	return 0;
}

int Sprites::RotationAA (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRSPR* spr = GetRSPR(ho);

	if (spr) {
		lua_pushboolean(L, spr->rsFlags & RSFLAG_ROTATE_ANTIA);
		return 1;
	}

	return 0;
}

int Sprites::ScaleAA (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRSPR* spr = GetRSPR(ho);

	if (spr) {
		lua_pushboolean(L, spr->rsFlags & RSFLAG_SCALE_RESAMPLE);
		return 1;
	}

	return 0;
}

int Sprites::ScaleX (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	lua_pushnumber(L, com->rcScaleX);

	return 1;
}

int Sprites::ScaleY (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	lua_pushnumber(L, com->rcScaleY);

	return 1;
}

int Sprites::Transparent (lua_State* L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRSPR* spr = GetRSPR(ho);

	if (com->rcSprite) {
		lua_pushboolean(L, (com->rcSprite->sprEffect & EFFECTFLAG_TRANSPARENT) == 0);
		return 1;
	}
	else if (spr) {
		lua_pushboolean(L, (spr->rsEffect & EFFECTFLAG_TRANSPARENT) == 0);
		return 1;
	}

	return 0;
}

int Sprites::Visible (lua_State* L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRSPR* spr = GetRSPR(ho);

	if (com->rcSprite) {
		lua_pushboolean(L, (com->rcSprite->sprFlags & SF_HIDDEN) == 0);
		return 1;
	}
	else if (spr) {
		lua_pushboolean(L, (spr->rsFlags & RSFLAG_HIDDEN) == 0);
		return 1;
	}

	return 0;
}

int Sprites::ZOrder (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (com->rcSprite) {
		lua_pushnumber(L, com->rcSprite->sprZOrder);
		return 1;
	}

	return 0;
}

// Mutators

int Sprites::SetAngle (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	int rtHWA = ho->hoAdRunHeader->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0);

	if (rtHWA) {
		float val = (float) lua_tonumber(L, PARAM1);
		com->rcOldAngle = com->rcAngle;
		com->rcAngle = *(long*) &val;
	}
	else {
		com->rcOldAngle = com->rcAngle;
		com->rcAngle = lua_tointeger(L, PARAM1) % 360;
	}

	com->rcChanged = 1;

	return 0;
}

int Sprites::SetEdgeAA (lua_State* L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRSPR* spr = GetRSPR(ho);

	if (com->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

		DWORD effect = com->rcSprite->sprEffect;
		if (lua_toboolean(L, PARAM1) == 1)
			effect = effect | EFFECTFLAG_ANTIALIAS;
		else
			effect = effect & ~EFFECTFLAG_ANTIALIAS;

		ModifSpriteEffect(win, com->rcSprite, effect, com->rcSprite->sprEffectParam);

		if (spr) {
			spr->rsEffect = com->rcSprite->sprEffect;
		}
	}

	com->rcChanged = 1;

	return 0;
}

int Sprites::SetInkEffect (lua_State* L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRSPR* spr = GetRSPR(ho);

	int eff = lua_tointeger(L, PARAM1);
	if (eff < 0 || eff > BOP_MAX)
		eff = 0;
	if (eff == 6 || eff == 7 || eff == 12)
		eff = 0;

	if (com->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

		DWORD effect = (com->rcSprite->sprEffect & ~EFFECT_MASK) | eff;
		ModifSpriteEffect(win, com->rcSprite, effect, com->rcSprite->sprEffectParam);

		if (spr) {
			spr->rsEffect = com->rcSprite->sprEffect;
		}
	}

	com->rcChanged = 1;

	return 0;
}

int Sprites::SetInkCoef (lua_State* L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRSPR* spr = GetRSPR(ho);

	int coef = lua_tointeger(L, PARAM1);
	if (coef < 0)
		coef = 0;
	if (coef > OP_MAX)
		coef = OP_MAX;

	if (com->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

		ModifSpriteEffect(win, com->rcSprite, com->rcSprite->sprEffect, coef);

		if (spr) {
			spr->rsEffectParam = coef;
		}
	}

	com->rcChanged = 1;

	return 0;
}

int Sprites::SetRotationAA (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRSPR* spr = GetRSPR(ho);

	if (spr) {
		if (lua_toboolean(L, PARAM1) == 1) {
			spr->rsFlags = spr->rsFlags | RSFLAG_ROTATE_ANTIA;
		}
		else {
			spr->rsFlags = spr->rsFlags & ~RSFLAG_ROTATE_ANTIA;
		}
	}

	com->rcChanged = 1;

	return 0;
}

int Sprites::SetScaleAA (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRSPR* spr = GetRSPR(ho);

	if (spr) {
		if (lua_toboolean(L, PARAM1) == 1) {
			spr->rsFlags = spr->rsFlags | RSFLAG_SCALE_RESAMPLE;
		}
		else {
			spr->rsFlags = spr->rsFlags & ~RSFLAG_SCALE_RESAMPLE;
		}
	}

	com->rcChanged = 1;

	return 0;
}

int Sprites::SetScaleX (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	com->rcScaleX = (float) lua_tonumber(L, PARAM1);
	com->rcChanged = 1;

	return 0;
}

int Sprites::SetScaleY (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	com->rcScaleY = (float) lua_tonumber(L, PARAM1);
	com->rcChanged = 1;

	return 0;
}

int Sprites::SetTransparent (lua_State* L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRSPR* spr = GetRSPR(ho);

	if (com->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

		DWORD effect = com->rcSprite->sprEffect;
		if (lua_toboolean(L, PARAM1) == 1)
			effect = effect | EFFECTFLAG_TRANSPARENT;
		else
			effect = effect & ~EFFECTFLAG_TRANSPARENT;

		ModifSpriteEffect(win, com->rcSprite, effect, com->rcSprite->sprEffectParam);

		if (spr) {
			spr->rsEffect = com->rcSprite->sprEffect;
		}
	}

	com->rcChanged = 1;

	return 0;
}

int Sprites::SetVisible (lua_State* L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	tagRSPR* spr = GetRSPR(ho);

	int flag = lua_toboolean(L, PARAM1);
	int nflag = (flag) ? 0 : 1;

	if (com->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		ShowSprite(win, com->rcSprite, flag);
	}

	if (spr) {
		spr->rsFlags = (spr->rsFlags & ~RSFLAG_VISIBLE) | (flag << 5);
		spr->rsFlags = (spr->rsFlags & ~RSFLAG_HIDDEN) | (nflag << 0);
		spr->rsCreaFlags = (spr->rsCreaFlags & ~SF_HIDDEN) | (nflag << 7);
	}

	com->rcChanged = 1;

	return 0;
}

// Queries

int Sprites::IsAbove (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::IsAboveFunc);
}

int Sprites::IsAboveFunc (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	
	if (!ObjectCheck(L, PARAM1))
		return 0;

	LPHO ho2 = GetHO(lua_touserdata(L, -1));
	tagRCOM* com2 = GetRCOM(ho2);

	if (ho2 == NULL || com2 == NULL)
		return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		lua_pushboolean(L, IsSpriteAfter(win, com->rcSprite, com2->rcSprite));
		return 1;
	}

	return 0;
}

int Sprites::IsBelow (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::IsBelowFunc);
}

int Sprites::IsBelowFunc (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	
	if (!ObjectCheck(L, PARAM1))
		return 0;

	LPHO ho2 = GetHO(lua_touserdata(L, -1));
	tagRCOM* com2 = GetRCOM(ho2);

	if (ho2 == NULL || com2 == NULL)
		return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		lua_pushboolean(L, IsSpriteBefore(win, com->rcSprite, com2->rcSprite));
		return 1;
	}

	return 0;
}

int Sprites::TestBackdrop (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::TestBackdropFunc);
}

int Sprites::TestBackdropFunc (lua_State* L) {
	if (!MinParamCount(L, "testBackdrop", 0))
		return 0;

	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (ho == NULL || com == NULL)
		return 0;

	int layer = Collisions::LayerParam(L, 1, ho->hoLayer);

	int xoff = 0;
	int yoff = 0;

	if (lua_isnumber(L, 2))
		xoff = lua_tointeger(L, 2);
	if (lua_isnumber(L, 3))
		yoff = lua_tointeger(L, 3);

	if (com->rcSprite) {
		int test = Collisions::BackdropTestSprite(ho->hoAdRunHeader, com->rcSprite, xoff, yoff, layer);
		lua_pushboolean(L, test);
		return 1;
	}

	return 0;
}

int Sprites::TestClass (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::TestClassFunc);
}

int Sprites::TestClassFunc (lua_State* L) {
	if (!MinParamCount(L, "testClass", 1))
		return 0;

	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	LPOIL oi = ObjectClass::GetObjectClass(L, 1);

	if (ho == NULL || com == NULL || oi == NULL)
		return 0;

	int layer = Collisions::LayerParam(L, 2, ho->hoLayer);

	int xoff = 0;
	int yoff = 0;

	if (lua_isnumber(L, 3))
		xoff = lua_tointeger(L, 3);
	if (lua_isnumber(L, 4))
		yoff = lua_tointeger(L, 4);

	// Flag check
	ColFlagCheck(L, ho->hoAdRunHeader, ho);
	ObjectClass::ColFlagCheck(L, ho->hoAdRunHeader, oi);

	if (com->rcSprite) {
		int test = Collisions::ClassTestSprite(ho->hoAdRunHeader, oi, com->rcSprite, xoff, yoff, layer, SAME_CLASS);
		lua_pushboolean(L, test);
		return 1;
	}

	return 0;
}

int Sprites::TestObject (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::TestObjectFunc);
}

int Sprites::TestObjectFunc (lua_State* L) {
	if (!MinParamCount(L, "testObject", 1))
		return 0;

	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	LPHO ho2 = Object::GetObject(L, 1);
	tagRCOM* com2 = GetRCOM(ho2);

	if (ho == NULL || com == NULL || ho2 == NULL || com2 == NULL || com->rcSprite == NULL || com2->rcSprite == NULL)
		return 0;

	if (!Collisions::LayerCheck(L, 2, ho->hoLayer, ho2->hoLayer))
		return 0;

	int xoff = 0;
	int yoff = 0;

	if (lua_isnumber(L, 3))
		xoff = lua_tointeger(L, 3);
	if (lua_isnumber(L, 4))
		yoff = lua_tointeger(L, 4);

	if (com->rcSprite) {
		int test = Collisions::SpriteTestSprite(ho->hoAdRunHeader, com->rcSprite, com2->rcSprite, xoff, yoff);
		lua_pushboolean(L, test);
		return 1;
	}

	return 0;
}

int Sprites::TestPoint (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::TestPointFunc);
}

int Sprites::TestPointFunc (lua_State* L) {
	if (!MinParamCount(L, "testPoint", 2))
		return 0;

	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (ho == NULL || com == NULL)
		return 0;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	
	if (!Collisions::LayerCheck(L, 3, ho->hoLayer))
		return 0;

	if (com->rcSprite) {
		int test = Collisions::SpriteTestPoint(ho->hoAdRunHeader, com->rcSprite, x, y);
		lua_pushboolean(L, test);
		return 1;
	}

	return 0;
}

int Sprites::TestRect (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::TestRectFunc);
}

int Sprites::TestRectFunc (lua_State* L) {
	if (!MinParamCount(L, "testRect", 4))
		return 0;

	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (ho == NULL || com == NULL)
		return 0;

	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	
	if (!Collisions::LayerCheck(L, 5, ho->hoLayer))
		return 0;

	if (com->rcSprite) {
		int test = Collisions::SpriteTestRect(ho->hoAdRunHeader, com->rcSprite, x, y, w, h);
		lua_pushboolean(L, test);
		return 1;
	}

	return 0;
}

// Actions

int Sprites::MoveAbove (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::MoveAboveFunc);
}

int Sprites::MoveAboveFunc (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	
	if (!ObjectCheck(L, PARAM1))
		return 0;

	LPHO ho2 = GetHO(lua_touserdata(L, -1));
	tagRCOM* com2 = GetRCOM(ho2);

	if (ho2 == NULL || com2 == NULL)
		return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteAfter(win, com->rcSprite, com2->rcSprite);
	}

	return 0;
}

int Sprites::MoveBelow (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::MoveBelowFunc);
}

int Sprites::MoveBelowFunc (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	
	if (!ObjectCheck(L, PARAM1))
		return 0;

	LPHO ho2 = GetHO(lua_touserdata(L, -1));
	tagRCOM* com2 = GetRCOM(ho2);

	if (ho2 == NULL || com2 == NULL)
		return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteBefore(win, com->rcSprite, com2->rcSprite);
	}

	return 0;
}

int Sprites::MoveToBack (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::MoveToBackFunc);
}

int Sprites::MoveToBackFunc (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (com->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteToBack(win, com->rcSprite);
	}

	return 0;
}

int Sprites::MoveToFront (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::MoveToFrontFunc);
}

int Sprites::MoveToFrontFunc (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);

	if (com->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteToFront(win, com->rcSprite);
	}

	return 0;
}

int Sprites::SwapOrder (lua_State* L) {
	return Object::MemoClosure(L, lua_tostring(L, 2), Sprites::SwapOrderFunc);
}

int Sprites::SwapOrderFunc (lua_State *L) {
	LPHO ho = GetHO(lua_touserdata(L, lua_upvalueindex(UV_OBJECT_LPHO)));
	tagRCOM* com = GetRCOM(ho);
	
	if (!ObjectCheck(L, PARAM1))
		return 0;

	LPHO ho2 = GetHO(lua_touserdata(L, -1));
	tagRCOM* com2 = GetRCOM(ho2);

	if (ho2 == NULL || com2 == NULL)
		return 0;

	if (com->rcSprite && com2->rcSprite) {
		npWin win = ho->hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		SwapSprites(win, com->rcSprite, com2->rcSprite);
	}

	return 0;
}
