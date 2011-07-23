#ifndef EDITSCI_H_
#define EDITSCI_H_

// License for EditSci.h, EditSci.cpp, and all Scintilla components:

// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
//
// All Rights Reserved 
//
// Permission to use, copy, modify, and distribute this software and its 
// documentation for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appear in all copies and that 
// both that copyright notice and this permission notice appear in 
// supporting documentation. 
//
// NEIL HODGSON DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
// SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
// AND FITNESS, IN NO EVENT SHALL NEIL HODGSON BE LIABLE FOR ANY 
// SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER 
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE 
// OR PERFORMANCE OF THIS SOFTWARE.

#include <windows.h>

#include "Platform.h"
#include "Scintilla.h"

// This file oraganizes pieces of code extracted from the SciTE
// distribution for use in the XLua internal editor.

class SciTECom {
protected:

	long		ptrEditor;
	long		resEditor;
	SciFnDirect fnEditor;

public:

	SciTECom ();
	SciTECom (long hwnd);

	SciTECom (const SciTECom& stec);

	SciTECom& operator= (const SciTECom& stec);

	long	GetWindow () const;
	long	GetResource () const;

	void	SetWindow (long hwnd);

	sptr_t	SendEditor(unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	sptr_t	SendEditorString(unsigned int msg, uptr_t wParam, const char *s);

	bool	MarginClick(int position, int modifiers);
	void	ToggleFoldRecursive(int line, int level);
	void	EnsureAllChildrenVisible(int line, int level);

	void	Expand(int &line, bool doExpand, bool force, int visLevels = 0, int level = -1);
	void	FoldAll();
};

struct ScintillaFailure {
	int status;
	ScintillaFailure(int status_) : status(status_) {
	}
};

#endif