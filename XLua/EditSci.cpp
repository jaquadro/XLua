#include "EditSci.h"

SciTECom::SciTECom () {
	ptrEditor = 0;
	fnEditor = 0;
}

SciTECom::SciTECom (long hwnd) {
	SetWindow(hwnd);
}

SciTECom::SciTECom (const SciTECom& stec) {
	ptrEditor = stec.ptrEditor;
	fnEditor = stec.fnEditor;
}

SciTECom& SciTECom::operator= (const SciTECom& stec) {
	ptrEditor = stec.ptrEditor;
	fnEditor = stec.fnEditor;

	return *this;
}

long SciTECom::GetWindow () const {
	return ptrEditor;
}

long SciTECom::GetResource () const {
	return resEditor;
}

void SciTECom::SetWindow (long hwnd) {
	ptrEditor = hwnd;
	resEditor = GetWindowLong(reinterpret_cast<HWND>(hwnd), GWL_ID);

	//fnEditor = reinterpret_cast<SciFnDirect>(SendMessage(reinterpret_cast<HWND>(hwnd), SCI_GETDIRECTFUNCTION, 0, 0));
}

sptr_t SciTECom::SendEditor(unsigned int msg, uptr_t wParam, sptr_t lParam) {
	sptr_t retVal = SendMessage(reinterpret_cast<HWND>(ptrEditor), msg, wParam, lParam);
		//fnEditor(ptrEditor, msg, wParam, lParam);
	sptr_t status = SendMessage(reinterpret_cast<HWND>(ptrEditor), SCI_GETSTATUS, 0, 0);
		//fnEditor(ptrEditor, SCI_GETSTATUS, 0, 0);
	if (status > 0)
		throw ScintillaFailure(status);
	return retVal;
}

sptr_t SciTECom::SendEditorString(unsigned int msg, uptr_t wParam, const char *s) {
	return SendEditor(msg, wParam, reinterpret_cast<sptr_t>(s));
}

bool SciTECom::MarginClick(int position, int modifiers) {
	int lineClick = SendEditor(SCI_LINEFROMPOSITION, position);
	//Platform::DebugPrintf("Margin click %d %d %x\n", position, lineClick,
	//	SendEditor(SCI_GETFOLDLEVEL, lineClick) & SC_FOLDLEVELHEADERFLAG);
	if ((modifiers & SCMOD_SHIFT) && (modifiers & SCMOD_CTRL)) {
		FoldAll();
	} else {
		int levelClick = SendEditor(SCI_GETFOLDLEVEL, lineClick);
		if (levelClick & SC_FOLDLEVELHEADERFLAG) {
			if (modifiers & SCMOD_SHIFT) {
				EnsureAllChildrenVisible(lineClick, levelClick);
			} else if (modifiers & SCMOD_CTRL) {
				ToggleFoldRecursive(lineClick, levelClick);
			} else {
				// Toggle this line
				SendEditor(SCI_TOGGLEFOLD, lineClick);
			}
		}
	}
	return true;
}

void SciTECom::ToggleFoldRecursive(int line, int level) {
	if (SendEditor(SCI_GETFOLDEXPANDED, line)) {
		// Contract this line and all children
		SendEditor(SCI_SETFOLDEXPANDED, line, 0);
		Expand(line, false, true, 0, level);
	} else {
		// Expand this line and all children
		SendEditor(SCI_SETFOLDEXPANDED, line, 1);
		Expand(line, true, true, 100, level);
	}
}

void SciTECom::EnsureAllChildrenVisible(int line, int level) {
	// Ensure all children visible
	SendEditor(SCI_SETFOLDEXPANDED, line, 1);
	Expand(line, true, true, 100, level);
}

void SciTECom::Expand(int &line, bool doExpand, bool force, int visLevels, int level) {
	int lineMaxSubord = SendEditor(SCI_GETLASTCHILD, line, level & SC_FOLDLEVELNUMBERMASK);
	line++;
	while (line <= lineMaxSubord) {
		if (force) {
			if (visLevels > 0)
				SendEditor(SCI_SHOWLINES, line, line);
			else
				SendEditor(SCI_HIDELINES, line, line);
		} else {
			if (doExpand)
				SendEditor(SCI_SHOWLINES, line, line);
		}
		int levelLine = level;
		if (levelLine == -1)
			levelLine = SendEditor(SCI_GETFOLDLEVEL, line);
		if (levelLine & SC_FOLDLEVELHEADERFLAG) {
			if (force) {
				if (visLevels > 1)
					SendEditor(SCI_SETFOLDEXPANDED, line, 1);
				else
					SendEditor(SCI_SETFOLDEXPANDED, line, 0);
				Expand(line, doExpand, force, visLevels - 1);
			} else {
				if (doExpand) {
					if (!SendEditor(SCI_GETFOLDEXPANDED, line))
						SendEditor(SCI_SETFOLDEXPANDED, line, 1);
					Expand(line, true, force, visLevels - 1);
				} else {
					Expand(line, false, force, visLevels - 1);
				}
			}
		} else {
			line++;
		}
	}
}

void SciTECom::FoldAll() {
	SendEditor(SCI_COLOURISE, 0, -1);
	int maxLine = SendEditor(SCI_GETLINECOUNT);
	bool expanding = true;
	for (int lineSeek = 0; lineSeek < maxLine; lineSeek++) {
		if (SendEditor(SCI_GETFOLDLEVEL, lineSeek) & SC_FOLDLEVELHEADERFLAG) {
			expanding = !SendEditor(SCI_GETFOLDEXPANDED, lineSeek);
			break;
		}
	}
	for (int line = 0; line < maxLine; line++) {
		int level = SendEditor(SCI_GETFOLDLEVEL, line);
		if ((level & SC_FOLDLEVELHEADERFLAG) &&
		        (SC_FOLDLEVELBASE == (level & SC_FOLDLEVELNUMBERMASK))) {
			if (expanding) {
				SendEditor(SCI_SETFOLDEXPANDED, line, 1);
				Expand(line, true, false, 0, level);
				line--;
			} else {
				int lineMaxSubord = SendEditor(SCI_GETLASTCHILD, line, -1);
				SendEditor(SCI_SETFOLDEXPANDED, line, 0);
				if (lineMaxSubord > line)
					SendEditor(SCI_HIDELINES, line + 1, lineMaxSubord);
			}
		}
	}
}
