#ifndef XLUA_SURFACE_H_
#define XLUA_SURFACE_H_

#include "common.h"
#include "surfaceref.h"

class BaseSurfaceRef;
class BaseSurfaceRefFactory;

struct XSurfaceState {
	static const short	FILL_SOLID = 1;
	static const short	FILL_GRAD = 2;
	static const short	FILL_PATTERN = 3;

	static const short	GRAD_HORIZ = 1;
	static const short	GRAD_VERT = 2;

	static const short	ANGLE_RADIANS = 1;
	static const short	ANGLE_DEGREES = 2;

	// Inner Fill State
	short		inFillMode;
	short		inGradDir;
	COLORREF	inColor1;
	COLORREF	inColor2;
	BaseSurfaceRef*	inPattern;
	int			inOffX;
	int			inOffY;

	// Outer Fill State
	short		outFillMode;
	short		outGradDir;
	COLORREF	outColor1;
	COLORREF	outColor2;
	BaseSurfaceRef*	outPattern;
	int			outOffX;
	int			outOffY;

	// Drawing State
	bool		useInFill;
	bool		useOutFill;
	bool		useAA;
	bool		transparent;
	int			thickness;
	BlitOp		inkEffect;
	int			inkEffectParam;

	// Text State
	DWORD		textAlign;
	COLORREF	textColor;
	HFONT		textFont;

	// Misc State
	int			angleMode;
};


struct XSurfaceUD {
	BaseSurfaceRef*	ref;
	XSurfaceState*	state;
};


class XSurface {
public:

	static int			NewSurface (lua_State* L);
	static int			NewSurfaceImpl (lua_State* L, BaseSurfaceRefFactory* factory, bool init = false);

	static XSurfaceUD*	GetSurface (lua_State* L, int index);

	static int	Register (lua_State* L);
	static int	MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc);

	// Getters

	static int	Height (lua_State* L);
	static int	Width (lua_State* L);

	static int	OriginX (lua_State* L);
	static int	OriginY (lua_State* L);

	// ---

	static int	InnerFillMode (lua_State* L);
	static int	InnerColor1 (lua_State* L);
	static int	InnerColor2 (lua_State* L);
	static int	InnerGradientDir (lua_State* L);
	static int	InnerPattern (lua_State* L);
	static int	InnerOffsetX (lua_State* L);
	static int	InnerOffsetY (lua_State* L);

	static int	OuterFillMode (lua_State* L);
	static int	OuterColor1 (lua_State* L);
	static int	OuterColor2 (lua_State* L);
	static int	OuterGradientDir (lua_State* L);
	static int	OuterPattern (lua_State* L);
	static int	OuterOffsetX (lua_State* L);
	static int	OuterOffsetY (lua_State* L);

	static int	UseInnerFill (lua_State* L);
	static int	UseOuterFill (lua_State* L);
	static int	UseAA (lua_State* L);
	static int	Transparent (lua_State* L);
	static int	LineWidth (lua_State* L);
	static int	InkEffect (lua_State* L);
	static int	InkEffectParam (lua_State* L);

	static int	TextFont (lua_State* L);
	static int	TextSize (lua_State* L);
	static int	TextColor (lua_State* L);
	static int	TextHAlign (lua_State* L);
	static int	TextVAlign (lua_State* L);

	// Setters

	static int	SetOriginX (lua_State* L);
	static int	SetOriginY (lua_State* L);

	static int	SetInnerFillMode (lua_State* L);
	static int	SetInnerColor1 (lua_State* L);
	static int	SetInnerColor2 (lua_State* L);
	static int	SetInnerGradientDir (lua_State* L);
	static int	SetInnerPattern (lua_State* L);
	static int	SetInnerOffsetX (lua_State* L);
	static int	SetInnerOffsetY (lua_State* L);

	static int	SetOuterFillMode (lua_State* L);
	static int	SetOuterColor1 (lua_State* L);
	static int	SetOuterColor2 (lua_State* L);
	static int	SetOuterGradientDir (lua_State* L);
	static int	SetOuterPattern (lua_State* L);
	static int	SetOuterOffsetX (lua_State* L);
	static int	SetOuterOffsetY (lua_State* L);

	static int	SetUseInnerFill (lua_State* L);
	static int	SetUseOuterFill (lua_State* L);
	static int	SetUseAA (lua_State* L);
	static int	SetTransparent (lua_State* L);
	static int	SetLineWidth (lua_State* L);
	static int	SetInkEffect (lua_State* L);
	static int	SetInkEffectParam (lua_State* L);

	static int	SetTextFont (lua_State* L);
	static int	SetTextSize (lua_State* L);
	static int	SetTextColor (lua_State* L);
	static int	SetTextHAlign (lua_State* L);
	static int	SetTextVAlign (lua_State* L);

	// Queries

	static int	GetPixel (lua_State* L);
	static int	GetTransparentColor (lua_State* L);

	static int	Copy (lua_State* L);
	static int	CopyFunc (lua_State* L);

	static int	CopyBlock (lua_State* L);
	static int	CopyBlockFunc (lua_State* L);

	static int	CopyRotated (lua_State* L);
	static int	CopyScaled (lua_State* L);

	// Actions

	static int	CopyInto (lua_State* L);
	static int	CopyIntoFunc (lua_State* L);

	static int	CopyBlockInto (lua_State* L);
	static int	CopyBlockIntoFunc (lua_State* L);

	static int	SetPixel (lua_State* L);
	static int	SetPixelFunc (lua_State* L);

	static int	SetTransparentColor (lua_State* L);

	static int	FlipX (lua_State* L);
	static int	FlipY (lua_State* L);

	static int	Fill (lua_State* L);
	static int	FillFunc (lua_State* L);

	static int	FillBlock (lua_State* L);
	static int	FillBlockFunc (lua_State* L);

	static int	FloodFill (lua_State* L);
	static int	FloodFillFunc (lua_State* L);

	static int	Line (lua_State* L);
	static int	LineFunc (lua_State* L);

	static int	Ellipse (lua_State* L);
	static int	EllipseFunc (lua_State* L);

	static int	Rectangle (lua_State* L);
	static int	RectangleFunc (lua_State* L);

	static int	Polygon (lua_State* L);
	static int	PolygonFunc (lua_State* L);

	static int	DrawText (lua_State* L);

public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);
};

const FunctionPair SurfaceRead[] = {
	{ "copy",				XSurface::Copy },
	{ "copyBlock",			XSurface::CopyBlock },
	{ "copyBlockInto",		XSurface::CopyBlockInto },
	{ "copyInto",			XSurface::CopyInto },
	{ "ellipse",			XSurface::Ellipse },
	{ "fill",				XSurface::Fill },
	{ "fillBlock",			XSurface::FillBlock },
	{ "floodFill",			XSurface::FloodFill },
	{ "height",				XSurface::Height },
	{ "line",				XSurface::Line },
	{ "polygon",			XSurface::Polygon },
	{ "rectangle",			XSurface::Rectangle },
	{ "setPixel",			XSurface::SetPixel },
	{ "width",				XSurface::Width },
};

const FunctionPair SurfaceWrite[] = {
	{ "inkEffect",			XSurface::SetInkEffect },
	{ "inkEffectParam",		XSurface::SetInkEffectParam },
	{ "innerColor1",		XSurface::SetInnerColor1 },
	{ "innerColor2",		XSurface::SetInnerColor2 },
	{ "innerFillMode",		XSurface::SetInnerFillMode },
	{ "innerGradientDir",	XSurface::SetInnerGradientDir },
	{ "innerOffsetX",		XSurface::SetInnerOffsetX },
	{ "innerOffsetY",		XSurface::SetInnerOffsetY },
	{ "innerPattern",		XSurface::SetInnerPattern },
	{ "lineWidth",			XSurface::SetLineWidth },
	{ "outerColor1",		XSurface::SetOuterColor1 },
	{ "outerColor2",		XSurface::SetOuterColor2 },
	{ "outerFillMode",		XSurface::SetOuterFillMode },
	{ "outerGradientDir",	XSurface::SetOuterGradientDir },
	{ "outerPattern",		XSurface::SetOuterPattern },
	{ "outerOffsetX",		XSurface::SetOuterOffsetX },
	{ "outerOffsetY",		XSurface::SetOuterOffsetY },
	{ "transparent",		XSurface::SetTransparent },
	{ "useAA",				XSurface::SetUseAA },
	{ "useInnerFill",		XSurface::SetUseInnerFill },
	{ "useOuterFill",		XSurface::SetUseOuterFill },
};

#endif
