// --------------------
// Object information
// --------------------

// Basic object details
#define	ObjectName			"XLua Surface"
#define	ObjectAuthor		"Justin Aquadro"
#define	ObjectCopyright		"Copyright © 2010 Justin Aquadro"
#define	ObjectComment		"A generic image surface to be manipulated from XLua"
#define	ObjectURL			"http://www.taloncrossing.com"
#define	ObjectHelp			"Help\\xlua-surface.chm"

// If you register your object with Clickteam, change this to the ID you were given
#define ObjectRegID			REGID_PRIVATE

// Change N,O,N,E to 4 unique characters (MMF currently still uses this to keep track)
#define	IDENTIFIER			MAKEID(X,L,S,F)

// --------------------
// Version information
// --------------------

// PRODUCT_VERSION_TGF or PRODUCT_VERSION_DEVELOPER
#define ForVersion			PRODUCT_VERSION_STANDARD

// Set this to the latest MMF build out when you build the object
#define	MinimumBuild		249

// --------------------
// Beta information
// --------------------

// #define BETA
// #define POPUP_ON_DROP
// #define POPUP_ON_EXE
// #define POPUP_ON_BUILD
// #define POPUP_MESSAGE	"This is a beta extension- use with caution!"

// --------------------
// Handling priority
// --------------------
// If this extension will handle windows messages, specify the priority
// of the handling procedure, 0 = low and 255 = very high

#define	WINDOWPROC_PRIORITY 100
