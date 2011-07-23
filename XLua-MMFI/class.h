#ifndef CLASS_H_
#define CLASS_H_

#include "common.h"


class ObjectClass {
public:

	static int		NewObjectClass (lua_State* L);
	static LPOIL	GetObjectClass (lua_State* L, int index);

	// Extension Type
	static int	TypeID (lua_State* L);

	// Class Type
	static int	ClassID (lua_State* L);
	static int	ClassName (lua_State* L);

	// Getters

	static int	Common (lua_State* L);

	// Queries

	static int	ObjectCount (lua_State* L);
	static int	ObjectList (lua_State* L);

	static int	RandomObject (lua_State* L);
	static int	RandomObjectFunc (lua_State* L);

	/*static int	OverPoint (lua_State* L);
	static int	OverRect (lua_State* L);
	static int	OverBackdrop (lua_State* L);
	static int	OverObject (lua_State* L);
	static int	OverClass (lua_State* L);

	static int	ObjectsOverPoint (lua_State* L);
	static int	ObjectsOverRect (lua_State* L);
	static int	ObjectsOverBackdrop (lua_State* L);
	static int	ObjectsOverObject (lua_State* L);
	static int	ObjectsOverClass (lua_State* L);*/

	static int	TestBackdrop (lua_State* L);
	static int	TestBackdropFunc (lua_State* L);
	static int	TestBackdropGet (lua_State* L);
	static int	TestBackdropGetFunc (lua_State* L);

	static int	TestClass (lua_State* L);
	static int	TestClassFunc (lua_State* L);
	static int	TestClassGet (lua_State* L);
	static int	TestClassGetFunc (lua_State* L);
	static int	TestClassGetEx (lua_State* L);
	static int	TestClassGetExFunc (lua_State* L);

	static int	TestObject (lua_State* L);
	static int	TestObjectFunc (lua_State* L);
	static int	TestObjectGet (lua_State* L);
	static int	TestObjectGetFunc (lua_State* L);

	static int	TestPoint (lua_State* L);
	static int	TestPointFunc (lua_State* L);
	static int	TestPointGet (lua_State* L);
	static int	TestPointGetFunc (lua_State* L);

	static int	TestRect (lua_State* L);
	static int	TestRectFunc (lua_State* L);
	static int	TestRectGet (lua_State* L);
	static int	TestRectGetFunc (lua_State* L);

public:

	static int	TestBackdropUnified (lua_State* L, bool testOnly);
	static int	TestClassUnified (lua_State* L, bool testOnly);
	static int	TestObjectUnified (lua_State* L, bool testOnly);
	static int	TestPointUnified (lua_State* L, bool testOnly);
	static int	TestRectUnified (lua_State* L, bool testOnly);

public:

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);

	static int	MemoClosure (lua_State* L, const char* key, lua_CFunction lfunc);

	static int	ReturnObjects (lua_State* L, GenList& list);
	static void	ColFlagCheck (lua_State* L, LPRH rh, LPOIL oi);
};

const FunctionPair ObjectClassRead[] = {
	{ "classID",			ObjectClass::ClassID },
	{ "className",			ObjectClass::ClassName },
	{ "objectCount",		ObjectClass::ObjectCount },
	{ "objectList",			ObjectClass::ObjectList },
	{ "randomObject",		ObjectClass::RandomObject },
	{ "testBackdrop",		ObjectClass::TestBackdrop },
	{ "testBackdropGet",	ObjectClass::TestBackdropGet },
	{ "testClass",			ObjectClass::TestClass },
	{ "testClassGet",		ObjectClass::TestClassGet },
	{ "testClassGetEx",		ObjectClass::TestClassGetEx },
	{ "testObject",			ObjectClass::TestObject },
	{ "testObjectGet",		ObjectClass::TestObjectGet },
	{ "testPoint",			ObjectClass::TestPoint },
	{ "testPointGet",		ObjectClass::TestPointGet },
	{ "testRect",			ObjectClass::TestRect },
	{ "testRectGet",		ObjectClass::TestRectGet },
	{ "typeID",				ObjectClass::TypeID },
};

const FunctionPair ObjectClassWrite[] = {
	{ 0, 0 },
};

#endif
