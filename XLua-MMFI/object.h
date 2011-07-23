#ifndef OBJECT_H_
#define OBJECT_H_

#include "common.h"

// Every exported object that does not have a specific implementation
// is a CommonObject, with its capabilities determined by its OEFlags

class Object {
public:

	static int	NewObject (lua_State* L);
	static LPHO	GetObject (lua_State* L, int index);

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);

	static int	InheritCommon (lua_State* L);
	static int	MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc);
};

class Common {
public:

	// Extension Type
	static int	Type (lua_State* L);

	// Class Type
	static int	Class (lua_State* L);
	static int	ClassID (lua_State* L);
	static int	ClassName (lua_State* L);

	// Unique instance id
	static int	Fixed (lua_State* L);

	// State Getters
	static int	X (lua_State* L);
	static int	Y (lua_State* L);

	static int	XLeft (lua_State* L);
	static int	XRight (lua_State* L);
	static int	YTop (lua_State* L);
	static int	YBottom (lua_State* L);

	static int	Height (lua_State* L);
	static int	Width (lua_State* L);

	static int	HotspotX (lua_State* L);
	static int	HotspotY (lua_State* L);

	static int	Layer (lua_State* L);

	// State Mutators
	static int	SetX (lua_State* L);
	static int	SetY (lua_State* L);

	static int	SetLayer (lua_State* L);
	
	// Actions
	static int	Destroy (lua_State* L);
	static int	DestroyFunc (lua_State* L);

public:

	static int	CommonIndex (lua_State* L);
	static int	CommonNewIndex (lua_State* L);
};

const FunctionPair CommonRead[] = {
	{ "class",		Common::Class },
	{ "classID",	Common::ClassID },
	{ "className",	Common::ClassName },
	{ "destroy",	Common::Destroy },
	{ "fixed",		Common::Fixed },
	{ "height",		Common::Height },
	{ "hotX",		Common::HotspotX },
	{ "hotY",		Common::HotspotY },
	{ "layer",		Common::Layer },
	{ "type",		Common::Type },
	{ "width",		Common::Width },
	{ "x",			Common::X },
	{ "xLeft",		Common::XLeft },
	{ "xRight",		Common::XRight },
	{ "y",			Common::Y },
	{ "yBottom",	Common::YBottom },
	{ "yTop",		Common::YTop },
};

const FunctionPair CommonWrite[] = {
	{ "layer",		Common::SetLayer },
	{ "x",			Common::SetX },
	{ "y",			Common::SetY },
};

class Sprites {
public:

	// Getters

	static int	Visible (lua_State* L);

	static int	Angle (lua_State* L);
	static int	ScaleX (lua_State* L);
	static int	ScaleY (lua_State* L);

	static int	ZOrder (lua_State* L);

	static int	ActionX (lua_State* L);
	static int	ActionY (lua_State* L);

	static int	EdgeAA (lua_State* L);
	static int	RotationAA (lua_State* L);
	static int	ScaleAA (lua_State* L);

	static int	InkEffect (lua_State* L);
	static int	InkCoef (lua_State* L);
	static int	Transparent (lua_State* L);

	// Mutators

	static int	SetVisible (lua_State* L);

	static int	SetAngle (lua_State* L);
	static int	SetScaleX (lua_State* L);
	static int	SetScaleY (lua_State* L);

	static int	SetEdgeAA (lua_State* L);
	static int	SetRotationAA (lua_State* L);
	static int	SetScaleAA (lua_State* L);

	static int	SetInkEffect (lua_State* L);
	static int	SetInkCoef (lua_State* L);
	static int	SetTransparent (lua_State* L);

	// Queries

	static int	IsAbove (lua_State* L);
	static int	IsAboveFunc (lua_State* L);

	static int	IsBelow (lua_State* L);
	static int	IsBelowFunc (lua_State* L);

	static int	TestPoint (lua_State* L);
	static int	TestPointFunc (lua_State* L);

	static int	TestRect (lua_State* L);
	static int	TestRectFunc (lua_State* L);

	static int	TestMask (lua_State* L);

	static int	TestBackdrop (lua_State* L);
	static int	TestBackdropFunc (lua_State* L);

	static int	TestObject (lua_State* L);
	static int	TestObjectFunc (lua_State* L);

	static int	TestClass (lua_State* L);
	static int	TestClassFunc (lua_State* L);

	// Actions

	static int	MoveToFront (lua_State* L);
	static int	MoveToFrontFunc (lua_State* L);

	static int	MoveToBack (lua_State* L);
	static int	MoveToBackFunc (lua_State* L);

	static int	MoveAbove (lua_State* L);
	static int	MoveAboveFunc (lua_State* L);

	static int	MoveBelow (lua_State* L);
	static int	MoveBelowFunc (lua_State* L);

	static int	SwapOrder (lua_State* L);
	static int	SwapOrderFunc (lua_State* L);

public:

	static int	SpritesIndex (lua_State* L);
	static int	SpritesNewIndex (lua_State* L);

	static void	ColFlagCheck (lua_State* L, LPRH rh, LPHO ho);
};

const FunctionPair SpritesRead[] = {
	{ "actionX",		Sprites::ActionX },
	{ "actionY",		Sprites::ActionY },
	{ "angle",			Sprites::Angle },
	{ "edgeAA",			Sprites::EdgeAA },
	{ "inkCoef",		Sprites::InkCoef },
	{ "inkEffect",		Sprites::InkEffect },
	{ "isAbove",		Sprites::IsAbove },
	{ "isBelow",		Sprites::IsBelow },
	{ "moveAbove",		Sprites::MoveAbove },
	{ "moveBelow",		Sprites::MoveBelow },
	{ "moveToBack",		Sprites::MoveToBack },
	{ "moveToFront",	Sprites::MoveToFront },
	{ "rotatonAA",		Sprites::RotationAA },
	{ "scaleAA",		Sprites::ScaleAA },
	{ "scaleX",			Sprites::ScaleX },
	{ "scaleY",			Sprites::ScaleY },
	{ "swapOrder",		Sprites::SwapOrder },
	{ "testBackdrop",	Sprites::TestBackdrop },
	{ "testClass",		Sprites::TestClass },
	{ "testObject",		Sprites::TestObject },
	{ "testPoint",		Sprites::TestPoint },
	{ "testRect",		Sprites::TestRect },
	{ "transparent",	Sprites::Transparent },
	{ "visible",		Sprites::Visible },
	{ "zorder",			Sprites::ZOrder },
};

const FunctionPair SpritesWrite[] = {
	{ "angle",			Sprites::SetAngle },
	{ "edgeAA",			Sprites::SetEdgeAA },
	{ "inkCoef",		Sprites::SetInkCoef },
	{ "inkEffect",		Sprites::SetInkEffect },
	{ "rotationAA",		Sprites::SetRotationAA },
	{ "scaleAA",		Sprites::SetScaleAA },
	{ "scaleX",			Sprites::SetScaleX },
	{ "scaleY",			Sprites::SetScaleY },
	{ "transparent",	Sprites::SetTransparent },
	{ "visible",		Sprites::SetVisible },
};

class Movements {
public:
	
	// Getters

	static int	Direction (lua_State* L);

	// Mutators

	static int	SetDirection (lua_State* L);

public:

	static int	MovementsIndex (lua_State* L);
	static int	MovementsNewIndex (lua_State* L);
};

const FunctionPair MovementsRead[] = {
	{ "direction",	Movements::Direction },
};

const FunctionPair MovementsWrite[] = {
	{ "direction",	Movements::SetDirection },
};

class Animations {
public:

	// Getters

	static int	AnimationSet (lua_State* L);
	static int	AnimationFrame (lua_State* L);

	// Mutators

	static int	SetAnimationSet (lua_State* L);
	static int	SetAnimationFrame (lua_State* L);

public:

	static int	AnimationsIndex (lua_State* L);
	static int	AnimationsNewIndex (lua_State* L);
};

const FunctionPair AnimationsRead[] = {
	{ "animFrame",		Animations::AnimationFrame },
	{ "animSet",		Animations::AnimationSet },
};

const FunctionPair AnimationsWrite[] = {
	{ "animFrame",		Animations::SetAnimationFrame },
	{ "animSet",		Animations::SetAnimationSet },
};

class Values {
public:
	
	static int	Flag (lua_State* L);
	static int	Value (lua_State* L);
	static int	String (lua_State* L);

	static int	SetFlag (lua_State* L);
	static int	SetValue (lua_State* L);
	static int	SetString (lua_State* L);

public:

	static int	ValuesIndex (lua_State* L);
	static int	ValuesNewIndex (lua_State* L);

	static int	SubIndexMetamethod (lua_State* L);
	static int	SubNewIndexMetamethod (lua_State* L);

	static int	ValueKeyLookup (lua_State* L);
	static int	StringKeyLookup (lua_State* L);
};

#endif