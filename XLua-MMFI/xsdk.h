#ifndef XSDK_H_
#define XSDK_H_

#define XSDK_DLL

#if defined(XSDK_DLL)
#define XAPI __declspec(dllexport)
#else
#define XAPI __declspec(dllimport)
#endif

#endif
