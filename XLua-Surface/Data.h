class rRundata;
typedef rRundata * LPRRDATA;

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// If you want to store anything between actions/conditions/expressions
// you should store it here

#include "dll_mmfi/surfaceref.h"

typedef struct tagRDATA
{
	#include "MagicRDATA.h"
	
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
	int				invalid;

	int				diffWidth;
	int				diffHeight;
	
} RUNDATA;
typedef	RUNDATA	* LPRDATA;

// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
// These values let you store data in your extension that will be saved in the MFA
// You should use these with properties

typedef struct tagEDATA_V1
{
	extHeader		eHeader;

	int				width;
	int				height;

} EDITDATA;
typedef EDITDATA * LPEDATA;
