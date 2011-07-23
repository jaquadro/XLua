#include "common.h"
#include "collisions.h"

#include <algorithm>

// Sprite Support

SpriteSupport::SpriteSupport (LPRH rh, npSpr spr) 
	: _rh(rh), _spr(spr) {
	_hwa = rh->rh4.rh4Mv->mvCallFunction(NULL, EF_ISHWA, 0, 0, 0);
}

int SpriteSupport::GetX () {
	if (!_hwa)
		return _spr->sprX;

	RECT rt;
	GetSpriteRect(_rh->rh4.rh4Mv->mvIdEditWin, _spr, &rt);
	return rt.left;
}

int SpriteSupport::GetY () {
	if (!_hwa)
		return _spr->sprY;

	RECT rt;
	GetSpriteRect(_rh->rh4.rh4Mv->mvIdEditWin, _spr, &rt);
	return rt.top;
}

int SpriteSupport::GetFlags () {
	if (!_hwa)
		return _spr->sprFlags;

	return GetSpriteFlags(_rh->rh4.rh4Mv->mvIdEditWin, _spr);
}

int SpriteSupport::GetAngle () {
	if (!_hwa)
		return _spr->sprAngle;

	BOOL aa;
	return GetSpriteAngle(_rh->rh4.rh4Mv->mvIdEditWin, _spr, &aa);
}

void SpriteSupport::GetScale (float& sx, float& sy) {
	if (!_hwa) {
		sx = _spr->sprScaleX;
		sy = _spr->sprScaleY;
	}

	BOOL aa;
	GetSpriteScale(_rh->rh4.rh4Mv->mvIdEditWin, _spr, &sx, &sy, &aa);
}

float SpriteSupport::GetScaleX () {
	if (!_hwa)
		return _spr->sprScaleX;

	float sx, sy;
	GetScale(sx, sy);

	return sx;
}

float SpriteSupport::GetScaleY () {
	if (!_hwa)
		return _spr->sprScaleY;

	float sx, sy;
	GetScale(sx, sy);

	return sy;
}

sMask* SpriteSupport::GetMask () {
	return GetSpriteMask(_rh->rh4.rh4Mv->mvIdEditWin, _spr, _spr->sprImg, 0);
}

LPBYTE SpriteSupport::GetMaskPtr (sMask* mask) {
	return (LPBYTE)mask + sizeof(sMask);
}

LPBYTE SpriteSupport::GetMaskPtr () {
	return GetMaskPtr(GetMask());
}

LPHO SpriteSupport::GetHO () {
	if (!_hwa)
		return (LPHO)_spr->sprExtraInfo;

	return (LPHO)GetSpriteExtra(_rh->rh4.rh4Mv->mvIdEditWin, _spr);
}

LPLO SpriteSupport::GetLO () {
	if (!_hwa)
		return (LPLO)_spr->sprExtraInfo;

	return (LPLO)GetSpriteExtra(_rh->rh4.rh4Mv->mvIdEditWin, _spr);
}

// MaskTestPoint

int Collisions::MaskTestPoint (sMask* mask, int x, int y) {
	if (!mask)
		return 0;

	LPBYTE maskptr = SpriteSupport::GetMaskPtr(mask);

	//x = x - mask->mkRect.left;
	//y = y - mask->mkRect.top;

	if (Mask_TestPoint(maskptr, x, y, mask->mkWidth, mask->mkHeight)) {
		return 1;
	}

	return 0;
}

int Collisions::SpriteTestPoint (LPRH rh, fpSpr spr, int x, int y) {
	SpriteSupport ss(rh, spr);
	sMask* mask = ss.GetMask();

	x = x - ss.GetX();
	y = y - ss.GetY();

	return MaskTestPoint(mask, x, y);
}

// MaskTestRect

int Collisions::MaskTestRect (sMask* mask, int x, int y, int w, int h) {
	if (!mask)
		return 0;

	LPBYTE maskptr = SpriteSupport::GetMaskPtr(mask);

	//x = x - mask->mkRect.left;
	//y = y - mask->mkRect.top;

	if (Mask_TestRect(maskptr, mask->mkWidth, mask->mkHeight, x, y, w, h)) {
		return 1;
	}

	return 0;
}

int Collisions::SpriteTestRect (LPRH rh, fpSpr spr, int x, int y, int w, int h) {
	SpriteSupport ss(rh, spr);
	sMask* mask = ss.GetMask();

	x = x - ss.GetX();
	y = y - ss.GetY();

	return MaskTestRect(mask, x, y, w, h);
}

// MaskTestMask

int Collisions::MaskTestMask (sMask* mask1, sMask* mask2, int x1, int y1, int x2, int y2) {
	if (!mask1 || !mask2)
		return 0;

	LPBYTE maskptr1 = SpriteSupport::GetMaskPtr(mask1);
	LPBYTE maskptr2 = SpriteSupport::GetMaskPtr(mask2);

	//x1 = x1 + mask->mkRect.left;
	//y1 = y1 + mask->mkRect.top;

	if (Mask_TestMask(maskptr1, maskptr2,
		x1, y1, mask1->mkWidth, mask1->mkHeight,
		x2, y2, mask2->mkWidth, mask2->mkHeight)) {
		return 1;
	}

	return 0;
}

int Collisions::SpriteTestMask (LPRH rh, fpSpr spr1, sMask* mask2, int x2, int y2) {
	SpriteSupport ss(rh, spr1);
	sMask* mask1 = ss.GetMask();

	return MaskTestMask(mask1, mask2, ss.GetX(), ss.GetY(), x2, y2);
}

int Collisions::SpriteTestSprite (LPRH rh, fpSpr spr1, fpSpr spr2, int x2off, int y2off) {
	SpriteSupport ss1(rh, spr1);
	SpriteSupport ss2(rh, spr2);
	sMask* mask1 = ss1.GetMask();
	sMask* mask2 = ss2.GetMask();

	int x1 = ss1.GetX();
	int y1 = ss1.GetY();
	int x2 = ss2.GetX() + x2off;
	int y2 = ss2.GetY() + y2off;

	return MaskTestMask(mask1, mask2, x1, y1, x2, y2);
}

// Filters

void Collisions::SpritesToObjects (GenList& list, LPRH rh, int layer) {
	for (GenList::iterator it = list.begin(); it != list.end(); ) {
		npSpr spr = (npSpr)*it;
		SpriteSupport ss(rh, spr);

		// Get rid of backgrounds
		DWORD flags = ss.GetFlags();
		if (flags & SF_BACKGROUND) {
			it = list.erase(it);
			continue;
		}

		// Switch to objects
		LPHO ho = ss.GetHO();
		tagRCOM* com = GetRCOM(ho);

		if (!ho || !com || com->rcSprite != spr) {
			it = list.erase(it);
			continue;
		}
		else if (layer != LAYER_ALL && ho->hoLayer != layer) {
			it = list.erase(it);
			continue;
		}
		else {
			*it = ho;
			it++;
		}
	}
}

void Collisions::SpritesToBackdrops (GenList& list, LPRH rh, int layer) {
	for (GenList::iterator it = list.begin(); it != list.end(); ) {
		npSpr spr = (npSpr)*it;
		SpriteSupport ss(rh, spr);

		// Get rid of non-backgrounds
		DWORD flags = ss.GetFlags();
		if (!(flags & SF_BACKGROUND) || !(flags & SF_OBSTACLE)) {
			it = list.erase(it);
			continue;
		}

		// Switch to LOs
		LPLO lo = ss.GetLO();
		
		if (!lo) {
			it = list.erase(it);
			continue;
		}
		else if (layer != LAYER_ALL && lo->loLayer != layer) {
			it = list.erase(it);
			continue;
		}
		else {
			*it = lo;
			it++;
		}
	}
}

void Collisions::ObjectsMatchClass (GenList& list, LPOIL oi, int comp) {
	for (GenList::iterator it = list.begin(); it != list.end(); ) {
		LPHO ho = (LPHO)*it;
		
		if (comp == SAME_CLASS && ho->hoOi != oi->oilOi) {
			it = list.erase(it);
			continue;
		}
		else if (comp == DIFF_CLASS && ho->hoOi == oi->oilOi) {
			it = list.erase(it);
			continue;
		}
		else
			it++;
	}
}

// *ListByPoint

void Collisions::SpriteListByPoint (GenList& list, LPRH rh, int x, int y, int layer, int flag) {
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;
	npSpr spr = 0;

	x = x - rh->rh3.rh3DisplayX;
	y = y - rh->rh3.rh3DisplayY;

	while ((spr = SpriteCol_TestPoint(win, spr, layer, x, y, flag)) != 0) {
		list.push_back(spr);
	}
}

void Collisions::ObjectListByPoint (GenList& list, LPRH rh, int x, int y, int layer) {
	SpriteListByPoint(list, rh, x, y, layer, SCF_EVENNOCOL);
	SpritesToObjects(list, rh, layer);
}

void Collisions::BackdropListByPoint (GenList& list, LPRH rh, int x, int y, int layer) {
	SpriteListByPoint(list, rh, x, y, layer, SCF_BACKGROUND);
	SpritesToBackdrops(list, rh, layer);
}

// *ListByRect

void Collisions::SpriteListByRect (GenList& list, LPRH rh, int x, int y, int w, int h, int layer, int flag) {
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;
	npSpr spr = 0;

	x = x - rh->rh3.rh3DisplayX;
	y = y - rh->rh3.rh3DisplayY;

	while ((spr = SpriteCol_TestRect(win, spr, layer, x, y, w, h, flag)) != 0) {
		list.push_back(spr);
	}
}

void Collisions::ObjectListByRect (GenList& list, LPRH rh, int x, int y, int w, int h, int layer) {
	SpriteListByRect(list, rh, x, y, w, h, layer, SCF_EVENNOCOL);
	SpritesToObjects(list, rh, layer);
}

void Collisions::BackdropListByRect (GenList& list, LPRH rh, int x, int y, int w, int h, int layer) {
	SpriteListByRect(list, rh, x, y, w, h, layer, SCF_BACKGROUND);
	SpritesToBackdrops(list, rh, layer);
}

// *ListByMask

void Collisions::SpriteListByMask (GenList& list, LPRH rh, sMask* mask, int x, int y, int layer, int flag) {
	SpriteListByRect(list, rh, x, y, mask->mkWidth, mask->mkHeight, layer, flag);

	// Kill off all sprites that fail the mask test
	for (GenList::iterator it = list.begin(); it != list.end(); ) {
		npSpr spr = (npSpr)*it;

		if (!SpriteTestMask(rh, spr, mask, x, y)) {
			it = list.erase(it);
			continue;
		}
		else
			it++;
	}
}

// !! Try getting SpriteCol_TestSprite / SpriteCol_TestSprite_All working !!

struct ColRecord {
	npSpr	spr;
	LPHO	ho;
};

void Collisions::SpriteListBySprite (GenList& list, LPRH rh, fpSpr spr, int xoff, int yoff, int layer, int flag) {
	/*SpriteSupport ss(rh, spr);
	sMask* mask = ss.GetMask();

	int x = ss.GetX() + xoff;
	int y = ss.GetY() + yoff;

	SpriteListByMask(list, rh, mask, x, y, layer, flag);*/

	npWin win = rh->rh4.rh4Mv->mvIdEditWin;
	SpriteSupport ss(rh, spr);

	int x = ss.GetX() - rh->rh3.rh3DisplayX;
	int y = ss.GetY() - rh->rh3.rh3DisplayY;

	float scalex, scaley;
	ss.GetScale(scalex, scaley);

	int res = SpriteCol_TestSprite_All(win, spr, (LPVOID*)rh->rhBuffer1, spr->sprImg, x, y, ss.GetAngle(), scalex, scaley, flag); // != 0) {

	for (int i = 0; i < res; i++) {
		ColRecord* cr = ((ColRecord*) rh->rhBuffer1) + i;
		list.push_back(cr->spr);
	}
}

void Collisions::ObjectListByMask (GenList& list, LPRH rh, sMask* mask, int x, int y, int layer) {
	SpriteListByMask(list, rh, mask, x, y, layer, SCF_EVENNOCOL);
	SpritesToObjects(list, rh, layer);
}

void Collisions::ObjectListBySprite (GenList& list, LPRH rh, fpSpr spr, int xoff, int yoff, int layer) {
	SpriteListBySprite(list, rh, spr, xoff, yoff, layer, SCF_EVENNOCOL);
	SpritesToObjects(list, rh, layer);
}

void Collisions::BackdropListByMask (GenList& list, LPRH rh, sMask* mask, int x, int y, int layer) {
	SpriteListByMask(list, rh, mask, x, y, layer, SCF_BACKGROUND);
	SpritesToBackdrops(list, rh, layer);
}

void Collisions::BackdropListBySprite (GenList& list, LPRH rh, fpSpr spr, int xoff, int yoff, int layer) {
	SpriteListBySprite(list, rh, spr, xoff, yoff, layer, SCF_BACKGROUND);
	SpritesToBackdrops(list, rh, layer);
}

// Object List with Class filter

void Collisions::ObjectListByPoint(GenList& list, LPRH rh, LPOIL oi, int x, int y, int layer, int comp) {
	ObjectListByPoint(list, rh, x, y, layer);
	ObjectsMatchClass(list, oi, comp);
}

void Collisions::ObjectListByRect(GenList& list, LPRH rh, LPOIL oi, int x, int y, int w, int h, int layer, int comp) {
	ObjectListByRect(list, rh, x, y, w, h, layer);
	ObjectsMatchClass(list, oi, comp);
}

void Collisions::ObjectListByMask(GenList& list, LPRH rh, LPOIL oi, sMask* mask, int x, int y, int layer, int comp) {
	ObjectListByMask(list, rh, mask, x, y, layer);
	ObjectsMatchClass(list, oi, comp);
}

void Collisions::ObjectListBySprite(GenList& list, LPRH rh, LPOIL oi, npSpr spr, int xoff, int yoff, int layer, int comp) {
	ObjectListBySprite(list, rh, spr, xoff, yoff, layer);
	ObjectsMatchClass(list, oi, comp);
}

// Class Tests

int Collisions::ClassTestPoint (LPRH rh, LPOIL oi, int x, int y, int layer, int comp) {
	GenList list;
	ObjectListByPoint(list, rh, oi, x, y, layer, comp);

	return (list.size() > 0);
}

int Collisions::ClassTestRect (LPRH rh, LPOIL oi, int x, int y, int w, int h, int layer, int comp) {
	GenList list;
	ObjectListByRect(list, rh, oi, x, y, w, h, layer, comp);

	return (list.size() > 0);
}

int Collisions::ClassTestMask (LPRH rh, LPOIL oi, sMask* mask, int x, int y, int layer, int comp) {
	GenList list;
	ObjectListByMask(list, rh, oi, mask, x, y, layer, comp);

	return (list.size() > 0);
}

int Collisions::ClassTestSprite (LPRH rh, LPOIL oi, npSpr spr, int xoff, int yoff, int layer, int comp) {
	GenList list;
	ObjectListBySprite(list, rh, oi, spr, xoff, yoff, layer, comp);

	return (list.size() > 0);
}

// Class test class

/*void Collisions::ObjectListByClass (GenList& list, LPRH rh, LPOIL oi, int layer) {
	LPOBL objList = rh->rhObjectList;

	LPHO obj = objList[oi->oilObject].oblOffset;
	while (obj) {
		tagRCOM* com = GetRCOM(obj);
		if (!com || !com->rcSprite)
			continue;

		ObjectListBySprite(list, rh, com->rcSprite, 0, 0, layer);

		if (obj->hoNumNext < 0)
			break;

		obj = objList[obj->hoNumNext].oblOffset;
	}

	std::sort(list.begin(), list.end());
	std::unique(list.begin(), list.end());
}

void Collisions::ObjectListByClass (GenList& list, LPRH rh, LPOIL oi, LPOIL oi2, int layer, int comp) {
	ObjectListByClass(list, rh, oi, later);
	ObjectsMatchClass(list, oi2, comp);
}*/

// Backdrop

int Collisions::BackdropTestPoint (LPRH rh, int x, int y, int layer) {
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;

	int xadj = x - rh->rh3.rh3DisplayX;
	int yadj = y - rh->rh3.rh3DisplayY;

	if (layer == LAYER_ALL || layer == 0) {
		if (ColMask_TestPoint(win, xadj, yadj, 0))
			return 1;

		if (layer == 0)
			return 0;
	}

	GenList list;
	BackdropListByPoint(list, rh, x, y, layer);

	return (list.size() > 0);
}

int Collisions::BackdropTestRect (LPRH rh, int x, int y, int w, int h, int layer) {
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;

	int xadj = x - rh->rh3.rh3DisplayX;
	int yadj = y - rh->rh3.rh3DisplayY;

	if (layer == LAYER_ALL || layer == 0) {
		if (ColMask_TestRect(win, xadj, yadj, w, h, 0))
			return 1;

		if (layer == 0)
			return 0;
	}

	GenList list;
	BackdropListByRect(list, rh, x, y, w, h, layer);

	return (list.size() > 0);
}

int Collisions::BackdropTestSprite (LPRH rh, npSpr spr, int xoff, int yoff, int layer) {
	npWin win = rh->rh4.rh4Mv->mvIdEditWin;
	SpriteSupport ss(rh, spr);

	int xadj = ss.GetX() - rh->rh3.rh3DisplayX + xoff;
	int yadj = ss.GetY() - rh->rh3.rh3DisplayY + yoff;

	if (layer == LAYER_ALL || layer == 0) {
		float scalex, scaley;
		ss.GetScale(scalex, scaley);

		if (ColMask_TestSprite(win, spr, spr->sprImg, xadj, yadj, ss.GetAngle(), scalex, scaley, 0, 0))
			return 1;

		if (layer == 0)
			return 0;
	}

	GenList list;
	BackdropListBySprite(list, rh, spr, xoff, yoff, layer);

	return (list.size() > 0);
}


// -----

int Collisions::LayerCheck (lua_State* L, int index, int layer1, int layer2, int layer) {
	if (lua_isnumber(L, index))
		layer = lua_tointeger(L, index);
	if (layer > 0)
		layer -= 1;

	if (layer == ANY_LAYER)
		return 1;
	if (layer == SAME_LAYER && layer1 == layer2)
		return 1;
	if (layer == DIFF_LAYER && layer1 != layer2)
		return 1;
	if (layer == layer1)
		return 1;

	return 0;
}

int Collisions::LayerParam (lua_State* L, int index, int layer1, int layer) {
	if (lua_isnumber(L, index))
		layer = lua_tointeger(L, index);
	if (layer > 0)
		layer -= 1;

	if (layer == ANY_LAYER || layer == DIFF_LAYER || (layer == SAME_LAYER && layer1 == -1))
		return LAYER_ALL;
	if (layer == SAME_LAYER)
		return layer1;

	return layer;
}
