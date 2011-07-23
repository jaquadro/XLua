#ifndef DATA_H_
#define DATA_H_

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// If you want to store anything between actions/conditions/expressions
// you should store it here

typedef struct tagRDATA
{
	headerObject	rHo;
	XLuaObject*		luaMan;
} RUNDATA;

typedef	RUNDATA	* LPRDATA;

// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
// These values let you store data in your extension that will be saved in the MFA
// You should use these with properties

#define XLUA_VERSION_0 1
#define XLUA_VERSION_1 2
#define XLUA_VERSION_2 3
#define XLUA_VERSION_3 4
#define XLUA_VERSION_4 5

#define XLUA_VERSION_CURRENT XLUA_VERSION_4

struct tagEDATA_V0
{
	extHeader		eHeader;
};

struct tagEDATA_V1
{
	extHeader		eHeader;
	
	// Update 1
	int				initState;
	int				errMode;
	int				printMode;
	bool			bindState;
	bool			useMMFI;
	bool			useWINI;
	bool			runScript;
	bool			loadLibs;
	
	char			script[1];

};

struct tagEDATA_V2
{
	extHeader		eHeader;
	
	// Update 1
	int				initState;
	int				errMode;
	int				printMode;
	bool			bindState;
	bool			useMMFI;
	bool			useWINI;
	bool			runScript;
	bool			loadLibs;

	// Update 2
	bool			autoReg;
	bool			useBacktrace;
	bool			unused2;
	
	char			script[1];

};

struct EditRecord;

struct tagEDATA_V3
{
	extHeader		eHeader;
	
	// Update 1
	int				initState;
	int				errMode;
	int				printMode;
	bool			bindState;
	bool			useMMFI;
	bool			useWINI;
	bool			runScript;
	bool			loadLibs;

	// Update 2
	bool			autoReg;
	bool			useBacktrace;
	bool			unused2;

	// Update 3
	int				numScripts;
	int				numLibs;
	int				scriptRecordOffset;
	int				libRecordOffset;
	int				stringOffset;

	EditRecord*		ScriptAtIndex(int index);
	EditRecord*		LibAtIndex(int index);
};

struct TempData;

typedef struct tagEDATA_V4
{
	extHeader		eHeader;
	
	// Update 1
	int				initState;
	int				errMode;
	int				printMode;
	bool			bindState;
	bool			useMMFI;
	bool			useWINI;
	bool			runScript;
	bool			loadLibs;

	// Update 2
	bool			autoReg;
	bool			useBacktrace;
	bool			unused2;

	// Update 3
	int				numScripts;
	int				numLibs;
	int				scriptRecordOffset;
	int				libRecordOffset;
	int				stringOffset;

	// Update 4
	TempData*		tData;
	DWORD			unused3;
	DWORD			unused4;
	DWORD			unused5;

	EditRecord*		ScriptAtIndex(int index);
	EditRecord*		LibAtIndex(int index);
} EDITDATA;

typedef EDITDATA * LPEDATA;

class ScriptSet;

struct TempData {
	int				serial;
	ScriptSet*		scripts;
};

struct EditRecord {
	size_t			nameOffset;
	size_t			nameLen;
	size_t			srcOffset;
	size_t			srcLen;

	DWORD			flags;
	DWORD			unused1;
	DWORD			unused2;
	DWORD			unused3;

	char*			Name (EDITDATA* edPtr);
	char*			Src (EDITDATA* edPtr);
};

#endif
