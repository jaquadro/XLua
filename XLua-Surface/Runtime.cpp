// ============================================================================
//
// This file contains routines that are handled during the Runtime
// 
// ============================================================================

// Common Include
#include	"common.h"

// --------------------
// GetRunObjectDataSize
// --------------------
// Returns the size of the runtime datazone of the object
// 
short WINAPI DLLExport GetRunObjectDataSize(fprh rhPtr, LPEDATA edPtr)
{
	return(sizeof(RUNDATA));
}

void UpdateHotSpot (LPRDATA rdPtr) {
	int hx = rdPtr->hotX;
	int hy = rdPtr->hotY;

	if (rdPtr->rc.rcScaleX != 1)
		hx = (int)(hx * rdPtr->rc.rcScaleX);
	if (rdPtr->rc.rcScaleY != 1)
		hy = (int)(hy * rdPtr->rc.rcScaleY);

	if (rdPtr->rc.rcAngle != 0) {
		double rotx = cos(rdPtr->rc.rcAngle * PI / 180);
		double roty = sin(rdPtr->rc.rcAngle * PI / 180);

		int halfx = (int)(rdPtr->width * rdPtr->rc.rcScaleX) / 2;
		int halfy = (int)(rdPtr->height * rdPtr->rc.rcScaleY) / 2;
		int tx = (int)(((hx - halfx) * rotx + (hy - halfy) * roty));
		int ty = (int)(((hy - halfy) * rotx - (hx - halfx) * roty));

		hx = tx + rdPtr->transSf->GetWidth() / 2;
		hy = ty + rdPtr->transSf->GetHeight() / 2;
	}

	if (rdPtr->pMask) {
		rdPtr->pMask->mkXSpot = hx;
		rdPtr->pMask->mkYSpot = hy;
	}

	if (rdPtr->transMask) {
		rdPtr->transMask->mkXSpot = hx;
		rdPtr->transMask->mkYSpot = hy;
	}

	rdPtr->rHo.hoImgXSpot = hx;
	rdPtr->rHo.hoImgYSpot = hy;
}

void TransformSurface(LPRDATA rdPtr) {
	LPRH	rhPtr = rdPtr->rHo.hoAdRunHeader;
	mv*		mV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;

	if (!rdPtr->refSf || !rdPtr->refSf->valid())
		return;

	// Cleanup previous transformed data
	if (rdPtr->transSf) {
		DeleteSurface(rdPtr->transSf);
		if (rdPtr->transMask) {
			free(rdPtr->transMask);
			rdPtr->transMask = 0;
		}
		rdPtr->transSf = 0;
		rdPtr->diffWidth = 0;
		rdPtr->diffHeight = 0;
	}

	if (rdPtr->rc.rcScaleX != 1 || rdPtr->rc.rcScaleY != 1) {
		int qscale = (rdPtr->rs.rsFlags & RSFLAG_SCALE_RESAMPLE) ? 7 : 0;
		rdPtr->transSf = NewSurface();
		rdPtr->transSf->Create(rdPtr->width * rdPtr->rc.rcScaleX, rdPtr->height * rdPtr->rc.rcScaleY, rdPtr->refSf->getSurface());
		rdPtr->refSf->getSurface()->Stretch(*rdPtr->transSf, qscale);
	}

	if (rdPtr->rc.rcAngle != 0) {
		int qrot = (rdPtr->rs.rsFlags & RSFLAG_ROTATE_ANTIA) ? 7 : 0;
		COLORREF transColor = rdPtr->refSf->getSurface()->GetTransparentColor();
		if (rdPtr->transSf) {
			cSurface* temp = NewSurface();
			rdPtr->transSf->CreateRotatedSurface(*temp, (int) rdPtr->rc.rcAngle, qrot, transColor, 1);
			DeleteSurface(rdPtr->transSf);
			rdPtr->transSf = temp;
		}
		else {
			rdPtr->transSf = NewSurface();
			rdPtr->refSf->getSurface()->CreateRotatedSurface(*rdPtr->transSf, (int) rdPtr->rc.rcAngle, qrot, transColor, 1);
		}

		rdPtr->diffWidth = rdPtr->transSf->GetWidth() - (rdPtr->width * rdPtr->rc.rcScaleX);
		rdPtr->diffHeight = rdPtr->transSf->GetHeight() - (rdPtr->height * rdPtr->rc.rcScaleY);
	}

	// Transformed Mask
	if (rdPtr->transSf) {
		DWORD dwMaskSize = rdPtr->transSf->CreateMask(NULL, 0);
		if (dwMaskSize != 0)
		{
			rdPtr->transMask = (LPSMASK)calloc(dwMaskSize, 1);
			if (rdPtr->transMask != NULL)
			{
				rdPtr->transSf->CreateMask(rdPtr->transMask, 0);
			}
		}
	}

	rdPtr->rHo.hoImgWidth = (rdPtr->rc.rcScaleX * rdPtr->width) + rdPtr->diffWidth;
	rdPtr->rHo.hoImgHeight = (rdPtr->rc.rcScaleY * rdPtr->height) + rdPtr->diffHeight;
}

/*void LoadImageFile(LPRDATA rdPtr, LPSTR pFileName)
{
	LPRH	rhPtr = rdPtr->rHo.hoAdRunHeader;
	HANDLE	hf = INVALID_HANDLE_VALUE;

	do {	
		if ( pFileName == NULL || *pFileName == 0 )
			break;

		// Copy filename to temp buffer
		char fname[MAX_PATH];
		strcpy(fname, pFileName);

		// Get surface prototype
		LPSURFACE wSurf = WinGetSurface((int)rhPtr->rhIdEditWin);
		LPSURFACE proto;
		GetSurfacePrototype(&proto, (wSurf != NULL) ? wSurf->GetDepth() : 24, ST_MEMORYWITHDC, SD_DIB);
		if ( proto == NULL )
			break;

		// Ask MMF2 to open the file (opens external file and embedded files, and downloads files in Vitalize mode)
		DWORD dwSize;
		hf = rhPtr->rh4.rh4Mv->mvOpenHFile(fname, &dwSize, 0);
		if ( hf == INVALID_HANDLE_VALUE )
			break;

		// Create CInpuBufFile object associated with the file handle
		DWORD dwOff = File_GetPosition((HFILE)hf);
		CInputBufFile bf;
		if ( bf.Create((HFILE)hf, dwOff, dwSize) != 0 )
			break;

		// Create surface
		cSurface* psf = NewSurface();
		if ( psf == NULL )
			break;
		psf->Create(4, 4, proto);
		psf->Fill(RGB(0,0,0));
		if ( psf->GetDepth() == 8 )
			psf->SetPalette (*wSurf);

		// Load picture
		CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
		if ( ImportImageFromInputFile(pImgMgr, &bf, psf, NULL, IMPORT_IMAGE_USESURFACEDEPTH | IMPORT_IMAGE_USESURFACEPALETTE) )
		{
			// Copy filename if file successfully loaded
			//strcpy(rdPtr->filename, fname);
			//rdPtr->nWidth = rdPtr->rHo.hoImgWidth = psf->GetWidth();
			//rdPtr->nHeight = rdPtr->rHo.hoImgHeight = psf->GetHeight();

			// Replace picture surface
			if ( rdPtr->image != NULL )
				DeleteSurface(rdPtr->image);
			rdPtr->image = psf;

			// Free collision mask
			//FreeColMask(rdPtr);

			// Set transparent color
			if ( psf->GetTransparentColor() == RGB(0,0,0) )
			{
				COLORREF trspColor = rdPtr->dwTransColor;
				if ( (rdPtr->dwOptions & 1) != 0 )
					psf->GetPixel(0,0,trspColor);
				psf->SetTransparentColor(trspColor);
			}

			BuildSurface(rdPtr);
		}
		else
			DeleteSurface(psf);

	} while(FALSE);

	// If we're clearing the image...
	if ( pFileName == NULL || *pFileName == 0 ) {
		if (rdPtr->image) {
			DeleteSurface(rdPtr->image);
			rdPtr->image = 0;
		}
		if (rdPtr->subImage) {
			DeleteSurface(rdPtr->subImage);
			rdPtr->subImage = 0;
		}
	}

	// Close picture file (opened with mvOpenHFile)
	if ( hf != INVALID_HANDLE_VALUE )
		rhPtr->rh4.rh4Mv->mvCloseHFile(hf);
}*/


// ---------------
// CreateRunObject
// ---------------
// The routine where the object is actually created
// 
short WINAPI DLLExport CreateRunObject(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr)
{
	// Do some rSDK stuff
	#include "rCreateRunObject.h"
	
	/*
	   This routine runs when your object is created, as you might have guessed.
	   It is here that you must transfer any data you need in rdPtr from edPtr,
	   because after this has finished you cannot access it again!
	   Also, if you have anything to initialise (e.g. dynamic arrays, surface objects)
	   you should do it here, and free your resources in DestroyRunObject.
	   See Graphic_Object_Ex.txt for an example of what you may put here.
	*/

	rdPtr->rHo.hoX = cobPtr->cobX;
	rdPtr->rHo.hoY = cobPtr->cobY;

	rdPtr->refSf = 0;
	rdPtr->invalid = 0;

//	rdPtr->refSf = new SurfaceRef();
	//rdPtr->rHo.hoImgWidth = edPtr->cWidth;
	//rdPtr->rHo.hoImgHeight = edPtr->cHeight;

	//rdPtr->cxOrigin = edPtr->cxOrigin;
	//rdPtr->cyOrigin = edPtr->cyOrigin;
	//rdPtr->cHeight = edPtr->cHeight;
	//rdPtr->cWidth = edPtr->cWidth;

	//rdPtr->dwOptions = edPtr->dwOptions;
	//rdPtr->dwTransColor = edPtr->dwTransColor;
	
	//strncpy(rdPtr->filename, edPtr->filename, _MAX_PATH);
	//LoadImageFile(rdPtr, rdPtr->filename);
	//BuildSurface(rdPtr);

	// No errors
	return 0;
}


// ----------------
// DestroyRunObject
// ----------------
// Destroys the run-time object
// 
short WINAPI DLLExport DestroyRunObject(LPRDATA rdPtr, long fast)
{
/*
   When your object is destroyed (either with a Destroy action or at the end of
   the frame) this routine is called. You must free any resources you have allocated!
   See Graphic_Object_Ex.txt for an example of what you may put here.
*/
//	delete rdPtr->refSf;

	// No errors
	delete rdPtr->rRd;
	return 0;
}


// ----------------
// HandleRunObject
// ----------------
// Called (if you want) each loop, this routine makes the object live
// 
short WINAPI DLLExport HandleRunObject(LPRDATA rdPtr)
{
/*
   If your extension will draw to the MMF window you should first 
   check if anything about its display has changed :

       return rdPtr->roc.rcChanged?REFLAG_DISPLAY:0;

   You will also need to make sure you change this flag yourself 
   to 1 whenever you want to redraw your object
 
   If your extension won't draw to the window, but it still needs 
   to do something every MMF loop use :

        return 0;

   If you don't need to do something every loop, use :

        return REFLAG_ONESHOT;

   This doesn't mean this function can never run again. If you want MMF
   to handle your object again (causing this code to run) use this function:

        rdPtr->rRd->Rehandle();

   At the end of the loop this code will run
*/
	// Will not be called next loop	

	/*if (rdPtr->reload) {
		//LoadImageFile(rdPtr, rdPtr->filename);
		rdPtr->reload = false;
		rdPtr->rebuild = false;

		return REFLAG_DISPLAY;
	}
	else if (rdPtr->rebuild) {
		BuildSurface(rdPtr);
		rdPtr->rebuild = false;

		return REFLAG_DISPLAY;
	}*/

	if (rdPtr->invalid) {
		TransformSurface(rdPtr);
		rdPtr->rc.rcChanged = 1;
	}

	UpdateHotSpot(rdPtr);
	
	if (rdPtr->rc.rcChanged)
		return REFLAG_DISPLAY;
	else
		return 0;
}

// ----------------
// DisplayRunObject
// ----------------
// Draw the object in the application screen.
// 
short WINAPI DLLExport DisplayRunObject(LPRDATA rdPtr)
{
/*
   If you return REFLAG_DISPLAY in HandleRunObject this routine will run.
   See Graphic_Object_Ex.txt for an example of what you may put here.
*/
	// Ok
	return 0;
}

// -------------------
// GetRunObjectSurface
// -------------------
// Implement this function instead of DisplayRunObject if your extension
// supports ink effects and transitions. Note: you can support ink effects
// in DisplayRunObject too, but this is automatically done if you implement
// GetRunObjectSurface (MMF applies the ink effect to the transition).
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.

cSurface* WINAPI DLLExport GetRunObjectSurface(LPRDATA rdPtr)
{
	WinAddZone(rdPtr->rHo.hoAdRunHeader->rhIdEditWin, &rdPtr->rHo.hoRect);

	if (rdPtr->refSf && rdPtr->refSf->valid()) {
		if (rdPtr->transSf)
			return rdPtr->transSf;

		return rdPtr->refSf->getSurface();
	}

	return 0;
}


// -------------------------
// GetRunObjectCollisionMask
// -------------------------
// Implement this function if your extension supports fine collision mode (OEPREFS_FINECOLLISIONS),
// Or if it's a background object and you want Obstacle properties for this object.
//
// Should return NULL if the object is not transparent.
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.
//
/*
cSurface* WINAPI DLLExport GetRunObjectCollisionMask(LPRDATA rdPtr, LPARAM lParam)
{
	// Typical example for active objects
	// ----------------------------------
	// Opaque? collide with box
	if ( (rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) == 0 )	// Note: only if your object has the OEPREFS_INKEFFECTS option
		return NULL;

	// Transparent? Create mask
	LPSMASK pMask = rdPtr->m_pColMask;
	if ( pMask == NULL )
	{
		if ( rdPtr->m_pSurface != NULL )
		{
			DWORD dwMaskSize = rdPtr->m_pSurface->CreateMask(NULL, lParam);
			if ( dwMaskSize != 0 )
			{
				pMask = (LPSMASK)calloc(dwMaskSize, 1);
				if ( pMask != NULL )
				{
					rdPtr->m_pSurface->CreateMask(pMask, lParam);
					rdPtr->m_pColMask = pMask;
				}
			}
		}
	}

	// Note: for active objects, lParam is always the same.
	// For background objects (OEFLAG_BACKGROUND), lParam maybe be different if the user uses your object
	// as obstacle and as platform. In this case, you should store 2 collision masks
	// in your data: one if lParam is 0 and another one if lParam is different from 0.

	return pMask;
}
*/

// ----------------
// PauseRunObject
// ----------------
// Enters the pause mode
// 
short WINAPI DLLExport PauseRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}


// -----------------
// ContinueRunObject
// -----------------
// Quits the pause mode
//
short WINAPI DLLExport ContinueRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}


// ============================================================================
//
// START APP / END APP / START FRAME / END FRAME routines
// 
// ============================================================================

// -------------------
// StartApp
// -------------------
// Called when the application starts or restarts.
// Useful for storing global data
// 
void WINAPI DLLExport StartApp(mv _far *mV, CRunApp* pApp)
{
	// Example
	// -------
	// Delete global data (if restarts application)
//	CMyData* pData = (CMyData*)mV->mvGetExtUserData(pApp, hInstLib);
//	if ( pData != NULL )
//	{
//		delete pData;
//		mV->mvSetExtUserData(pApp, hInstLib, NULL);
//	}
}

// -------------------
// EndApp
// -------------------
// Called when the application ends.
// 
void WINAPI DLLExport EndApp(mv _far *mV, CRunApp* pApp)
{
	// Example
	// -------
	// Delete global data
//	CMyData* pData = (CMyData*)mV->mvGetExtUserData(pApp, hInstLib);
//	if ( pData != NULL )
//	{
//		delete pData;
//		mV->mvSetExtUserData(pApp, hInstLib, NULL);
//	}
}

// -------------------
// StartFrame
// -------------------
// Called when the frame starts or restarts.
// 
void WINAPI DLLExport StartFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{

}

// -------------------
// EndFrame
// -------------------
// Called when the frame ends.
// 
void WINAPI DLLExport EndFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{

}

// ============================================================================
//
// TEXT ROUTINES (if OEFLAG_TEXT)
// 
// ============================================================================

// -------------------
// GetRunObjectFont
// -------------------
// Return the font used by the object.
// 
/*

  // Note: do not forget to enable the functions in the .def file 
  // if you remove the comments below.

void WINAPI GetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf)
{
	// Example
	// -------
	// GetObject(rdPtr->m_hFont, sizeof(LOGFONT), pLf);
}

// -------------------
// SetRunObjectFont
// -------------------
// Change the font used by the object.
// 
void WINAPI SetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf, RECT* pRc)
{
	// Example
	// -------
//	HFONT hFont = CreateFontIndirect(pLf);
//	if ( hFont != NULL )
//	{
//		if (rdPtr->m_hFont!=0)
//			DeleteObject(rdPtr->m_hFont);
//		rdPtr->m_hFont = hFont;
//		SendMessage(rdPtr->m_hWnd, WM_SETFONT, (WPARAM)rdPtr->m_hFont, FALSE);
//	}

}

// ---------------------
// GetRunObjectTextColor
// ---------------------
// Return the text color of the object.
// 
COLORREF WINAPI GetRunObjectTextColor(LPRDATA rdPtr)
{
	// Example
	// -------
	return 0;	// rdPtr->m_dwColor;
}

// ---------------------
// SetRunObjectTextColor
// ---------------------
// Change the text color of the object.
// 
void WINAPI SetRunObjectTextColor(LPRDATA rdPtr, COLORREF rgb)
{
	// Example
	// -------
	rdPtr->m_dwColor = rgb;
	InvalidateRect(rdPtr->m_hWnd, NULL, TRUE);
}
*/


// ============================================================================
//
// DEBUGGER ROUTINES
// 
// ============================================================================

// -----------------
// GetDebugTree
// -----------------
// This routine returns the address of the debugger tree
//
LPWORD WINAPI DLLExport GetDebugTree(LPRDATA rdPtr)
{
#if !defined(RUN_ONLY)
	return DebugTree;
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}

// -----------------
// GetDebugItem
// -----------------
// This routine returns the text of a given item.
//
void WINAPI DLLExport GetDebugItem(LPSTR pBuffer, LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)

	// Example
	// -------
/*
	char temp[DB_BUFFERSIZE];

	switch (id)
	{
	case DB_CURRENTSTRING:
		LoadString(hInstLib, IDS_CURRENTSTRING, temp, DB_BUFFERSIZE);
		wsprintf(pBuffer, temp, rdPtr->text);
		break;
	case DB_CURRENTVALUE:
		LoadString(hInstLib, IDS_CURRENTVALUE, temp, DB_BUFFERSIZE);
		wsprintf(pBuffer, temp, rdPtr->value);
		break;
	case DB_CURRENTCHECK:
		LoadString(hInstLib, IDS_CURRENTCHECK, temp, DB_BUFFERSIZE);
		if (rdPtr->check)
			wsprintf(pBuffer, temp, "TRUE");
		else
			wsprintf(pBuffer, temp, "FALSE");
		break;
	case DB_CURRENTCOMBO:
		LoadString(hInstLib, IDS_CURRENTCOMBO, temp, DB_BUFFERSIZE);
		wsprintf(pBuffer, temp, rdPtr->combo);
		break;
	}
*/

#endif // !defined(RUN_ONLY)
}

// -----------------
// EditDebugItem
// -----------------
// This routine allows to edit editable items.
//
void WINAPI DLLExport EditDebugItem(LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)

	// Example
	// -------
/*
	switch (id)
	{
	case DB_CURRENTSTRING:
		{
			EditDebugInfo dbi;
			char buffer[256];

			dbi.pText=buffer;
			dbi.lText=TEXT_MAX;
			dbi.pTitle=NULL;

			strcpy(buffer, rdPtr->text);
			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITTEXT, 0, (LPARAM)&dbi);
			if (ret)
				strcpy(rdPtr->text, dbi.pText);
		}
		break;
	case DB_CURRENTVALUE:
		{
			EditDebugInfo dbi;

			dbi.value=rdPtr->value;
			dbi.pTitle=NULL;

			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITINT, 0, (LPARAM)&dbi);
			if (ret)
				rdPtr->value=dbi.value;
		}
		break;
	}
*/
#endif // !defined(RUN_ONLY)
}


