// ============================================================================
//
// This file contains routines that are handled only during the Edittime,
// under the Frame and Event editors.
//
// Including creating, display, and setting up your object.
// 
// ============================================================================

#include "stdafx.h"
#include "common.h"
#include "EditSetup.h"

#include <cassert>

LPCSTR propModeList[] = {
	0,
	"Immediate",
	"Queued",
	NULL
};

// --------------------
// Properties
// --------------------

#define PSTR(s)	((int)s)

#define PROPID_FUNC_EXPORT PROPID_EXTITEM_CUSTOM_FIRST + 0x1000

enum { 
	PROPID_SETTINGS=PROPID_EXTITEM_CUSTOM_FIRST,

	PROPID_GROUP_STATE,
	PROPID_INIT_STATE,

	PROPID_GROUP_INTERFACE,
	PROPID_USE_MMF,
	PROPID_USE_WIN,

	PROPID_GROUP_QUEUE,
	PROPID_ERRQUEUE_MODE,
	PROPID_PRINTQUEUE_MODE,
	PROPID_USE_BT,

	PROPID_GROUP_SCRIPT,
	PROPID_SCRIPT,
	PROPID_RUNSCRIPT,
	PROPID_STD_LIBS,
	PROPID_AUTOREG,
};

PropData Properties[] = {
	PropData_Group(PROPID_GROUP_STATE, PSTR("Lua State"), PSTR("Lua State")),
	PropData_EditNumber_Check(PROPID_INIT_STATE, PSTR("Bind State"), PSTR("Automatically bind to state")),

	PropData_Group(PROPID_GROUP_INTERFACE, PSTR("Interfaces"), PSTR("Interfaces")),
	PropData_CheckBox(PROPID_USE_MMF, PSTR("Use MMF Interface"), PSTR("...")),
	PropData_CheckBox(PROPID_USE_WIN, PSTR("Use WIN Interface"), PSTR("...")),

	PropData_Group(PROPID_GROUP_QUEUE, PSTR("Standard I/O"), PSTR("Standard I/O")),
	PropData_ComboBox(PROPID_ERRQUEUE_MODE, PSTR("Error Events"), PSTR("Error event handling"), propModeList),
	PropData_ComboBox(PROPID_PRINTQUEUE_MODE, PSTR("Print Events"), PSTR("Print event handling"), propModeList),
	PropData_CheckBox(PROPID_USE_BT, PSTR("Report Backtraces"), PSTR("Include a backtrace of the call stack when reporting runtime errors")),

	PropData_Group(PROPID_GROUP_SCRIPT, PSTR("Script"), PSTR("Script")),
	PropData_Button(PROPID_SCRIPT, PSTR("Embedded Script"), PSTR("Embedded Lua scripts"), PSTR("Edit")),
	PropData_CheckBox(PROPID_STD_LIBS, PSTR("Load Standard Libs"), PSTR("Load standard libs at start of frame")),
	PropData_CheckBox(PROPID_AUTOREG, PSTR("Auto-Register Functions"), PSTR("Registar all MMF functions used in XLua conditions")),

	PropData_End()
};

PropData *funcExports = 0;

EDITDATA* UpdateEditRecords (LPMV mV, EDITDATA* edPtr, ScriptSet& set);

// --------------------
// GetProperties
// --------------------
// Inserts properties into the properties of the object.
//

BOOL WINAPI DLLExport GetProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	mvInsertProps(mV, edPtr, Properties, PROPID_TAB_GENERAL, TRUE);
#endif // !RUN_ONLY

	// OK
	return TRUE;
}

// --------------------
// ReleaseProperties
// --------------------
// Called when the properties are removed from the property window.
//

void WINAPI DLLExport ReleaseProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	// Write your code here
#endif // !RUN_ONLY
}

// --------------------
// GetPropCreateParam
// --------------------
// Called when a property is initialized and its creation parameter is NULL (in the PropData).
// Allows you, for example, to change the content of a combobox property according to specific settings in the EDITDATA structure.
//

LPARAM WINAPI DLLExport GetPropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
	return NULL;
}

// ----------------------
// ReleasePropCreateParam
// ----------------------
// Called after a property has been initialized.
// Allows you, for example, to free memory allocated in GetPropCreateParam.
//

void WINAPI DLLExport ReleasePropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID, LPARAM lParam)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
}

// --------------------
// GetPropValue
// --------------------
// Returns the value of properties that have a value.
// Note: see GetPropCheck for checkbox properties
//

LPVOID WINAPI DLLExport GetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY

	switch (nPropID) {

	case PROPID_INIT_STATE:
		return new CPropDWordValue(edPtr->initState);

	case PROPID_ERRQUEUE_MODE:
		return new CPropDWordValue(edPtr->errMode);

	case PROPID_PRINTQUEUE_MODE:
		return new CPropDWordValue(edPtr->printMode);
	}

#endif // !RUN_ONLY
	return NULL;
}

// --------------------
// GetPropCheck
// --------------------
// Returns the checked state of properties that have a check box.
//

BOOL WINAPI DLLExport GetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY

	switch (nPropID) {

	case PROPID_INIT_STATE:
		return edPtr->bindState;

	case PROPID_USE_MMF:
		return edPtr->useMMFI;

	case PROPID_USE_WIN:
		return edPtr->useWINI;

	case PROPID_AUTOREG:
		return edPtr->autoReg;

	case PROPID_STD_LIBS:
		return edPtr->loadLibs;

	case PROPID_USE_BT:
		return edPtr->useBacktrace;

	}

#endif // !RUN_ONLY
	return 0;		// Unchecked
}

// --------------------
// SetPropValue
// --------------------
// This routine is called by MMF after a property has been modified.
//

void WINAPI DLLExport SetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID, LPVOID lParam)
{
#ifndef RUN_ONLY
	// Gets the pointer to the CPropValue structure
	CPropValue* pValue = (CPropValue*)lParam;

	switch (nPropID) {

	case PROPID_INIT_STATE:
		edPtr->initState = ((CPropDWordValue*)pValue)->m_dwValue;
		break;

	case PROPID_ERRQUEUE_MODE:
		edPtr->errMode = ((CPropDWordValue*)pValue)->m_dwValue;
		break;

	case PROPID_PRINTQUEUE_MODE:
		edPtr->printMode = ((CPropDWordValue*)pValue)->m_dwValue;
		break;
	}

#endif // !RUN_ONLY
}

// --------------------
// SetPropCheck
// --------------------
// This routine is called by MMF when the user modifies a checkbox in the properties.
//

void WINAPI DLLExport SetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID, BOOL nCheck)
{
#ifndef RUN_ONLY

	switch (nPropID) {
	
	case PROPID_INIT_STATE:
		edPtr->bindState = (nCheck != 0);
		break;

	case PROPID_USE_MMF:
		edPtr->useMMFI = (nCheck != 0);
		break;

	case PROPID_USE_WIN:
		edPtr->useWINI = (nCheck != 0);
		break;

	case PROPID_AUTOREG:
		edPtr->autoReg = (nCheck != 0);
		break;

	case PROPID_STD_LIBS:
		edPtr->loadLibs = (nCheck != 0);
		break;

	case PROPID_USE_BT:
		edPtr->useBacktrace = (nCheck != 0);
		break;

	}
#endif // !RUN_ONLY
}

// --------------------
// EditProp
// --------------------
// This routine is called when the user clicks the button of a Button or EditButton property.
//

BOOL WINAPI DLLExport EditProp(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY

	if (nPropID == PROPID_SCRIPT) {
		return SetupDialog(mV, edPtr);
	}

#endif // !RUN_ONLY
	return FALSE;
}

// --------------------
// IsPropEnabled
// --------------------
// This routine returns the enabled state of a property.
//

BOOL WINAPI IsPropEnabled(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
	return TRUE;
}


// ============================================================================
//
// TEXT PROPERTIES
// 
// ============================================================================

// --------------------
// GetTextCaps
// --------------------
// Return the text capabilities of the object under the frame editor.
//

DWORD WINAPI DLLExport GetTextCaps(mv _far *mV, LPEDATA edPtr)
{
	return 0;	// (TEXT_ALIGN_LEFT|TEXT_ALIGN_HCENTER|TEXT_ALIGN_RIGHT|TEXT_ALIGN_TOP|TEXT_ALIGN_VCENTER|TEXT_ALIGN_BOTTOM|TEXT_FONT|TEXT_COLOR);
}

// --------------------
// GetTextFont
// --------------------
// Return the font used the object.
// Note: the pStyle and cbSize parameters are obsolete and passed for compatibility reasons only.
//

BOOL WINAPI DLLExport GetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPSTR pStyle, UINT cbSize)
{
#if !RUN_ONLY
#endif // !RUN_ONLY

	return TRUE;
}

// --------------------
// SetTextFont
// --------------------
// Change the font used the object.
// Note: the pStyle parameter is obsolete and passed for compatibility reasons only.
//

BOOL WINAPI DLLExport SetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPCSTR pStyle)
{
#if !RUN_ONLY
#endif // !RUN_ONLY

	return TRUE;
}

// --------------------
// GetTextClr
// --------------------
// Get the text color of the object.
//

COLORREF WINAPI DLLExport GetTextClr(mv _far *mV, LPEDATA edPtr)
{
	return 0;	// edPtr->fontColor;
}

// --------------------
// SetTextClr
// --------------------
// Set the text color of the object.
//

void WINAPI DLLExport SetTextClr(mv _far *mV, LPEDATA edPtr, COLORREF color)
{

}

// --------------------
// GetTextAlignment
// --------------------
// Get the text alignment of the object.
//

DWORD WINAPI DLLExport GetTextAlignment(mv _far *mV, LPEDATA edPtr)
{
	DWORD dw = 0;
#if !RUN_ONLY
#endif // !RUN_ONLY
	return dw;
}

// --------------------
// SetTextAlignment
// --------------------
// Set the text alignment of the object.
//

void WINAPI DLLExport SetTextAlignment(mv _far *mV, LPEDATA edPtr, DWORD dwAlignFlags)
{
#if !RUN_ONLY
#endif // !RUN_ONLY
}


// ============================================================================
//
// ROUTINES USED UNDER FRAME EDITOR
// 
// ============================================================================

// --------------------
// MakeIcon
// --------------------
// Called once object is created or modified, just after setup.
//
// Note: this function is optional. If it's not defined in your extension,
// MMF2 will load the EXO_ICON bitmap if it's defined in your resource file.
//
// If you need to draw the icon manually, remove the comments around this function and in the .def file.
//

/*
int WINAPI DLLExport MakeIconEx ( mv _far *mV, cSurface* pIconSf, LPSTR lpName, fpObjInfo oiPtr, LPEDATA edPtr )
{
	int error = -1;
#ifndef RUN_ONLY
	if ( pIconSf->LoadImage(hInstLib, EXO_ICON) != 0 )
		error = 0;
#endif // !RUN_ONLY
	return error;
}
*/

// --------------------
// CreateObject
// --------------------
// Called when you choose "Create new object". It should display the setup box 
// and initialize everything in the datazone.

int WINAPI DLLExport CreateObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY

	edPtr->initState = 0;
	edPtr->bindState = true;

	edPtr->useMMFI = false;
	edPtr->useWINI = false;
	edPtr->autoReg = true;

	edPtr->printMode = 1; // queued
	edPtr->errMode = 1; // queued
	edPtr->useBacktrace = false;

	edPtr->loadLibs = true;

	edPtr->tData = new TempData;
	edPtr->tData->serial = ScriptManager::serialNext++;
	edPtr->tData->scripts = 0;

	ScriptSet ss;

	ScriptData* sd = new ScriptData();
	sd->name = "Main";
	ss.scripts.push_back(sd);

	UpdateEditRecords(mV, edPtr, ss);

	return 0;	// No error

#endif // !RUN_ONLY

	// Error
	return -1;
}

// --------------------
// EditObject
// --------------------
// One of the option from the menu has been selected, and not a default menu option
// automatically handled by MMF2: this routine is then called.
//

BOOL WINAPI EditObject (mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY

	return SetupDialog(mV, edPtr);

#endif // !RUN_ONLY

	// Error
	return FALSE;
}

// --------------------
// SetEditSize
// --------------------
// Called by MMF2 when the object has been resized
//
// Note: remove the comments if your object can be resized (and remove the comments in the .def file)

/*
BOOL WINAPI SetEditSize(LPMV mv, LPEDATA edPtr, int cx, int cy)
{
#ifndef RUN_ONLY
	edPtr->swidth = cx;
	edPtr->sheight = cy;
#endif // !RUN_ONLY
	return TRUE;	// OK
}
*/

// --------------------
// PutObject
// --------------------
// Called when each individual object is dropped in the frame.
//

void WINAPI	DLLExport PutObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
}

// --------------------
// RemoveObject
// --------------------
// Called when each individual object is removed from the frame.
//

void WINAPI	DLLExport RemoveObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
	// Is the last object removed?
    if (0 == cpt)
	{
		// Do whatever necessary to remove our data
	}
#endif // !RUN_ONLY
}

// --------------------
// DuplicateObject
// --------------------
// Called when an object is created from another one (note: should be called CloneObject instead...)
//

void WINAPI DLLExport DuplicateObject(mv __far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
}

// --------------------
// GetObjectRect
// --------------------
// Returns the size of the rectangle of the object in the frame editor.
//

void WINAPI DLLExport GetObjectRect(mv _far *mV, RECT FAR *rc, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	rc->right = rc->left + 32;	// edPtr->swidth;
	rc->bottom = rc->top + 32;	// edPtr->sheight;
#endif // !RUN_ONLY
	return;
}


// --------------------
// EditorDisplay
// --------------------
// Displays the object under the frame editor
//
// Note: this function is optional. If it's not defined in your extension,
// MMF2 will load and display the EXO_IMAGE bitmap if it's defined in your resource file.
//
// If you need to draw the icon manually, remove the comments around this function and in the .def file.
//

/*
void WINAPI DLLExport EditorDisplay(mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr, RECT FAR *rc)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
}
*/

// --------------------
// IsTransparent
// --------------------
// This routine tells MMF2 if the mouse pointer is over a transparent zone of the object.
// 

extern "C" BOOL WINAPI DLLExport IsTransparent(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, int dx, int dy)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
	return FALSE;
}

// --------------------
// PrepareToWriteObject
// --------------------
// Just before writing the datazone when saving the application, MMF2 calls this routine.
// 

void WINAPI	DLLExport PrepareToWriteObject(mv _far *mV, LPEDATA edPtr, fpObjInfo adoi)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
}

// --------------------
// GetFilters
// --------------------
//

BOOL WINAPI GetFilters(LPMV mV, LPEDATA edPtr, DWORD dwFlags, LPVOID pReserved)
{
#ifndef RUN_ONLY
#endif // RUN_ONLY
	return FALSE;
}

// --------------------
// UsesFile
// --------------------
// Triggers when a file is dropped onto the frame
// Return TRUE if you can create an object from the given file
//

BOOL WINAPI	DLLExport UsesFile (LPMV mV, LPSTR fileName)
{
	BOOL r = FALSE;
#ifndef RUN_ONLY
#endif // !RUN_ONLY
	return r;
}


// --------------------
// CreateFromFile
// --------------------
// Creates a new object from file
//

void WINAPI	DLLExport CreateFromFile (LPMV mV, LPSTR fileName, LPEDATA edPtr)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
}



// ============================================================================
//
// ROUTINES USED UNDER EVENT / TIME / STEP-THROUGH EDITOR
// You should not need to change these routines
// 
// ============================================================================

// -----------------
// menucpy
// -----------------
// Internal routine used later, copy one menu onto another
// 
#ifndef RUN_ONLY
void menucpy(HMENU hTargetMenu, HMENU hSourceMenu)
{
	int			n, id, nMn;
	NPSTR		strBuf;
	HMENU		hSubMenu;

	nMn = GetMenuItemCount(hSourceMenu);
	strBuf = (NPSTR)LocalAlloc(LPTR, 80);
	for (n=0; n<nMn; n++)
	{
		if (0 == (id = GetMenuItemID(hSourceMenu, n)))
			AppendMenu(hTargetMenu, MF_SEPARATOR, 0, 0L);
		else
		{
			GetMenuString(hSourceMenu, n, strBuf, 80, MF_BYPOSITION);
			if (id != -1)
				AppendMenu(hTargetMenu, GetMenuState(hSourceMenu, n, MF_BYPOSITION), id, strBuf);
			else
			{
				hSubMenu = CreatePopupMenu();
				AppendMenu(hTargetMenu, MF_POPUP | MF_STRING, (uint)hSubMenu, strBuf);
				menucpy(hSubMenu, GetSubMenu(hSourceMenu, n));
			}
		}
	}
	LocalFree((HLOCAL)strBuf);
}

// -----------------
// GetPopupMenu
// -----------------
// Internal routine used later. Returns the first popup from a menu
// 
HMENU GetPopupMenu(short mn)
{
	HMENU	hMn, hSubMenu, hPopup = NULL;

	if ((hMn = LoadMenu(hInstLib, MAKEINTRESOURCE(mn))) != NULL)
	{
		if ((hSubMenu = GetSubMenu(hMn, 0)) != NULL)
		{
			if ((hPopup = CreatePopupMenu()) != NULL)
				menucpy(hPopup, hSubMenu);
		}
		DestroyMenu(hMn);
	}
	return hPopup;
}

// --------------------
// GetEventInformations
// --------------------
// Internal routine used later. Look for one event in one of the eventInfos array...
// No protection to go faster: you must properly enter the conditions/actions!
//

static LPEVENTINFOS2 GetEventInformations(LPEVENTINFOS2 eiPtr, short code)

{
	while(eiPtr->infos.code != code)
		eiPtr = EVINFO2_NEXT(eiPtr);
	
	return eiPtr;
}
#endif // !RUN_ONLY


// ----------------------------------------------------
// GetConditionMenu / GetActionMenu / GetExpressionMenu
// ----------------------------------------------------
// Load the condition/action/expression menu from the resource, eventually
// enable or disable some options, and returns it to MMF2.
//
HMENU WINAPI DLLExport GetConditionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		return GetPopupMenu(MN_CONDITIONS);
#endif // !defined(RUN_ONLY)
	return NULL;
}

HMENU WINAPI DLLExport GetActionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		return GetPopupMenu(MN_ACTIONS);
#endif // !defined(RUN_ONLY)
	return NULL;
}

HMENU WINAPI DLLExport GetExpressionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		return GetPopupMenu(MN_EXPRESSIONS);
#endif // !defined(RUN_ONLY)
	return NULL;
}

// -------------------------------------------------------
// GetConditionTitle / GetActionTitle / GetExpressionTitle
// -------------------------------------------------------
// Returns the title of the dialog box displayed when entering
// parameters for the condition, action or expressions, if any.
// Here, we simply return the title of the menu option
//

#ifndef RUN_ONLY
void GetCodeTitle(LPEVENTINFOS2 eiPtr, short code, short param, short mn, LPSTR strBuf, WORD maxLen)
{
	HMENU		hMn;

	// Finds event in array
	eiPtr=GetEventInformations(eiPtr, code);

	// If a special string is to be returned
	short strID = EVINFO2_PARAMTITLE(eiPtr, param);

	if ( strID != 0 )
		LoadString(hInstLib, strID, strBuf, maxLen);
	else
	{
		// Otherwise, returns the menu option 
		if ((hMn = LoadMenu(hInstLib, MAKEINTRESOURCE(mn))) != NULL )
		{
			GetMenuString(hMn, eiPtr->menu, strBuf, maxLen, MF_BYCOMMAND);
			DestroyMenu(hMn);
		}
	}
}
#else
#define GetCodeTitle(a,b,c,d,e,f)
#endif // !defined(RUN_ONLY)

void WINAPI DLLExport GetConditionTitle(mv _far *mV, short code, short param, LPSTR strBuf, short maxLen) {
	GetCodeTitle((LPEVENTINFOS2)conditionsInfos, code, param, MN_CONDITIONS, strBuf, maxLen);
}
void WINAPI DLLExport GetActionTitle(mv _far *mV, short code, short param, LPSTR strBuf, short maxLen) {
	GetCodeTitle((LPEVENTINFOS2)actionsInfos, code, param, MN_ACTIONS, strBuf, maxLen);
}
void WINAPI DLLExport GetExpressionTitle(mv _far *mV, short code, LPSTR strBuf, short maxLen) {
	GetCodeTitle((LPEVENTINFOS2)expressionsInfos, code, 0, MN_EXPRESSIONS, strBuf, maxLen);
}

// -------------------------------------------------------
// GetConditionTitle / GetActionTitle / GetExpressionTitle
// -------------------------------------------------------
// From a menu ID, these routines returns the code of the condition,
// action or expression, as defined in the .H file
//

short WINAPI DLLExport GetConditionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	LPEVENTINFOS2	eiPtr;
	int				n;

	for (n=CND_LAST, eiPtr=(LPEVENTINFOS2)conditionsInfos; n>0 && eiPtr->menu!=menuId; n--)
		eiPtr = EVINFO2_NEXT(eiPtr);
	if (n>0) 
		return eiPtr->infos.code;
#endif // !defined(RUN_ONLY)
	return -1;
}

short WINAPI DLLExport GetActionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	LPEVENTINFOS2	eiPtr;
	int				n;

	for (n=ACT_LAST, eiPtr=(LPEVENTINFOS2)actionsInfos; n>0 && eiPtr->menu!=menuId; n--)
		eiPtr = EVINFO2_NEXT(eiPtr);
	if (n>0) 
		return eiPtr->infos.code;
#endif // !defined(RUN_ONLY)
	return -1;
}

short WINAPI DLLExport GetExpressionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	LPEVENTINFOS2	eiPtr;
	int				n;

	for (n=EXP_LAST, eiPtr=(LPEVENTINFOS2)expressionsInfos; n>0 && eiPtr->menu!=menuId; n--)
		eiPtr = EVINFO2_NEXT(eiPtr);
	if (n>0) 
		return eiPtr->infos.code;
#endif // !defined(RUN_ONLY)
	return -1;
}

// -------------------------------------------------------
// GetConditionInfos / GetActionInfos / GetExpressionInfos
// -------------------------------------------------------
// From a action / condition / expression code, returns 
// an infosEvents structure. 
//

LPINFOEVENTSV2 WINAPI DLLExport GetConditionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY
	return &GetEventInformations((LPEVENTINFOS2)conditionsInfos, code)->infos;
#else
	return NULL;
#endif // !RUN_ONLY
}

LPINFOEVENTSV2 WINAPI DLLExport GetActionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY
	return &GetEventInformations((LPEVENTINFOS2)actionsInfos, code)->infos;
#else
	return NULL;
#endif // !RUN_ONLY
}

LPINFOEVENTSV2 WINAPI DLLExport GetExpressionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY
	return &GetEventInformations((LPEVENTINFOS2)expressionsInfos, code)->infos;
#else
	return NULL;
#endif // !RUN_ONLY
}


// ----------------------------------------------------------
// GetConditionString / GetActionString / GetExpressionString
// ----------------------------------------------------------
// From a action / condition / expression code, returns 
// the string to use for displaying it under the event editor
//

void WINAPI DLLExport GetConditionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		LoadString(hInstLib, GetEventInformations((LPEVENTINFOS2)conditionsInfos, code)->string, strPtr, maxLen);
#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetActionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		LoadString(hInstLib, GetEventInformations((LPEVENTINFOS2)actionsInfos, code)->string, strPtr, maxLen);
#endif // !defined(RUN_ONLY)
}

void WINAPI DLLExport GetExpressionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
		LoadString(hInstLib, GetEventInformations((LPEVENTINFOS2)expressionsInfos, code)->string, strPtr, maxLen);
#endif // !defined(RUN_ONLY)
}

// ----------------------------------------------------------
// GetExpressionParam
// ----------------------------------------------------------
// Returns the parameter name to display in the expression editor
//

void WINAPI DLLExport GetExpressionParam(mv _far *mV, short code, short param, LPSTR strBuf, short maxLen)
{
#if !defined(RUN_ONLY)
	short		strID;

	// Finds event in array
	LPEVENTINFOS2 eiPtr=GetEventInformations((LPEVENTINFOS2)expressionsInfos, code);

	// If a special string is to be returned
	strID = EVINFO2_PARAMTITLE(eiPtr, param);
	if ( strID != 0 )
		LoadString(hInstLib, strID, strBuf, maxLen);
	else
		*strBuf=0;
#endif // !defined(RUN_ONLY)
}

// ----------------------------------------------------------
// Custom Parameters
// ----------------------------------------------------------

// --------------------
// InitParameter
// --------------------
// Initialize the parameter.
//

void WINAPI InitParameter(mv _far *mV, short code, paramExt* pExt)
{
#if !RUN_ONLY
	// Example
	// -------
	// strcpy(&pExt->pextData[0], "Parameter Test");
	// pExt->pextSize = sizeof(paramExt) + strlen(pExt->pextData)+1;
#endif // !RUN_ONLY
}

// Example of custom parameter setup proc
// --------------------------------------
/*
#if !RUN_ONLY
BOOL CALLBACK DLLExport SetupProc(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	paramExt*			pExt;

	switch (msgType)
	{
		case WM_INITDIALOG: // Init dialog

			// Save edptr
			SetWindowLong(hDlg, DWL_USER, lParam);
			pExt=(paramExt*)lParam;

			SetDlgItemText(hDlg, IDC_EDIT, pExt->pextData);
			return TRUE;

		case WM_COMMAND: // Command

			// Retrieve edptr
			pExt = (paramExt *)GetWindowLong(hDlg, DWL_USER);

			switch (wmCommandID)
			{
			case IDOK:	// Exit
				GetDlgItemText(hDlg, IDC_EDIT, pExt->pextData, 500);
				pExt->pextSize=sizeof(paramExt)+strlen(pExt->pextData)+1;
				EndDialog(hDlg, TRUE);
				return TRUE;

				default:
					break;
			}
			break;

		default:
			break;
	}
	return FALSE;
}
#endif // !RUN_ONLY
*/

// --------------------
// EditParameter
// --------------------
// Edit the parameter.
//

void WINAPI EditParameter(mv _far *mV, short code, paramExt* pExt)
{
#if !RUN_ONLY

	// Example
	// -------
	// DialogBoxParam(hInstLib, MAKEINTRESOURCE(DB_TRYPARAM), mV->mvHEditWin, SetupProc, (LPARAM)(LPBYTE)pExt);

#endif // !RUN_ONLY
}

// --------------------
// GetParameterString
// --------------------
// Initialize the parameter.
//

void WINAPI GetParameterString(mv _far *mV, short code, paramExt* pExt, LPSTR pDest, short size)
{
#if !RUN_ONLY

	// Example
	// -------
	// wsprintf(pDest, "Super parameter %s", pExt->pextData);

#endif // !RUN_ONLY
}

// -----------------
// GetObjInfos
// -----------------
// Return object info
//
// Info displayed in the object's About properties
// Note: ObjComment is also displayed in the Quick Description box in the Insert Object dialog box
//

void WINAPI	DLLExport GetObjInfos (mv _far *mV, LPEDATA edPtr, LPSTR ObjName, LPSTR ObjAuthor, LPSTR ObjCopyright, LPSTR ObjComment, LPSTR ObjHttp)
{
#ifndef RUN_ONLY
	strcpy(ObjName,ObjectName);
	strcpy(ObjAuthor,ObjectAuthor);
	strcpy(ObjCopyright,ObjectCopyright);
	strcpy(ObjComment,ObjectComment);
	strcpy(ObjHttp,ObjectURL);
#endif
}

// -----------------
// GetHelpFileName
// -----------------
// Returns the help filename of the object.
//

LPCSTR WINAPI GetHelpFileName()
{
#ifndef RUN_ONLY
	return ObjectHelp;
#else
	return NULL;
#endif
}

// -----------------
// GetRegID
// -----------------
// Returns the registered ID of the object
//

int WINAPI GetRegID()
{
#ifndef RUN_ONLY
	return ObjectRegID;
#else
	return -2; //REGID_RUNTIME;
#endif
}

//
// Support Functions
//

EditRecord* EDITDATA::ScriptAtIndex (int index) {
	char* addr = (char*)this + this->scriptRecordOffset + (index * sizeof(EditRecord));
	return (EditRecord*)addr;
}

EditRecord* EDITDATA::LibAtIndex (int index) {
	char* addr = (char*)this + this->libRecordOffset + (index * sizeof(EditRecord));
	return (EditRecord*)addr;
}

char* EditRecord::Name (EDITDATA* edPtr) {
	char* addr = (char*)edPtr + this->nameOffset;
	//assert(addr[this->nameLen] == 0);
	return addr;
}

char* EditRecord::Src (EDITDATA* edPtr) {
	char* addr = (char*)edPtr + this->srcOffset;
	//assert(addr[this->srcLen] == 0);
	return addr;
}

EDITDATA* UpdateEditRecords (LPMV mV, EDITDATA* edPtr, ScriptSet& set) {
	int size = sizeof(EDITDATA);
	int numScripts = set.scripts.size();
	int numLibs = 0;

	int scriptOffset = sizeof(EDITDATA);
	int libOffset = scriptOffset + numScripts * sizeof(EditRecord);
	int strOffset = libOffset + numLibs * sizeof(EditRecord);

	EditRecord*		erList = new EditRecord[numScripts];
	const char**	nameList = new const char*[numScripts];
	const char**	srcList = new const char*[numScripts];

	int i = 0;
	int curOffset = strOffset;

	for (ScriptSet::IScript it = set.scripts.begin(); it != set.scripts.end(); it++, i++) {
		size += sizeof(EditRecord);

		ScriptData* sd = *it;

		nameList[i] = sd->name.data();
		erList[i].nameLen = sd->name.size();
		erList[i].nameOffset = curOffset;
		curOffset += erList[i].nameLen + 1;
		size += erList[i].nameLen + 1;

		srcList[i] = sd->script.data();
		erList[i].srcLen = sd->script.size();
		erList[i].srcOffset = curOffset;
		curOffset += erList[i].srcLen + 1;
		size += erList[i].srcLen + 1;

		erList[i].flags = 0x00 | (sd->loadAtStart & 0x01);
	}

	// Apply changes
	LPEDATA pNewPtr = (LPEDATA)mvReAllocEditData(mV, edPtr, size);
	if (pNewPtr == NULL)
		return edPtr;

	edPtr = pNewPtr;
	edPtr->numScripts = numScripts;
	edPtr->numLibs = numLibs;
	edPtr->scriptRecordOffset = scriptOffset;
	edPtr->libRecordOffset = libOffset;
	edPtr->stringOffset = strOffset;

	for (i = 0; i < numScripts; i++) {
		memcpy_s(edPtr->ScriptAtIndex(i), sizeof(EditRecord), &erList[i], sizeof(EditRecord));
		memcpy_s(erList[i].Name(edPtr), erList[i].nameLen + 1, nameList[i], erList[i].nameLen + 1);
		memcpy_s(erList[i].Src(edPtr), erList[i].srcLen + 1, srcList[i], erList[i].srcLen + 1);
	}

	// Cleanup
	delete[] erList;
	delete[] nameList;
	delete[] srcList;

	return edPtr;
}
