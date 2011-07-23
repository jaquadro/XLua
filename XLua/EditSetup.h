#ifndef EDITSETUP_H_
#define EDITSETUP_H_

#include "Common.h"
#include "EditSci.h"
#include <string>
#include <list>

/**
 * ScriptData 
 *
 * Holds a single script and associated meta-information during the
 * embedded script editing process.  Creating an SD object will automatically
 * acquire a Scintilla document reference, and release it when destroyed.
 *
 * Scripts are stored permanently in EditRecord structures and embedded in the
 * end of the EDITDATA.
 */

class ScriptData {
public:

	std::string	name;
	std::string	script;
	bool		loadAtStart;

	SciTECom*	sciCom;
	void*		sciDoc;

public:

	ScriptData ()
		: name(""), script(""), loadAtStart(0), sciCom(0), sciDoc(0) {
	}

	ScriptData (const ScriptData& sd)
		: name(sd.name), script(sd.script), loadAtStart(sd.loadAtStart), sciCom(sd.sciCom), sciDoc(sd.sciDoc) {

		// Acquire new document reference
		if (sciCom && sciDoc) {
			sciCom->SendEditor(SCI_ADDREFDOCUMENT, 0, (sptr_t)sciDoc);
		}
	}

	~ScriptData () {
		if (sciCom && sciDoc) {
			sciCom->SendEditor(SCI_RELEASEDOCUMENT, 0, (sptr_t)sciDoc);
		}
	}

	ScriptData& operator= (const ScriptData& sd) {
		if (&sd == this)
			return *this;

		// Release old document reference
		if (sciCom && sciDoc) {
			sciCom->SendEditor(SCI_RELEASEDOCUMENT, 0, (sptr_t)sciDoc);
		}

		name = sd.name;
		script = sd.script;
		loadAtStart = sd.loadAtStart;
		sciCom = sd.sciCom;
		sciDoc = sd.sciDoc;

		// Acquire new document reference
		if (sciCom && sciDoc) {
			sciCom->SendEditor(SCI_ADDREFDOCUMENT, 0, (sptr_t)sciDoc);
		}

		return *this;
	}

};

/**
 * ScriptSet
 *
 * Holds a list of ScriptData objects -- every script added to a single XLua
 * object.  A single ScriptSet is owned by a single XLua object.
 *
 * During the editing process, scripts are added and removed through the
 * ScriptSet object.
 */

class ScriptSet {
public:

	// Serial is unique per XLua object but not currently used
	int			serial;
	LPMV		mV;

	bool		updated;
	ScriptData* curScript;

	std::list<ScriptData*>	scripts;

	typedef std::list<ScriptData*>::iterator IScript;
	typedef std::list<ScriptData*>::const_iterator CIScript;

public:

	ScriptSet ()
		: serial(0), mV(0), updated(0), scripts(0), curScript(0) {
	}

	ScriptSet (LPMV _mV)
		: serial(0), mV(_mV), updated(0), scripts(0), curScript(0) {
	}

	ScriptSet (const ScriptSet& ss)
		: serial(ss.serial), mV(ss.mV), updated(ss.updated), scripts(0), curScript(0) {

		for (CIScript it = ss.scripts.begin(); it != ss.scripts.end(); it++) {
			ScriptData* sd = new ScriptData(**it);
			scripts.push_back(sd);

			if (ss.curScript == *it)
				curScript = sd;
		}
	}

	~ScriptSet () {
		for (IScript it = scripts.begin(); it != scripts.end(); it++)
			delete *it;
	}

	ScriptSet& operator= (const ScriptSet& ss) {
		if (&ss == this)
			return *this;

		serial = ss.serial;
		mV = ss.mV;
		updated = ss.updated;

		// Delete all previously held scripts
		for (IScript it = scripts.begin(); it != scripts.end(); it++)
			delete *it;

		scripts.clear();
		curScript = 0;

		// Create independent copies of all the scripts
		for (CIScript it = ss.scripts.begin(); it != ss.scripts.end(); it++) {
			ScriptData* sd = new ScriptData(**it);
			scripts.push_back(sd);

			if (ss.curScript == *it)
				curScript = sd;
		}

		return *this;
	}

	// Loads all embedded EditRecord data from the EDITDATA into the ScriptSet
	void Refresh (LPMV mV, LPEDATA edPtr);

	// Manages scripts
	ScriptData* AddScript (std::string name, std::string src = "");
	ScriptData* RenameScript (std::string name, std::string newname);
	void RemScript (std::string name);

};

/**
 * ScriptManager
 *
 * The ScriptManager is a global object that holds information relating to the
 * editing dialog and Scintilla control.  It provides access to the current
 * valid edPtr and ScriptSet to the active editor dialog.
 */

class ScriptManager {
private:

	bool sciInit;

public:

	static int serialNext;

	LPEDATA		edPtr;
	LPMV		mV;

	SciTECom	stec;

	int			prefDlgWidth;
	int			prefDlgHeight;

	std::string	tempName;

	static ScriptManager& Get () {
		static ScriptManager sm;
		return sm;
	}

	void Init ();

	void InitDlg (HWND dlg);
	void EndDlg (HWND dlg);

private:

	ScriptManager ()
		: edPtr(0), mV(0), prefDlgWidth(600), prefDlgHeight(500), sciInit(0) {
	}

	ScriptManager (const ScriptManager& sm) { }

	ScriptManager& operator= (const ScriptManager& sm) { 
		return *this;
	}
};

// ---------------------------------------------------------------------------------------

int SetupDialog (mv _far *mV, LPEDATA edPtr);
HWND CreateEditControl (HWND hwndParent);

INT_PTR CALLBACK SetupDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK NameDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int ScriptListCommand (HWND hwndDlg, WPARAM wParam, LPARAM lParam);
int EditCommand (HWND hwndDlg, WPARAM wParam, LPARAM lParam);

void ResizeSetup (HWND hwnd, int w, int h);

void InitEditor (HWND hwnd);
void InitLists (HWND hwnd);
void ScriptSelChange (HWND hwnd);
void ScriptDelete (HWND hwnd);
void ScriptAdd (HWND hwnd);
void ScriptRename (HWND hwnd);

#endif
