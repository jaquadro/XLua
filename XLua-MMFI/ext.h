#ifndef EXT_H_
#define EXT_H_

#include "common.h"

struct RDSurface {
	headerObject	rHo;
	rCom			rc;
	rMvt			rm;
	rSpr			rs;
	rVal			rv;
	RDSurface*		rRd;

	void*			surf;		// ImageBank*
	cSurface*		target;
	short			targetId;
	bool			targetValid;
	cSurface*		current;
	short			currentId;
	short			lastId;

	//Functions
	cSurface*		(*imageAt)(RDSurface*,int);
	int				(*imageCount)(RDSurface*);
};

#include "xsurface.h"

class BaseSurfaceRef;

struct RDXLuaSurface {
	headerObject	rHo;
	rCom			rc;
	rMvt			rm;
	rSpr			rs;
	rVal			rv;
	RDXLuaSurface*	rRd;
	
	int				width;
	int				height;

	//cSurface*		pSf;
	BaseSurfaceRef*	refSf;
	sMask*			pMask;

	int				hotX;
	int				hotY;

	// Transformed data
	cSurface*		transSf;
	sMask*			transMask;
};

#endif
