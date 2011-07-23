#ifndef TYPE_H_
#define TYPE_H_

#include "system.h"
#include "kcobject.h"
#include "extobject.h"

struct TypeRecord {
	DWORD			id;
	lua_CFunction	index;
	lua_CFunction	newIndex;
};

const TypeRecord typeList[] = {
	{ 'RTNC',	Counter::IndexMetamethod,			Counter::NewIndexMetamethod },
	{ 'TXET',	Text::IndexMetamethod,				Text::NewIndexMetamethod },
	{ 'ARR0',	KCOArray::IndexMetamethod,			KCOArray::NewIndexMetamethod },
	{ 'XLSF',	ExtXLuaSurface::IndexMetamethod,	ExtXLuaSurface::NewIndexMetamethod },
	{ 'SURF',	ExtSurface::IndexMetamethod,		ExtSurface::NewIndexMetamethod },

	{ 0, 0, 0 },
};

#endif