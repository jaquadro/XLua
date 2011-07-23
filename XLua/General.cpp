/// =====================================================================================
//
// The following routines are used internally by MMF, and should not need to be modified
// 
// =====================================================================================

#include "Common.h"
#include "Windows.h"
#include "EditSetup.h"

HINSTANCE hInstLib;

// ============================================================================
//
// LIBRARY ENTRY & QUIT POINTS
// 
// ============================================================================

// -----------------
// Entry points
// -----------------
// Usually you do not need to do any initialization here: it is preferable to
// do it in "Initialize" found in Edittime.cpp

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{

	/*conditionsInfos = getConditionInfos();
	actionsInfos = getActionInfos();
	expressionsInfos = getExpressionInfos();
	
	ConditionJumps = getConditions();
	ActionJumps = getActions();
	ExpressionJumps = getExpressions();*/
	
	switch (dwReason)
	{
		// DLL is attaching to the address space of the current process.
		case DLL_PROCESS_ATTACH:
			
			hInstLib = hDLL; // Store HINSTANCE
			break;

		// A new thread is being created in the current process.
		case DLL_THREAD_ATTACH:
			break;

		// A thread is exiting cleanly.
		case DLL_THREAD_DETACH:
			break;

		// The calling process is detaching the DLL from its address space.
	    case DLL_PROCESS_DETACH:
#ifndef RUN_ONLY
			Scintilla_ReleaseResources();
#endif
			break;
	}
	
	return TRUE;
}

// -----------------
// Initialize
// -----------------
// Where you want to do COLD-START initialization.
// Called when the extension is loaded into memory.
//
extern "C" int WINAPI DLLExport Initialize(mv _far *mV, int quiet)
{
	// No error
	return 0;
}

// -----------------
// Free
// -----------------
// Where you want to kill and initialized data opened in the above routine
// Called just before freeing the DLL.
// 
extern "C" int WINAPI DLLExport Free(mv _far *mV)
{
	// No error
	return 0;
}

// ============================================================================
//
// GENERAL INFO
// 
// ============================================================================

// -----------------
// Get Infos
// -----------------
// 
extern "C" 
{
	DWORD WINAPI DLLExport GetInfos(int info)
	{
		
		switch (info)
		{
			case KGI_VERSION:
				return EXT_VERSION2;
			case KGI_PLUGIN:
				return EXT_PLUGIN_VERSION1;
			case KGI_PRODUCT:
				return ForVersion;
			case KGI_BUILD:
				return MinimumBuild;
			default:
				return 0;
		}
	}
}

// ----------------------------------------------------------
// GetRunObjectInfos
// ----------------------------------------------------------
// Fills an information structure that tells MMF2 everything
// about the object, its actions, conditions and expressions
// 

short WINAPI DLLExport GetRunObjectInfos(mv _far *mV, fpKpxRunInfos infoPtr)
{
	infoPtr->conditions = (LPBYTE)ConditionJumps;
	infoPtr->actions = (LPBYTE)ActionJumps;
	infoPtr->expressions = (LPBYTE)ExpressionJumps;

	infoPtr->numOfConditions = CND_LAST; //Conditions.size();
	infoPtr->numOfActions = ACT_LAST; //Actions.size();
	infoPtr->numOfExpressions = EXP_LAST; //Expressions.size();

	infoPtr->editDataSize = sizeof(EDITDATA);
	
	//MagicFlags(infoPtr->editFlags);
	infoPtr->editFlags = OEFLAG_RUNBEFOREFADEIN | OEFLAG_WINDOWPROC | OEFLAG_NEVERKILL | OEFLAG_NEVERSLEEP;

	infoPtr->windowProcPriority = WINDOWPROC_PRIORITY;

	//MagicPrefs(infoPtr->editPrefs);
	infoPtr->editPrefs = 0;

	infoPtr->identifier = IDENTIFIER;
	infoPtr->version = XLUA_VERSION_CURRENT;
	
	return TRUE;
}

// ----------------------------------------------------------
// GetDependencies
// ----------------------------------------------------------
// Returns the name of the external modules that you wish MMF to include
// with stand-alone applications (these modules must be in the MMF
// Data\Runtime folder).
//

LPCSTR* WINAPI DLLExport GetDependencies()
{
	// Do some rSDK stuff
	#include "rGetDependencies.h"
	
	//LPCSTR szDep[] = {
	//	"MyDll.dll",
	//	NULL
	//};

	return NULL; // szDep;
}

// -----------------
// LoadObject
// -----------------
// Routine called for each object when the object is read from the MFA file (edit time)
// or from the CCN or EXE file (run time).
// You can load data here, reserve memory etc...
//
int	WINAPI DLLExport LoadObject(mv _far *mV, LPCSTR fileName, LPEDATA edPtr, int reserved)
{
#ifndef RUN_ONLY
	edPtr->tData = new TempData;
	edPtr->tData->serial = ScriptManager::serialNext++;
	edPtr->tData->scripts = 0;
#endif
	return 0;
}

// -----------------
// UnloadObject
// -----------------
// The counterpart of the above routine: called just before the object is
// deleted from the frame.
//
void WINAPI DLLExport UnloadObject(mv _far *mV, LPEDATA edPtr, int reserved)
{
#ifndef RUN_ONLY
	if (edPtr->tData) {
		delete edPtr->tData->scripts;
		delete edPtr->tData;
	}
	edPtr->tData = 0;
#endif
}

// --------------------
// UpdateEditStructure
// --------------------
// For you to update your object structure to newer versions
// Called at both edit time and run time
// 
HGLOBAL WINAPI DLLExport UpdateEditStructure(mv __far *mV, void __far * OldEdPtr)
{
	DWORD extVersion = ((tagEDATA_V0*) OldEdPtr)->eHeader.extVersion;
	if (extVersion == XLUA_VERSION_CURRENT) {
		return NULL;
	}

	DWORD hgSize = 0;
	HGLOBAL hgNew = NULL;

	char nullstr[1] = "";
	char* script = &nullstr[0];

	switch (extVersion) {
		case XLUA_VERSION_0:
			hgSize = sizeof(EDITDATA) + sizeof(EditRecord) + 8;
			break;

		case XLUA_VERSION_1:
			script = ((tagEDATA_V1*)OldEdPtr)->script;
			hgSize = sizeof(EDITDATA) + strlen(script) + sizeof(EditRecord) + 8;
			break;

		case XLUA_VERSION_2:
			script = ((tagEDATA_V2*)OldEdPtr)->script;
			hgSize = sizeof(EDITDATA) + strlen(script) + sizeof(EditRecord) + 8;
			break;

		case XLUA_VERSION_3:
			hgSize = sizeof(EDITDATA) + ((tagEDATA_V3*)OldEdPtr)->eHeader.extSize - sizeof(tagEDATA_V3);
			break;
	}

	if ((hgNew = GlobalAlloc(GPTR, hgSize)) == NULL) {
		return NULL;
	}

	LPEDATA newEdPtr = (LPEDATA) GlobalLock(hgNew);
	memcpy(newEdPtr, OldEdPtr, ((tagEDATA_V0*)OldEdPtr)->eHeader.extSize);
	newEdPtr->eHeader.extVersion = XLUA_VERSION_CURRENT;
	newEdPtr->eHeader.extSize = hgSize;

	if (extVersion < XLUA_VERSION_1) {
		newEdPtr->bindState = false;
		newEdPtr->initState = 0;
		newEdPtr->errMode = 1;
		newEdPtr->printMode = 1;
		newEdPtr->runScript = false;
		//newEdPtr->script[0] = 0;
		newEdPtr->loadLibs = false;
		newEdPtr->useMMFI = false;
		newEdPtr->useWINI = false;
	}

	if (extVersion < XLUA_VERSION_2) {
		newEdPtr->autoReg = false;
		newEdPtr->useBacktrace = false;
	}

	if (extVersion < XLUA_VERSION_3) {
		int oldSrcLen = strlen(script);

		newEdPtr->numScripts = 1;
		newEdPtr->numLibs = 0;
		newEdPtr->scriptRecordOffset = sizeof(EDITDATA);
		newEdPtr->libRecordOffset = newEdPtr->scriptRecordOffset;
		newEdPtr->stringOffset = newEdPtr->scriptRecordOffset + sizeof(EditRecord);

		EditRecord* er = newEdPtr->ScriptAtIndex(0);
		er->nameLen = 4;
		er->nameOffset = newEdPtr->stringOffset;
		er->srcLen = oldSrcLen;
		er->srcOffset = er->nameOffset + 5;
		er->flags = newEdPtr->runScript & 0x01;

		memcpy_s(er->Name(newEdPtr), 5, "Main", 5);
		memcpy_s(er->Src(newEdPtr), oldSrcLen + 1, script, oldSrcLen + 1);
	}

	if (extVersion == XLUA_VERSION_3) {
		int oldExtraLen = ((tagEDATA_V3*)OldEdPtr)->eHeader.extSize - sizeof(tagEDATA_V3);
		memcpy_s(newEdPtr + 1, oldExtraLen, ((tagEDATA_V3*)OldEdPtr) + 1, oldExtraLen);

		int ediff = sizeof(EDITDATA) - sizeof(tagEDATA_V3);
		newEdPtr->scriptRecordOffset += ediff;
		newEdPtr->libRecordOffset += ediff;
		newEdPtr->stringOffset += ediff;

		for (int i = 0; i < newEdPtr->numScripts; i++) {
			EditRecord* er = newEdPtr->ScriptAtIndex(i);
			er->nameOffset += ediff;
			er->srcOffset += ediff;
		}
	}

	if (extVersion < XLUA_VERSION_4) {
		newEdPtr->unused2 = 0;
		newEdPtr->unused3 = 0;
		newEdPtr->unused4 = 0;
	}

	GlobalUnlock(hgNew);
	return hgNew;
}

// --------------------
// UpdateFileNames
// --------------------
// If you store file names in your datazone, they have to be relocated when the
// application is moved to a different directory: this routine does it.
// Called at edit time and run time.
//
// Call lpfnUpdate to update your file pathname (refer to the documentation)
// 
void WINAPI DLLExport UpdateFileNames(mv _far *mV, LPSTR appName, LPEDATA edPtr, void (WINAPI * lpfnUpdate)(LPSTR, LPSTR))
{
}

// ---------------------
// EnumElts
// ---------------------
//
// Uncomment this function if you need to store an image in the image bank.
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.
//
/*
int WINAPI DLLExport EnumElts (mv __far *mV, LPEDATA edPtr, ENUMELTPROC enumProc, ENUMELTPROC undoProc, LPARAM lp1, LPARAM lp2)
{  
	int error = 0;

	// Replace wImgIdx with the name of the WORD variable you create within the edit structure
  
	// Enum images  
	if ( (error = enumProc(&edPtr->wImgIdx, IMG_TAB, lp1, lp2)) != 0 )
	{
		// Undo enum images      
		undoProc (&edPtr->wImgIdx, IMG_TAB, lp1, lp2);    
	}  

	return error;
}
*/

