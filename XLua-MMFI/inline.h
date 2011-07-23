// Included from common.h
// Necessary from non-support of 'export' keyword by mainstream compilers

template <int N1, int N2>
inline int StandardIndex (lua_State* L, const FunctionPair (&fpRead)[N1], const FunctionPair (&fpWrite)[N2]) {
	const char* key = lua_tostring(L, 2);

	lua_CFunction func = FindFunction(key, &fpRead[0], sizeof(fpRead) / sizeof(FunctionPair));
	if (func) {
		return func(L);
	}

	return -1;
}

template <int N1, int N2>
inline int StandardNewIndex (lua_State* L, const FunctionPair (&fpRead)[N1], const FunctionPair (&fpWrite)[N2]) {
	const char* key = lua_tostring(L, 2);

	lua_CFunction func = FindFunction(key, &fpWrite[0], sizeof(fpWrite) / sizeof(FunctionPair));
	if (func) {
		return func(L);
	}

	func = FindFunction(key, &fpRead[0], sizeof(fpRead) / sizeof(FunctionPair));
	if (func) {
		return ReadOnlyViolation(L, key);
	}

	return -1;
}
