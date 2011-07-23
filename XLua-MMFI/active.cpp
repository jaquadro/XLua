#include "common.h"
#include "active.h"

/**
 * NAME						READ		WRITE
 * object.visible			ROS			ROS/ROC
 * object.scalex			ROC			ROC
 * object.scaley			ROC			ROC
 * object.angle				ROC			[ROS]/ROC
 * object.direction			ROC			ROC
 * object.animationset		ROC			ROC
 * object.animationframe	ROA			ROA
 * object.zorder			ROC
 *
 * object.MoveToFront()					ROC
 * object.MoveToBack()					ROC
 * object.MoveAbove()					ROC
 * object.MoveBelow()					ROC
 * object.IsAbove()			ROC
 * object.IsBelow()			ROC
 * object.SwapOrder()					ROC
 *
 * object.OverlapsBG()
 * object.OverlapsObject()
 * object.OverlapsInstance()
 * object.OberlapList()
 */

int MemoClosure (lua_State* L, const char* key, int (*lfunc)(lua_State*)) {
	lua_getmetatable(L, 1);
	lua_pushstring(L, key);
	lua_rawget(L, -2);
	if (lua_isfunction(L, -1)) {
		return 1;
	}

	lua_pop(L, 1);

	lua_pushstring(L, key);
	lua_pushvalue(L, lua_upvalueindex(1));
	lua_pushcclosure(L, lfunc, 1);
	lua_rawset(L, -3);

	lua_pushstring(L, key);
	lua_rawget(L, -2);
	return 1;
}

int Active::IndexMetamethod (lua_State* L) {
	const char* key = lua_tostring(L, 2);

	// Check that object is valid
	if (!CommonObject::ObjectCheck(L)) {
		return 0;
	}

	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	// Handle overrides first
	if (strcmp(key, "layer") == 0) {
		return Active::Layer(L);
	}

	// Try dispatching CommonObject
	int ret = CommonObject::CommonIndex(L);
	if (ret > -1) return ret;
	
	// Try dispatching Values
	ret = Values::ValuesIndex(L, &(obj->rov));
	if (ret > -1) return ret;

	// AO-specific
	if (strcmp(key, "zorder") == 0) {
		return Active::ZOrder(L);
	}
	else if (strcmp(key, "angle") == 0) {
		return Active::Angle(L);
	}
	else if (strcmp(key, "direction") == 0) {
		return Active::Direction(L);
	}
	else if (strcmp(key, "animationFrame") == 0) {
		return Active::AnimationFrame(L);
	}
	else if (strcmp(key, "animationSet") == 0) {
		return Active::AnimationSet(L);
	}
	else if (strcmp(key, "xscale") == 0) {
		return Active::ScaleX(L);
	}
	else if (strcmp(key, "yscale") == 0) {
		return Active::ScaleY(L);
	}
	else if (strcmp(key, "xaction") == 0) {
		return Active::ActionX(L);
	}
	else if (strcmp(key, "yaction") == 0) {
		return Active::ActionX(L);
	}
	else if (strcmp(key, "visible") == 0) {
		return Active::Visible(L);
	}
	else if (strcmp(key, "isAbove") == 0) {
		return MemoClosure(L, key, Active::IsAbove);
	}
	else if (strcmp(key, "isBelow") == 0) {
		return MemoClosure(L, key, Active::IsBelow);
	}
	else if (strcmp(key, "moveAbove") == 0) {
		return MemoClosure(L, key, Active::MoveAbove);
	}
	else if (strcmp(key, "moveBelow") == 0) {
		return MemoClosure(L, key, Active::MoveBelow);
	}
	else if (strcmp(key, "moveToFront") == 0) {
		return MemoClosure(L, key, Active::MoveToFront);
	}
	else if (strcmp(key, "movetoBack") == 0) {
		return MemoClosure(L, key, Active::MoveToBack);
	}
	else if (strcmp(key, "swapOrder") == 0) {
		return MemoClosure(L, key, Active::SwapOrder);
	}
	else if (strcmp(key, "overlapsBG") == 0) {
		return MemoClosure(L, key, Active::OverlapsBG);
	}
	else if (strcmp(key, "overlapsObject") == 0) {
		return MemoClosure(L, key, Active::OverlapsObject);
	}
	else if (strcmp(key, "overlapsInstance") == 0) {
		return MemoClosure(L, key, Active::OverlapsInstance);
	}
	else if (strcmp(key, "getOverlapping") == 0) {
		return MemoClosure(L, key, Active::GetOverlapping);
	}
	else if (strcmp(key, "update") == 0) {
		return MemoClosure(L, key, Active::Update);
	}
	
	return 0;
}

int Active::NewIndexMetamethod (lua_State* L) {
	const char* key = lua_tostring(L, 2);

	// Check that object is valid
	if (!CommonObject::ObjectCheck(L)) {
		return 0;
	}

	// Handle overrides first
	if (strcmp(key, "layer") == 0) {
		return Active::SetLayer(L);
	}

	// Try dispatching CommonObject
	int ret = CommonObject::CommonNewIndex(L);
	if (ret > -1) return ret;

	// AO-specific
	if (strcmp(key, "angle") == 0) {
		return Active::SetAngle(L);
	}
	else if (strcmp(key, "direction") == 0) {
		return Active::SetDirection(L);
	}
	else if (strcmp(key, "animationFrame") == 0) {
		return Active::SetAnimationFrame(L);
	}
	else if (strcmp(key, "animationSet") == 0) {
		return Active::SetAnimationSet(L);
	}
	else if (strcmp(key, "scalex") == 0) {
		return Active::SetScaleX(L);
	}
	else if (strcmp(key, "scaley") == 0) {
		return Active::SetScaleY(L);
	}
	else if (strcmp(key, "visible") == 0) {
		return Active::SetVisible(L);
	}

	// Protect us from stomping over internal identifiers!
	if (strcmp(key, "zorder") == 0) {
		return ReadOnlyViolation(L, key);
	}

	if (strcmp(key, "isAbove") == 0 ||
		strcmp(key, "isBelow") == 0 ||
		strcmp(key, "moveAbove") == 0 ||
		strcmp(key, "moveBelow") == 0 ||
		strcmp(key, "moveToFront") == 0 ||
		strcmp(key, "moveToBack") == 0 ||
		strcmp(key, "swapOrder") == 0 ||
		strcmp(key, "overlapsBG") == 0 ||
		strcmp(key, "overlapsObject") == 0 ||
		strcmp(key, "overlapsInstance") == 0 ||
		strcmp(key, "getOverlapping") == 0 ||
		strcmp(key, "update") == 0) {
		return FunctionViolation(L, key);
	}

	// It's some untracked value, let them add it
	lua_rawset(L, 1);

	return 0;
}

// =====================================================================================
//
// Overrides
//
// =====================================================================================

int Active::Layer (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	// TODO: Why am I reading the sprite layer and not the object layer?
	if (obj->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rhIdMainWin;
		lua_pushnumber(L, GetSpriteLayer(win, obj->roc.rcSprite) + 1);
		return 1;
	}

	//lua_pushnumber(L, obj->roHo.hoLayer);
	return 0;
}

int Active::SetLayer (lua_State* L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	int layer = lua_tointeger(L, 2);
	if (obj->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		SetSpriteLayer(win, obj->roc.rcSprite, layer - 1);
		obj->roHo.hoLayer = layer - 1;
	}

	return 0;
}

// =====================================================================================
//
// Misc
//
// =====================================================================================

int Active::Update (lua_State* L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	obj->roc.rcChanged = 1;
	return 0;
}

// =====================================================================================
//
// Scale & Rotation
//
// =====================================================================================

int Active::Angle (lua_State* L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	int rtHWA = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0);

	if (rtHWA) {
		float val = *(float*) &obj->roc.rcAngle;
		lua_pushnumber(L, val);
	}
	else {
		lua_pushinteger(L, obj->roc.rcAngle);
	}

	return 1;
}

int Active::SetAngle (lua_State* L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	int rtHWA = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0);

	/*if (lua_toboolean(L, 3) == 1) {
		obj->ros.rsFlags = obj->ros.rsFlags | RSFLAG_ROTATE_ANTIA;
	}
	else {
		obj->ros.rsFlags = obj->ros.rsFlags & ~RSFLAG_ROTATE_ANTIA;
	}*/

	if (rtHWA) {
		float val = (float) lua_tonumber(L, 3);
		obj->roc.rcAngle = *(long*) &val;
	}
	else {
		obj->roc.rcAngle = lua_tointeger(L, 3) % 360;
	}

	return 0;
}

int Active::ScaleX (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushnumber(L, obj->roc.rcScaleX);
	return 1;
}

int Active::ScaleY (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushnumber(L, obj->roc.rcScaleY);
	return 1;
}

int Active::SetScaleX (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	obj->roc.rcScaleX = (float)lua_tonumber(L, 3);
	return 0;
}

int Active::SetScaleY (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	obj->roc.rcScaleY = (float)lua_tonumber(L, 3);
	return 0;
}

// =====================================================================================
//
// Visibility & Display
//
// =====================================================================================

int Active::Visible (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushboolean(L, (obj->ros.rsFlags & RSFLAG_VISIBLE) != 0);
	return 1;
}

int Active::SetVisible (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	// TODO: There is probably something about this that should be symmetric
	int flag = lua_toboolean(L, 3);
	if (obj->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		ShowSprite(win, obj->roc.rcSprite, flag);
	}

	obj->ros.rsFlags = (obj->ros.rsFlags & ~RSFLAG_VISIBLE) | (flag << 5);

	return 0;
}

// =====================================================================================
//
// Direction & Animation
//
// =====================================================================================

int Active::Direction (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushnumber(L, obj->roc.rcDir);
	return 1;
}

int Active::SetDirection (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	obj->roc.rcDir = lua_tointeger(L, 3) % DIRID_MAX;
	return 0;
}

int Active::AnimationSet (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushnumber(L, obj->roc.rcAnim);
	return 1;
}

int Active::AnimationFrame (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushnumber(L, obj->roa.raAnimFrame - 1);
	return 1;
}

int Active::SetAnimationSet (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	obj->roc.rcAnim = lua_tointeger(L, 3);
	return 0;
}

int Active::SetAnimationFrame (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	obj->roa.raAnimFrame = lua_tointeger(L, 3) + 1;
	obj->roa.raAnimFrameForced = obj->roa.raAnimFrame;

	return 0;
}

int Active::ActionX (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (obj->roc.rcSprite) {
		Img info;
		npAppli app = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdAppli;
		GetImageInfoEx(app, obj->roc.rcSprite->sprImg, 0, 1, 1, &info);
		lua_pushnumber(L, info.imgXAction);
		return 1;
	}

	return 0;
}

int Active::ActionY (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (obj->roc.rcSprite) {
		Img info;
		npAppli app = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdAppli;
		GetImageInfoEx(app, obj->roc.rcSprite->sprImg, 0, 1, 1, &info);
		lua_pushnumber(L, info.imgYAction);
		return 1;
	}

	return 0;
}

// =====================================================================================
//
// Order
//
// =====================================================================================

int Active::ZOrder (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (!obj->roc.rcSprite) {
		return 0;
	}
	
	lua_pushnumber(L, obj->roc.rcSprite->sprZOrder);
	return 1;
}

int Active::IsAbove (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (lua_istable(L, 1)) {
		lua_remove(L, 1);
	}

	if (!CommonObject::ObjectCheckOther(L, 1)) {
		return 0;
	}
	LPRO obj2 = (LPRO)lua_touserdata(L, -1);

	if (obj->roc.rcSprite && obj2->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		lua_pushboolean(L, IsSpriteAfter(win, obj->roc.rcSprite, obj2->roc.rcSprite));
		return 1;
	}

	return 0;
}

int Active::IsBelow (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (!CommonObject::ObjectCheckOther(L, 1)) {
		return 0;
	}
	LPRO obj2 = (LPRO)lua_touserdata(L, -1);

	if (obj->roc.rcSprite && obj2->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		lua_pushboolean(L, IsSpriteBefore(win, obj->roc.rcSprite, obj2->roc.rcSprite));
		return 1;
	}

	return 0;
}

int Active::MoveToFront (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (obj->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteToFront(win, obj->roc.rcSprite);
	}

	return 0;
}

int Active::MoveToBack (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (obj->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteToBack(win, obj->roc.rcSprite);
	}

	return 0;
}

int Active::MoveAbove (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (!CommonObject::ObjectCheckOther(L, 1)) {
		return 0;
	}
	LPRO obj2 = (LPRO)lua_touserdata(L, -1);

	if (obj->roc.rcSprite && obj2->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteAfter(win, obj->roc.rcSprite, obj2->roc.rcSprite);
		//obj->ros.rsZOrder = obj->roc.rcSprite->sprZOrder;
	}

	return 0;
}

int Active::MoveBelow (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (!CommonObject::ObjectCheckOther(L, 1)) {
		return 0;
	}
	LPRO obj2 = (LPRO)lua_touserdata(L, -1);

	if (obj->roc.rcSprite && obj2->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		MoveSpriteBefore(win, obj->roc.rcSprite, obj2->roc.rcSprite);
	}

	return 0;
}

int Active::SwapOrder (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (!CommonObject::ObjectCheckOther(L, 1)) {
		return 0;
	}
	LPRO obj2 = (LPRO)lua_touserdata(L, -1);

	if (obj->roc.rcSprite && obj2->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		SwapSprites(win, obj->roc.rcSprite, obj2->roc.rcSprite);
	}

	return 0;
}

// =====================================================================================
//
// Collision & Overlap Detection
//
// =====================================================================================

int Active::OverlapsBG (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	//int layer = lua_tointeger(L, 2);

	if (obj->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;
		int xoff = obj->roHo.hoX - obj->roc.rcOldX;
		int yoff = obj->roHo.hoY - obj->roc.rcOldY;

		if (lua_isnumber(L, 1)) {
			xoff += lua_tointeger(L, 1);
		}
		if (lua_isnumber(L, 2)) {
			yoff += lua_tointeger(L, 2);
		}

		if (ColMask_TestSprite(win, obj->roc.rcSprite, obj->roc.rcSprite->sprImg, 
			obj->roc.rcSprite->sprX + xoff, obj->roc.rcSprite->sprY + yoff, obj->roc.rcSprite->sprAngle,
			obj->roc.rcSprite->sprScaleX, obj->roc.rcSprite->sprScaleY, 0, obj->roc.rcSprite->sprLayer)) {
			lua_pushboolean(L, true);
		}
		else {
			lua_pushboolean(L, false);
		}
	}

	return 1;
}

int Active::OverlapsObject (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	if (!CommonObject::ObjectCheckOther(L, 1)) {
		return 0;
	}
	LPRO obj2 = (LPRO)lua_touserdata(L, -1);

	// Optional parameter
	int e_layer = MF_SAME_LAYER;
	if (lua_isnumber(L, 2) && lua_tointeger(L, 2) == MF_ANY_LAYER) {
		e_layer = MF_ANY_LAYER;
	}

	if (obj->roc.rcSprite && obj2->roc.rcSprite) {
		npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

		// If objects not on the same layer, and layer enforced, return false
		if (e_layer == MF_SAME_LAYER && obj->roc.rcSprite->sprLayer != obj2->roc.rcSprite->sprLayer) {
			lua_pushboolean(L, false);
			return 1;
		}

		sMask* mask1 = GetSpriteMask(win, obj->roc.rcSprite, obj->roc.rcSprite->sprImg, 0);
		sMask* mask2 = GetSpriteMask(win, obj2->roc.rcSprite, obj2->roc.rcSprite->sprImg, 0);

		LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);
		LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

		int x1off = obj->roHo.hoX - obj->roc.rcOldX;
		int y1off = obj->roHo.hoY - obj->roc.rcOldY;
		int x2off = obj2->roHo.hoX - obj2->roc.rcOldX;
		int y2off = obj2->roHo.hoY - obj2->roc.rcOldY;

		if (lua_isnumber(L, 4)) {
			x1off += lua_tointeger(L, 4);
		}
		if (lua_isnumber(L, 5)) {
			y1off += lua_tointeger(L, 5);
		}

		if (Mask_TestMask(maskptr1, maskptr2,
			obj->roc.rcSprite->sprX1 + x1off, obj->roc.rcSprite->sprY1 + y1off, mask1->mkWidth, mask1->mkHeight,
			obj2->roc.rcSprite->sprX1 + x2off, obj2->roc.rcSprite->sprY1 + y2off, mask2->mkWidth, mask2->mkHeight)) {
			lua_pushboolean(L, true);
		}
		else {
			lua_pushboolean(L, false);
		}

		return 1;
	}

	return 0;
}

int Active::OverlapsInstance (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));
	int objclass = lua_tointeger(L, 1);

	// Optional parameter
	int e_layer = MF_SAME_LAYER;
	if (lua_isnumber(L, 2) && lua_tointeger(L, 2) == MF_ANY_LAYER) {
		e_layer = MF_ANY_LAYER;
	}

	if (obj->roc.rcSprite == NULL) return 0;

	npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

	int nSprites = obj->roc.rcSprite->sprCollisList[0];
	if (nSprites <= 0 || nSprites == 0x10000000) {
		lua_pushboolean(L, false);
		return 1;
	}

	sMask* mask1 = GetSpriteMask(win, obj->roc.rcSprite, obj->roc.rcSprite->sprImg, 0);
	LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);

	Spr** sprlist = (Spr**)(obj->roc.rcSprite->sprCollisList[1]);
	for (int i = 0; i < nSprites; i++) {
		Spr* sprite = (Spr*)(sprlist[i]);
		LPRO target = (LPRO)(sprite->sprExtraInfo);
		
		if (target->roHo.hoOi == objclass) {
			if (e_layer == MF_ANY_LAYER || (e_layer == MF_SAME_LAYER && obj->roc.rcSprite->sprLayer == sprite->sprLayer)) {
				sMask* mask2 = GetSpriteMask(win, target->roc.rcSprite, target->roc.rcSprite->sprImg, 0);
				LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

				int x1off = obj->roHo.hoX - obj->roc.rcOldX;
				int y1off = obj->roHo.hoY - obj->roc.rcOldY;
				int x2off = target->roHo.hoX - target->roc.rcOldX;
				int y2off = target->roHo.hoY - target->roc.rcOldY;

				if (Mask_TestMask(maskptr1, maskptr2,
					obj->roc.rcSprite->sprX1 + x1off, obj->roc.rcSprite->sprY1 + y1off, mask1->mkWidth, mask1->mkHeight,
					target->roc.rcSprite->sprX1 + x2off, target->roc.rcSprite->sprY1 + y2off, mask2->mkWidth, mask2->mkHeight)) {
					lua_pushboolean(L, true);
					return 1;
				}
			}
		}
	}

	lua_pushboolean(L, false);
	return 1;
}

int Active::GetOverlapping (lua_State *L) {
	LPRO obj = (LPRO)lua_touserdata(L, lua_upvalueindex(1));

	// Optional parameters
	int e_class = MF_ANY_CLASS;
	int e_layer = MF_SAME_LAYER;

	if (lua_isnumber(L, 1)) {
		e_class = lua_tointeger(L, 1);
	}
	if (lua_isnumber(L, 2) && lua_tointeger(L, 2) == MF_ANY_LAYER) {
		e_layer = MF_ANY_LAYER;
	}

	if (obj->roc.rcSprite == NULL) return 0;

	npWin win = obj->roHo.hoAdRunHeader->rh4.rh4Mv->mvIdEditWin;

	int matchCount = 0;
	lua_newtable(L);

	int nSprites = obj->roc.rcSprite->sprCollisList[0];
	if (nSprites <= 0 || nSprites == 0x10000000) {
		return 1;
	}

	sMask* mask1 = GetSpriteMask(win, obj->roc.rcSprite, obj->roc.rcSprite->sprImg, 0);
	LPBYTE maskptr1 = (LPBYTE)mask1 + sizeof(sMask);

	Spr** sprlist = (Spr**)(obj->roc.rcSprite->sprCollisList[1]);
	for (int i = 0; i < nSprites; i++) {
		Spr* sprite = (Spr*)(sprlist[i]);
		LPRO target = (LPRO)(sprite->sprExtraInfo);
		
		if (e_class == MF_ANY_CLASS || target->roHo.hoOi == e_class) {
			if (e_layer == MF_ANY_LAYER || (e_layer == MF_SAME_LAYER && obj->roc.rcSprite->sprLayer == sprite->sprLayer)) {
				sMask* mask2 = GetSpriteMask(win, target->roc.rcSprite, target->roc.rcSprite->sprImg, 0);
				LPBYTE maskptr2 = (LPBYTE)mask2 + sizeof(sMask);

				int x1off = obj->roHo.hoX - obj->roc.rcOldX;
				int y1off = obj->roHo.hoY - obj->roc.rcOldY;
				int x2off = target->roHo.hoX - target->roc.rcOldX;
				int y2off = target->roHo.hoY - target->roc.rcOldY;

				if (Mask_TestMask(maskptr1, maskptr2,
					obj->roc.rcSprite->sprX1 + x1off, obj->roc.rcSprite->sprY1 + y1off, mask1->mkWidth, mask1->mkHeight,
					target->roc.rcSprite->sprX1 + x2off, target->roc.rcSprite->sprY1 + y2off, mask2->mkWidth, mask2->mkHeight)) {
					matchCount++;
					lua_pushnumber(L, (target->roHo.hoCreationId<<16)+target->roHo.hoNumber);
					lua_rawseti(L, -2, matchCount);
				}
			}
		}
	}

	return 1;
}
