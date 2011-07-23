#ifndef EXT_OPENGL_H_
#define EXT_OPENGL_H_

#include "xlua.h"

#include <windows.h>
#include <gl/gl.h>

#define OBJID MAKEID(O,G,L,E)

typedef struct OpenGlStruct
{
	headerObject rHo;
	rCom		rc;
	rMvt		rm;
	rSpr		rs;
	rVal		rv;
	LPRRDATA	rRd;

	HWND		hWnd; // Window handle
	HDC			hDC; // Device context handle
	HGLRC		hRC; // Render context handle
	cSurface*	ps;

	GLuint*		textureID;		// Texture list
	GLuint		dspListStart;	// ID of the first display list
	GLenum*		glsl;			// Shader list
	int			glslVar;		// Current shader

	short		maxTex;			// Number of textures
	short		maxDsp;			// Display lists
	short		maxSha;			// Shader programs

	/*GLuint textureID[256];
	GLuint dspListStart;
	GLenum glsl[256];*/
} EXTOPENGL;

#endif