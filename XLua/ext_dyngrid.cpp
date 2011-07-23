#ifndef EXT_DYNGRID_H_
#define EXT_DYNGRID_H_



typedef struct tagRDATA
{
	#include "MagicRDATA.h"
	
	int				gridWidth;
	int				gridHeight;
	int				cellWidth;
	int				cellHeight;
	int				xOffset;
	int				yOffset;

	bool			useVLine;
	bool			useHLine;
	bool			useIntersect;
	bool			usePosSlope;
	bool			useNegSlope;

	int				type;
	COLORREF		lineColor;
	int				lineWeight;

	WORD			nImages;
	WORD			images[3];

	int				rebuild;
	cSurface*		pSf;
	
} RUNDATA;

#endif
