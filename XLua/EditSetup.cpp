#include "EditSetup.h"

#ifndef RUN_ONLY
#include "Platform.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include <cassert>

int ScriptManager::serialNext = 1;

// External declarations

EDITDATA* UpdateEditRecords (LPMV mV, EDITDATA* edPtr, ScriptSet& set);

/**
 * Entry and exit point into the XLua editor dialog.  Refreshes the ScriptManager
 * and commits the ScriptSet to the EDITDATA if updates are flagged.
 */

int SetupDialog (mv _far *mV, LPEDATA edPtr) {

	ScriptManager& sm = ScriptManager::Get();
	sm.Init();

	sm.edPtr = edPtr;
	sm.mV = mV;

	int retcode = DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_SETUP), mV->mvHEditWin, SetupDialogProc, (LPARAM)edPtr);

	if (edPtr->tData->scripts->updated) {
		UpdateEditRecords(mV, edPtr, *(edPtr->tData->scripts));
		edPtr->tData->scripts->updated = false;
		return true;
	}

	return false;
}

/**
 * Initializes Scintilla
 */

void ScriptManager::Init () {
	if (!sciInit) {
		Scintilla_RegisterClasses(hInstLib);
		sciInit = true;
	}
}

/**
 * Creates a new Scintilla control for the current dialog session and
 * populates the dialog with the ScriptSet.
 *
 * Called from WM_INITDIALOG message.
 */

void ScriptManager::InitDlg (HWND dlg) {
	HWND sci = CreateEditControl(dlg);
	ShowWindow(sci, SW_SHOW);
	SetFocus(sci);

	stec.SetWindow(reinterpret_cast<long>(sci));

	if (!edPtr->tData->scripts) {
		edPtr->tData->scripts = new ScriptSet(mV);
		edPtr->tData->scripts->Refresh(mV, edPtr);
	}

	InitEditor(sci);
	InitLists(dlg);

	ScriptManager& sm = ScriptManager::Get();
	ResizeSetup(dlg, sm.prefDlgWidth, sm.prefDlgHeight);
}

void ScriptManager::EndDlg (HWND dlg) {
	DestroyWindow(reinterpret_cast<HWND>(stec.GetWindow()));
	stec.SetWindow(0);
}

/**
 * Creates the physical Scintilla control.  All control parameters are encoded
 * directly into this function.
 */

HWND CreateEditControl (HWND hwndParent) {
	RECT rc;
	rc.left = 15;
	rc.top = 115;
	rc.right = rc.left + 370;
	rc.bottom = rc.top + 150;

	// Transform coordinates
	MapDialogRect(hwndParent, &rc);

	HWND sci;
	sci = CreateWindowEx(WS_EX_CLIENTEDGE, "Scintilla", "", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hwndParent, 0, hInstLib, 0);

	if (!sci) {
		char msg[256];
		sprintf(msg, "Could not create the editor control: error code %x", GetLastError());
		MessageBox(0, msg, "Editor Error", MB_OK | MB_ICONWARNING);
	}

	return sci;
}

/**
 * Reads EditRecord structures out of the EDITDATA and adds them to
 * the current ScriptSet.  Any old scripts are purged.
 */

void ScriptSet::Refresh (LPMV mV, LPEDATA edPtr) {

	// Clean up old script stuff
	for (IScript it = scripts.begin(); it != scripts.end(); it++) {
		delete (*it);
	}

	scripts.clear();

	// Create new scripts from editdata
	for (int i = 0; i < edPtr->numScripts; i++) {
		EditRecord* er = edPtr->ScriptAtIndex(i);
		
		AddScript(er->Name(edPtr), er->Src(edPtr));

		scripts.back()->loadAtStart = er->flags & 0x01;
	}
}

/**
 * Adds a new script to the ScriptSet.
 * Creates a new Scintilla document pointer to store the unsaved script
 * and undo stack.
 */

ScriptData* ScriptSet::AddScript (std::string name, std::string src) {
	RemScript(name);

	ScriptManager& sm = ScriptManager::Get();

	ScriptData* sd = new ScriptData();
	sd->name = name;
	sd->script = src;

	sd->sciCom = &sm.stec;
	sd->sciDoc = (void*)sm.stec.SendEditor(SCI_CREATEDOCUMENT);

	// Configure the new Scintilla document
	sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, (sptr_t)sd->sciDoc);
	sm.stec.SendEditor(SCI_SETTEXT, 0, (LPARAM)sd->script.c_str());
	sm.stec.SendEditor(SCI_EMPTYUNDOBUFFER);
	sm.stec.SendEditor(SCI_SETSAVEPOINT);

	InitEditor(reinterpret_cast<HWND>(sm.stec.GetWindow()));

	// Restore the previous Scintilla document to the editor
	if (curScript)
		sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, (sptr_t)curScript->sciDoc);
	else
		sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, 0);

	scripts.push_back(sd);

	return sd;
}

/**
 * Delete a script from the ScriptSet.  The SD's destructor will take care
 * of freeing its SciDoc reference.
 */

void ScriptSet::RemScript (std::string name) {
	ScriptManager& sm = ScriptManager::Get();

	for (IScript it = scripts.begin(); it != scripts.end(); it++) {
		if ((*it)->name.compare(name) != 0)
			continue;

		if (curScript && (*it)->sciDoc == curScript->sciDoc) {
			sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, 0);
			curScript = 0;
		}

		delete (*it);

		scripts.erase(it);
		break;
	}
}

/**
 * Changes the name of a script.  If there is a name collision, the existing
 * script will be deleted.
 */

ScriptData* ScriptSet::RenameScript (std::string name, std::string newname) {
	bool found = false;
	ScriptData* sd = 0;

	// Search for script that we want to rename
	for (IScript it = scripts.begin(); it != scripts.end(); it++) {
		if ((*it)->name.compare(name) == 0) {
			found = true;
			sd = *it;
			break;
		}
	}

	if (!found)
		return 0;

	// Remove script identified by the new name, if it exists
	RemScript(newname);

	// Update the script
	sd->name = newname;
	updated = true;

	return sd;
}

const char luaKeywords[] =
	"and break do else elseif end false for function if "
	"in local nil not or repeat return then true until while";

const char luaLibFuncs[] =
	"_VERSION assert collectgarbage dofile error gcinfo loadfile loadstring "
	"print rawget rawset require tonumber tostring type unpack _G getfenv "
	"getmetatable ipairs loadlib next pairs pcall rawequal setfenv "
	"setmetatable xpcall string table math coroutine io os debug "
	"load module select"

	"string.byte string.char string.dump string.find string.len "
	"string.lower string.rep string.sub string.upper string.format string.gfind string.gsub "
	"table.concat table.foreach table.foreachi table.getn table.sort table.insert table.remove table.setn "
	"math.abs math.acos math.asin math.atan math.atan2 math.ceil math.cos math.deg math.exp "
	"math.floor math.frexp math.ldexp math.log math.log10 math.max math.min math.mod "
	"math.pi math.pow math.rad math.random math.randomseed math.sin math.sqrt math.tan "
	"string.gmatch string.match string.reverse table.maxn "
	"math.cosh math.fmod math.modf math.sinh math.tanh math.huge"

	"coroutine.create coroutine.resume coroutine.status "
	"coroutine.wrap coroutine.yield "
	"io.close io.flush io.input io.lines io.open io.output io.read io.tmpfile io.type io.write "
	"io.stdin io.stdout io.stderr "
	"os.clock os.date os.difftime os.execute os.exit os.getenv os.remove os.rename "
	"os.setlocale os.time os.tmpname "
	"coroutine.running package.cpath package.loaded package.loadlib package.path "
	"package.preload package.seeall io.popen"
	
	"bit bit.band bit.bor bit.tobit bit.tohex bit.bnot bit.bxor bit.lshift "
	"bit.rshift bit.arshift bit.rol bit.ror bit.swap";

void InitEditor (HWND hwnd) {
	SendMessage(hwnd, SCI_SETLEXER, SCLEX_LUA, 0);
	SendMessage(hwnd, SCI_SETSTYLEBITS, 7, 0);

	// Folding
	SendMessage(hwnd, SCI_SETPROPERTY, (WPARAM)"fold", (LPARAM)"1");
	SendMessage(hwnd, SCI_SETPROPERTY, (WPARAM)"fold.compact", (LPARAM)"0");

	SendMessage(hwnd, SCI_SETFOLDFLAGS, SC_FOLDFLAG_LINEAFTER_CONTRACTED, 0);

	// Margins
	SendMessage(hwnd, SCI_SETMARGINWIDTHN, 0, 40);
	SendMessage(hwnd, SCI_SETMARGINWIDTHN, 1, 0);
	SendMessage(hwnd, SCI_SETMARGINWIDTHN, 2, 18);

	SendMessage(hwnd, SCI_SETMARGINTYPEN, 2, SC_MARGIN_SYMBOL);
	SendMessage(hwnd, SCI_SETMARGINMASKN, 2, SC_MASK_FOLDERS);

	SendMessage(hwnd, SCI_SETMARGINSENSITIVEN, 2, 1);

	// Tabs & Indent
	SendMessage(hwnd, SCI_SETTABWIDTH, 4, 0);
	SendMessage(hwnd, SCI_SETINDENT, 4, 0);

	// Folding symbols
	SendMessage(hwnd, SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);
	SendMessage(hwnd, SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
	SendMessage(hwnd, SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
	SendMessage(hwnd, SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER);
	SendMessage(hwnd, SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED);
	SendMessage(hwnd, SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
	SendMessage(hwnd, SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);

	SendMessage(hwnd, SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPEN, 0xFFFFFF);
	SendMessage(hwnd, SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPEN, 0);
	SendMessage(hwnd, SCI_MARKERSETFORE, SC_MARKNUM_FOLDER, 0xFFFFFF);
	SendMessage(hwnd, SCI_MARKERSETBACK, SC_MARKNUM_FOLDER, 0);
	SendMessage(hwnd, SCI_MARKERSETFORE, SC_MARKNUM_FOLDERSUB, 0xFFFFFF);
	SendMessage(hwnd, SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0);
	SendMessage(hwnd, SCI_MARKERSETFORE, SC_MARKNUM_FOLDERTAIL, 0xFFFFFF);
	SendMessage(hwnd, SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0);
	SendMessage(hwnd, SCI_MARKERSETFORE, SC_MARKNUM_FOLDEREND, 0xFFFFFF);
	SendMessage(hwnd, SCI_MARKERSETBACK, SC_MARKNUM_FOLDEREND, 0);
	SendMessage(hwnd, SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPENMID, 0xFFFFFF);
	SendMessage(hwnd, SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPENMID, 0);
	SendMessage(hwnd, SCI_MARKERSETFORE, SC_MARKNUM_FOLDERMIDTAIL, 0xFFFFFF);
	SendMessage(hwnd, SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0);

	// Load keywords
	SendMessage(hwnd, SCI_SETKEYWORDS, 0, (LPARAM)luaKeywords);
	SendMessage(hwnd, SCI_SETKEYWORDS, 1, (LPARAM)luaLibFuncs);

	// Default font
	SendMessage(hwnd, SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Courier New");
	SendMessage(hwnd, SCI_STYLESETSIZE, STYLE_DEFAULT, 10);
	SendMessage(hwnd, SCI_STYLESETFONT, SCE_LUA_DEFAULT, (LPARAM)"Courier New");
	SendMessage(hwnd, SCI_STYLESETSIZE, SCE_LUA_DEFAULT, 10);
	SendMessage(hwnd, SCI_STYLESETBOLD, SCE_LUA_DEFAULT, (LPARAM)false);

	// Reserved words
	SendMessage(hwnd, SCI_STYLESETFORE, SCE_LUA_WORD, RGB(0, 0, 192));
	SendMessage(hwnd, SCI_STYLESETBOLD, SCE_LUA_WORD, (LPARAM)true);

	// Library Functions
	SendMessage(hwnd, SCI_STYLESETFORE, SCE_LUA_WORD2, RGB(128, 0, 128));

	// Comments
	SendMessage(hwnd, SCI_STYLESETFORE, SCE_LUA_COMMENTLINE, RGB(0, 128, 0));
	SendMessage(hwnd, SCI_STYLESETITALIC, SCE_LUA_COMMENTLINE, (LPARAM)true);

	SendMessage(hwnd, SCI_STYLESETFORE, SCE_LUA_COMMENT, RGB(0, 128, 0));
	SendMessage(hwnd, SCI_STYLESETBACK, SCE_LUA_COMMENT, RGB(230, 230, 230));
	SendMessage(hwnd, SCI_STYLESETEOLFILLED, SCE_LUA_COMMENT, (LPARAM)true);

	// Strings
	SendMessage(hwnd, SCI_STYLESETFORE, SCE_LUA_STRING, RGB(192, 0, 0));

	// Ops
	SendMessage(hwnd, SCI_STYLESETFORE, SCE_LUA_OPERATOR, RGB(0, 128, 128));

	// Numbers
	SendMessage(hwnd, SCI_STYLESETFORE, SCE_LUA_NUMBER, RGB(192, 128, 0));

	// Braces
	SendMessage(hwnd, SCI_STYLESETFORE, STYLE_BRACELIGHT, RGB(0, 128, 128));

	SendMessage(hwnd, SCI_STYLESETFORE, STYLE_BRACEBAD, RGB(255, 255, 255));
	SendMessage(hwnd, SCI_STYLESETBACK, STYLE_BRACEBAD, RGB(192, 0, 0));
}

/**
 * Load all scripts in current ScriptSet into the dialog's script listbox,
 * select first script.
 */

void InitLists (HWND hwnd) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;
	if (!ss)
		return;

	// Insert names of all scripts
	for (ScriptSet::IScript it = ss->scripts.begin(); it != ss->scripts.end(); it++) {
		SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_ADDSTRING, 0, (LPARAM)(*it)->name.c_str());
	}

	// Select first item, update dialog state accordingly
	SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_SETCURSEL, 0, 0);
	ScriptSelChange(hwnd);
}

/**
 * Loads the SciDoc for the currently selected script into the editor,
 * updates the rest of the dialog state.
 */

void ScriptSelChange (HWND hwnd) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;
	if (!ss)
		return;

	int sel = SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_GETCURSEL, 0, 0);
	
	// If no selection... 
	if (sel == LB_ERR) {
		sm.stec.SendEditor(SCI_SETDOCPOINTER);
		sm.stec.SendEditor(SCI_SETREADONLY, 1);

		EnableWindow(reinterpret_cast<HWND>(sm.stec.GetWindow()), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_AUTORUN), FALSE);

		ss->curScript = 0;
		return;
	}

	// Otherwise, re-enable various parts of the dialog
	EnableWindow(reinterpret_cast<HWND>(sm.stec.GetWindow()), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_AUTORUN), TRUE);

	sm.stec.SendEditor(SCI_SETREADONLY, 0);

	// Find out what's selected, load the corresponding SciDoc
	int slen = SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_GETTEXTLEN, sel, 0);
	char* buf = new char[slen + 1];

	SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_GETTEXT, sel, (LPARAM)buf);

	for (ScriptSet::IScript it = ss->scripts.begin(); it != ss->scripts.end(); it++) {
		if ((*it)->name.compare(buf) == 0) {
			ss->curScript = *it;

			// Load document into editor
			sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, (sptr_t)(*it)->sciDoc);

			//ResizeSetup(hwnd, sm.prefDlgWidth, sm.prefDlgHeight);

			// Update auto-run checkbox state
			if ((*it)->loadAtStart)
				SendDlgItemMessage(hwnd, IDC_CHECKBOX_AUTORUN, BM_SETCHECK, BST_CHECKED, 0);
			else
				SendDlgItemMessage(hwnd, IDC_CHECKBOX_AUTORUN, BM_SETCHECK, BST_UNCHECKED, 0);

			break;
		}
	}

	delete[] buf;
}

/**
 * Delete the currently selected script from the editor and ScriptSet
 */

void ScriptDelete (HWND hwnd) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;
	if (!ss)
		return;

	// Is anything selected for deletion?
	int sel = SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_GETCURSEL, 0, 0);
	if (sel == LB_ERR)
		return;
	
	// Find out what's selected, delete it
	int slen = SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_GETTEXTLEN, sel, 0);
	char* buf = new char[slen + 1];

	SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_GETTEXT, sel, (LPARAM)buf);

	for (ScriptSet::IScript it = ss->scripts.begin(); it != ss->scripts.end(); it++) {
		if ((*it)->name.compare(buf) == 0) {
			ss->RemScript(buf);

			// We just deleted our current script!
			ss->curScript = 0;

			SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_DELETESTRING, sel, 0);
			ScriptSelChange(hwnd);
			break;
		}
	}

	delete[] buf;
}

/**
 * Add a new script to the ScriptSet and point the editor to it
 */

void ScriptAdd (HWND hwnd) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;
	if (!ss)
		return;

	// Make sure we actually "OK"ed the action
	int retcode = DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_NEW), hwnd, NameDialogProc, (LPARAM)"Add Script");
	if (retcode != IDOK)
		return;

	// Ferret out duplicate names
	for (ScriptSet::IScript it = ss->scripts.begin(); it != ss->scripts.end(); it++) {
		if ((*it)->name.compare(sm.tempName) == 0) {
			MessageBox(hwnd, "A script with this name already exists.", "Duplicate Name", MB_ICONWARNING);
			return;
		}
	}

	// Start working on our newly added script
	ss->curScript = ss->AddScript(sm.tempName);
	ScriptSelChange(hwnd);

	SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_ADDSTRING, 0, (LPARAM)sm.tempName.c_str());
}

/**
 * Rename a script.  
 * Something conceptually simple that consumes too many lines of code.
 */

void ScriptRename (HWND hwnd) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;

	// We can only rename a script if it's selected
	int sel = SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_GETCURSEL, 0, 0);
	if (sel == LB_ERR)
		return;

	// Find out what's selected
	ScriptData* sd = 0;
	int slen = SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_GETTEXTLEN, sel, 0);
	char* buf = new char[slen + 1];

	SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_GETTEXT, sel, (LPARAM)buf);

	for (ScriptSet::IScript it = ss->scripts.begin(); it != ss->scripts.end(); it++) {
		if ((*it)->name.compare(buf) == 0) {
			sd = (*it);
			break;
		}
	}

	delete[] buf;

	// This should never happen
	if (!sd)
		return;

	// Did we decide to follow through?
	int retcode = DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_NEW), hwnd, NameDialogProc, (LPARAM)"Rename Script");
	if (retcode != IDOK)
		return;

	// Ferret out duplicate names
	for (ScriptSet::IScript it = ss->scripts.begin(); it != ss->scripts.end(); it++) {
		if ((*it)->name.compare(sm.tempName) == 0 && (*it) != sd) {
			MessageBox(hwnd, "A script with this name already exists.", "Duplicate Name", MB_ICONWARNING);
			return;
		}
	}

	// Update our name
	sd->name = sm.tempName;

	SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_DELETESTRING, sel, 0);
	SendDlgItemMessage(hwnd, IDC_LIST_SCRIPT, LB_ADDSTRING, 0, (LPARAM)sm.tempName.c_str());
}

void CaptureName (HWND hwnd) {
	ScriptManager& sm = ScriptManager::Get();

	int len = SendDlgItemMessage(hwnd, IDC_EDIT_NEWNAME, EM_LINELENGTH, 0, 0);
	
	// Setup destination buffer to accept a string from the input box.
	char* buf = new char[len + 1];
	((int*)buf)[0] = len + 1;

	// Grab name and store it to a temporary place in the ScriptManager
	SendDlgItemMessage(hwnd, IDC_EDIT_NEWNAME, EM_GETLINE, 0, (LPARAM)buf);
	sm.tempName = buf;

	delete[] buf;
}

/**
 * This is called whenever the "OK" or "Apply" button is pressed.  For every script
 * in the ScriptSet, the script data is copied from the SciDoc to the corresponding
 * ScriptData object.  A new save point is made, which is where the SciDoc will roll
 * back to if the "Cancel" button is pressed in the future.
 *
 * This function flags the ScriptSet to be committed to EDITDATA once the dialog
 * is terminated.
 */

void EditSave (HWND hwnd) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;
	if (!ss)
		return;

	// Go through all the scripts.  We must point the Scintilla control to a 
	// document before working on it.
	for (ScriptSet::IScript is = ss->scripts.begin(); is != ss->scripts.end(); is++) {
		sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, (sptr_t)(*is)->sciDoc);

		// Get text and update save point.
		int len = sm.stec.SendEditor(SCI_GETLENGTH);
		char* buf = new char[len + 1];
		sm.stec.SendEditor(SCI_GETTEXT, len + 1, (LPARAM)buf);
		sm.stec.SendEditor(SCI_SETSAVEPOINT);

		// Save text and flag future commit.
		(*is)->script = buf;
		ss->updated = true;

		delete[] buf;
	}

	// Restore previously selected document to the Scintilla control.
	if (ss->curScript)
		sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, (sptr_t)ss->curScript->sciDoc);
	else
		sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, 0);
}

/**
 * This is called whenever the "Cancel" button is pressed.
 * This rolls back every SciDoc in the ScriptSet to its last save point.
 */

void EditDiscard (HWND hwnd) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;
	if (!ss)
		return;

	// Go through all the scripts
	for (ScriptSet::IScript is = ss->scripts.begin(); is != ss->scripts.end(); is++) {
		sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, (sptr_t)(*is)->sciDoc);
		
		// Continue undoing changes until the document state matches the save point
		while (sm.stec.SendEditor(SCI_GETMODIFY) && sm.stec.SendEditor(SCI_CANUNDO))
			sm.stec.SendEditor(SCI_UNDO);

		// Sanity check
		int len = sm.stec.SendEditor(SCI_GETLENGTH);
		assert((*is)->script.length() == len);
	}

	// Restore previously selected document to the Scintilla control.
	if (ss->curScript)
		sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, (sptr_t)ss->curScript->sciDoc);
	else
		sm.stec.SendEditor(SCI_SETDOCPOINTER, 0, 0);
}

// Move this to EditSci?

void EditorAutoIndent (HWND hwnd) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;
	if (!ss || !ss->curScript)
		return;

	char linebuf[1024];
	int curPos = sm.stec.SendEditor(SCI_GETCURRENTPOS);
	int curLine = sm.stec.SendEditor(SCI_LINEFROMPOSITION, curPos);
	int lineLength = sm.stec.SendEditor(SCI_LINELENGTH, curLine);

	if (curLine > 0 && lineLength <= 2) {
		int prevLineLength = sm.stec.SendEditor(SCI_LINELENGTH, curLine - 1);
		if (prevLineLength < sizeof(linebuf)) {
			WORD buflen = sizeof(linebuf);
			memcpy(linebuf, &buflen, sizeof(buflen));
			sm.stec.SendEditor(SCI_GETLINE, curLine - 1, (LPARAM)(&linebuf[0]));
			linebuf[prevLineLength] = '\0';
			for (int pos = 0; linebuf[pos]; pos++) {
				if (linebuf[pos] != ' ' && linebuf[pos] != '\t')
					linebuf[pos] = '\0';
			}
			sm.stec.SendEditor(SCI_REPLACESEL, 0, (LPARAM)(&linebuf[0]));
		}
	}
}

void EditorMarginClick (Scintilla::SCNotification* notify) {
	ScriptManager& sm = ScriptManager::Get();

	const int lineNumber = sm.stec.SendEditor(SCI_LINEFROMPOSITION, notify->position, 0);

	switch (notify->margin) {
		case 2:
			sm.stec.SendEditor(SCI_TOGGLEFOLD, lineNumber);
			break;
	}
}

#define POS_ALIGN_LEFT		0x0001
#define POS_ALIGN_CENTER	0x0002
#define POS_ALIGN_RIGHT		0x0004
#define POS_ALIGN_TOP		0x0008
#define POS_ALIGN_MIDDLE	0x0010
#define POS_ALIGN_BOTTOM	0x0020

#define POS_FLOAT_LEFT		0x0040
#define POS_FLOAT_CENTER	0x0080
#define POS_FLOAT_RIGHT		0x0100
#define POS_FLOAT_ABOVE		0x0200
#define POS_FLOAT_MIDDLE	0x0400
#define POS_FLOAT_BELOW		0x0800

#define SIZ_W_PERCENT		0x0001
#define SIZ_W_EXACT			0x0002
#define SIZ_H_PERCENT		0x0004
#define SIZ_H_EXACT			0x0008

void PositionInterior (HWND container, HWND parent, HWND child, DWORD anchor, int xoff, int yoff) {
	RECT rcParent;
	RECT rcChild;
	GetWindowRect(child, &rcChild);

	if (container == parent)
		GetClientRect(parent, &rcParent);
	else
		GetWindowRect(parent, &rcParent);

	int wParent = rcParent.right - rcParent.left;
	int hParent = rcParent.bottom - rcParent.top;
	int wChild = rcChild.right - rcChild.left;
	int hChild = rcChild.bottom - rcChild.top;

	POINT pt;

	pt.x = rcParent.left;
	if (anchor & POS_ALIGN_LEFT)
		pt.x = pt.x + xoff;
	else if (anchor & POS_ALIGN_CENTER)
		pt.x = pt.x + (wParent / 2) - (wChild / 2) + xoff;
	else if (anchor & POS_ALIGN_RIGHT)
		pt.x = pt.x + wParent - wChild + xoff;

	pt.y = rcParent.top;
	if (anchor & POS_ALIGN_TOP)
		pt.y = pt.y + yoff;
	else if (anchor & POS_ALIGN_MIDDLE)
		pt.y = pt.y + (hParent / 2) - (hChild / 2) + yoff;
	else if (anchor & POS_ALIGN_BOTTOM)
		pt.y = pt.y + hParent - hChild + yoff;

	if (container != parent)
		ScreenToClient(container, &pt);

	MoveWindow(child, pt.x, pt.y, wChild, hChild, TRUE);
}

void PositionExterior (HWND container, HWND reference, HWND hwnd, DWORD anchor, int xoff, int yoff) {
	RECT rcRef;
	RECT rcWnd;
	GetWindowRect(reference, &rcRef);
	GetWindowRect(hwnd, &rcWnd);

	int wRef = rcRef.right - rcRef.left;
	int hRef = rcRef.bottom - rcRef.top;
	int wWnd = rcWnd.right - rcWnd.left;
	int hWnd = rcWnd.bottom - rcWnd.top;

	POINT pt;

	pt.x = xoff;
	if (anchor & POS_FLOAT_LEFT)
		pt.x = pt.x + rcRef.left - wWnd;
	else if (anchor & POS_FLOAT_RIGHT)
		pt.x = pt.x + rcRef.right;
	else {
		if (anchor & POS_ALIGN_LEFT)
			pt.x = pt.x + rcRef.left;
		else if (anchor & POS_ALIGN_RIGHT)
			pt.x = pt.x + rcRef.left + wRef - wWnd;
		else
			pt.x = pt.x + rcRef.left + (wRef / 2) - (wWnd / 2);
	}

	pt.y = yoff;
	if (anchor & POS_FLOAT_ABOVE)
		pt.y = pt.y + rcRef.top - hWnd;
	else if (anchor & POS_FLOAT_BELOW)
		pt.y = pt.y + rcRef.bottom;
	else {
		if (anchor & POS_ALIGN_TOP)
			pt.y = pt.y + rcRef.top;
		else if (anchor & POS_ALIGN_BOTTOM)
			pt.y = pt.y + rcRef.top + hRef - hWnd;
		else
			pt.y = pt.y + rcRef.top + (hRef / 2) - (hWnd / 2);
	}

	ScreenToClient(container, &pt);
	MoveWindow(hwnd, pt.x, pt.y, wWnd, hWnd, TRUE);
}

void ResizeInterior (HWND container, HWND parent, HWND child, DWORD siz, int pWidth, int pHeight, int resW, int resH) {
	RECT rcParent;
	RECT rcChild;
	GetClientRect(parent, &rcParent);
	GetWindowRect(child, &rcChild);

	int wParent = rcParent.right - rcParent.left - resW;
	int hParent = rcParent.bottom - rcParent.top - resH;

	int wChild = pWidth;
	int hChild = pHeight;

	if (siz & SIZ_W_PERCENT)
		wChild = wParent * pWidth / 100;

	if (siz & SIZ_H_PERCENT)
		hChild = hParent * pHeight / 100;
	
	POINT pt;
	pt.x = rcChild.left;
	pt.y = rcChild.right;

	ScreenToClient(container, &pt);
	MoveWindow(child, pt.x, pt.y, wChild, hChild, TRUE);
}

void ResizeSetup (HWND hwnd, int w, int h) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;
	sm.prefDlgWidth = w + 8;
	sm.prefDlgHeight = h + 11;

	RECT rcSetup;
	RECT rcClient;
	GetWindowRect(hwnd, &rcSetup);
	GetClientRect(hwnd, &rcClient);

	if (rcClient.right - rcClient.left != w || rcClient.bottom - rcClient.top != h) {
		MoveWindow(hwnd, rcSetup.left, rcSetup.top, w + 8, h + 27, true);
		return;
	}

	// Groups
	HWND hwGroupScript = GetDlgItem(hwnd, IDC_GROUP_SCRIPT);
	HWND hwGroupLib = GetDlgItem(hwnd, IDC_GROUP_LIB);
	HWND hwGroupEdit = GetDlgItem(hwnd, IDC_GROUP_EDIT);

	ResizeInterior(hwnd, hwnd, hwGroupScript, SIZ_W_PERCENT | SIZ_H_EXACT, 50, 140, 48, 48);
	PositionInterior(hwnd, hwnd, hwGroupScript, POS_ALIGN_LEFT | POS_ALIGN_TOP, 16, 16);

	ResizeInterior(hwnd, hwnd, hwGroupLib, SIZ_W_PERCENT | SIZ_H_EXACT, 50, 140, 48, 48);
	PositionExterior(hwnd, hwGroupScript, hwGroupLib, POS_FLOAT_RIGHT | POS_ALIGN_TOP, 16, 0);

	ResizeInterior(hwnd, hwnd, hwGroupEdit, SIZ_W_PERCENT | SIZ_H_PERCENT, 100, 100, 32, 48 + 140);
	PositionExterior(hwnd, hwGroupScript, hwGroupEdit, POS_FLOAT_BELOW | POS_ALIGN_LEFT, 0, 16);

	// Script
	HWND hwListScript = GetDlgItem(hwnd, IDC_LIST_SCRIPT);
	HWND hwButtonAddScr	= GetDlgItem(hwnd, IDC_BUTTON_ADDSCRIPT);
	HWND hwButtonRemScr = GetDlgItem(hwnd, IDC_BUTTON_REMSCRIPT);
	HWND hwButtonNameScr = GetDlgItem(hwnd, IDC_BUTTON_NAMESCRIPT);

	ResizeInterior(hwnd, hwGroupScript, hwListScript, SIZ_W_PERCENT | SIZ_H_PERCENT, 100, 100, 80, 32);
	PositionInterior(hwnd, hwGroupScript, hwListScript, POS_ALIGN_LEFT | POS_ALIGN_TOP, 8, 24);

	PositionInterior(hwnd, hwGroupScript, hwButtonAddScr, POS_ALIGN_RIGHT | POS_ALIGN_TOP, -8, 24);
	PositionExterior(hwnd, hwButtonAddScr, hwButtonRemScr, POS_FLOAT_BELOW | POS_ALIGN_LEFT, 0, 8);
	PositionInterior(hwnd, hwGroupScript, hwButtonNameScr, POS_ALIGN_RIGHT | POS_ALIGN_BOTTOM, -8, -8);

	// Lib
	HWND hwListLib = GetDlgItem(hwnd, IDC_LIST_LIB);
	HWND hwButtonAddLib	= GetDlgItem(hwnd, IDC_BUTTON_ADDLIB);
	HWND hwButtonRemLib = GetDlgItem(hwnd, IDC_BUTTON_REMLIB);

	ResizeInterior(hwnd, hwGroupLib, hwListLib, SIZ_W_PERCENT | SIZ_H_PERCENT, 100, 100, 80, 32);
	PositionInterior(hwnd, hwGroupLib, hwListLib, POS_ALIGN_LEFT | POS_ALIGN_TOP, 8, 24);

	PositionInterior(hwnd, hwGroupLib, hwButtonAddLib, POS_ALIGN_RIGHT | POS_ALIGN_TOP, -8, 24);
	PositionExterior(hwnd, hwButtonAddLib, hwButtonRemLib, POS_FLOAT_BELOW | POS_ALIGN_LEFT, 0, 8);

	// Editor
	HWND hwEditor = reinterpret_cast<HWND>(sm.stec.GetWindow()); //sd.sciEdit;
	HWND hwButtonOK = GetDlgItem(hwnd, IDOK);
	HWND hwButtonCancel = GetDlgItem(hwnd, IDCANCEL);
	HWND hwButtonApply = GetDlgItem(hwnd, IDC_BUTTON_APPLY);
	HWND hwCheckboxAuto = GetDlgItem(hwnd, IDC_CHECKBOX_AUTORUN);

	ResizeInterior(hwnd, hwGroupEdit, hwEditor, SIZ_W_PERCENT | SIZ_H_PERCENT, 100, 100, 16, 62);
	PositionInterior(hwnd, hwGroupEdit, hwEditor, POS_ALIGN_LEFT | POS_ALIGN_TOP, 8, 24);

	PositionInterior(hwnd, hwGroupEdit, hwButtonApply, POS_ALIGN_RIGHT | POS_ALIGN_BOTTOM, -8, -8);
	PositionExterior(hwnd, hwButtonApply, hwButtonCancel, POS_FLOAT_LEFT | POS_ALIGN_TOP, -8, 0);
	PositionExterior(hwnd, hwButtonCancel, hwButtonOK, POS_FLOAT_LEFT | POS_ALIGN_TOP, -8, 0);

	PositionInterior(hwnd, hwGroupEdit, hwCheckboxAuto, POS_ALIGN_LEFT | POS_ALIGN_BOTTOM, 8, -8);
}

int ScriptListCommand (HWND hwndDlg, WPARAM wParam, LPARAM lParam) {
	switch (HIWORD(wParam)) {
		case LBN_SELCHANGE:
			ScriptSelChange(hwndDlg);
			break;

		default:
			return false;
	}

	return true;
}

int EditCommand (HWND hwndDlg, WPARAM wParam, LPARAM lParam) {
	switch (HIWORD(wParam)) {
		case SCEN_KILLFOCUS:
			break;

		default:
			return false;
	}

	return true;
}

int EditNotify (HWND hwndDlg, WPARAM wParam, LPARAM lParam) {
	Scintilla::SCNotification* notification = (Scintilla::SCNotification*)lParam;

	switch (notification->nmhdr.code) {

		case SCN_CHARADDED:
			if ((char)notification->ch == '\n') {
				EditorAutoIndent(hwndDlg);
			}
			return false;

		case SCN_MARGINCLICK:
			EditorMarginClick(notification);
			return false;

		default:
			return false;
	}

	return true;
}

int CheckboxAutorunCommand (HWND hwndDlg, WPARAM wParam, LPARAM lParam) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;
	if (!ss)
		return false;

	switch (HIWORD(wParam)) {
		
		case BN_CLICKED: 
			{
				int state = SendDlgItemMessage(hwndDlg, IDC_CHECKBOX_AUTORUN, BM_GETCHECK, 0, 0);
				if (state == BST_CHECKED)
					ss->curScript->loadAtStart = true;
				else
					ss->curScript->loadAtStart = false;

				ss->updated = true;
			}
			return true;	
	}

	return false;
}

int ButtonApplyCommand (HWND hwndDlg, WPARAM wParam, LPARAM lParam) {

	switch (HIWORD(wParam)) {
		
		case BN_CLICKED: 
			EditSave(hwndDlg);
			return true;	
	}

	return false;
}

INT_PTR CALLBACK SetupDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ScriptManager& sm = ScriptManager::Get();
	ScriptSet* ss = sm.edPtr->tData->scripts;

	switch (uMsg) {

	case WM_INITDIALOG:
		sm.InitDlg(hwndDlg);
		return TRUE;

	case WM_CLOSE:
		sm.EndDlg(hwndDlg);
		return FALSE;

	case WM_SIZE:
		ResizeSetup(hwndDlg, LOWORD(lParam), HIWORD(lParam));
		return TRUE;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {
			case IDOK:
				EditSave(hwndDlg);
				sm.EndDlg(hwndDlg);
				EndDialog(hwndDlg, IDOK);
				break;

			case IDCANCEL:
				EditDiscard(hwndDlg);
				sm.EndDlg(hwndDlg);
				EndDialog(hwndDlg, IDCANCEL);
				break;

			case IDC_LIST_SCRIPT:
				ScriptListCommand(hwndDlg, wParam, lParam);
				break;

			case IDC_BUTTON_REMSCRIPT:
				if (HIWORD(wParam) == BN_CLICKED)
					ScriptDelete(hwndDlg);
				break;

			case IDC_BUTTON_ADDSCRIPT:
				if (HIWORD(wParam) == BN_CLICKED)
					ScriptAdd(hwndDlg);
				break;

			case IDC_BUTTON_NAMESCRIPT:
				if (HIWORD(wParam) == BN_CLICKED)
					ScriptRename(hwndDlg);
				break;

			case IDC_CHECKBOX_AUTORUN:
				return CheckboxAutorunCommand(hwndDlg, wParam, lParam);

			case IDC_BUTTON_APPLY:
				return ButtonApplyCommand(hwndDlg, wParam, lParam);
		}

		if (ss && ss->curScript && LOWORD(wParam) == sm.stec.GetResource())
			return EditCommand(hwndDlg, wParam, lParam);

		return false;

	case WM_NOTIFY:

		if (ss && ss->curScript && LOWORD(wParam) == sm.stec.GetResource())
			return EditNotify(hwndDlg, wParam, lParam);

		return false;

	default:
		return FALSE;
	}

	return TRUE;
}

INT_PTR CALLBACK NameDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {

	case WM_INITDIALOG:
		SetDlgItemText(hwndDlg, IDD_NEW, (LPCSTR)lParam);
		return TRUE;

	case WM_COMMAND:

		switch (LOWORD(wParam)) {
			case IDOK:
				CaptureName(hwndDlg);
				EndDialog(hwndDlg, IDOK);
				break;

			case IDCANCEL:
				EndDialog(hwndDlg, IDCANCEL);
				break;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

#endif
