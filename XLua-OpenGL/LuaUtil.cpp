#include "LuaUtil.h"

bool check2 (lua_State* L, int index, int* n1, int* n2) {
	// Check first dimension
	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	// Check second dimension
	lua_rawgeti(L, index, 1);
	if(!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return false;
	}

	if (*n2 && *n2 != luaL_getn(L, -1)) {
		lua_pop(L, 1);
		return false;
	}
	else
		*n2 = luaL_getn(L, -1);

	lua_pop(L, 1);
	return true;
}

bool check3 (lua_State* L, int index, int* n1, int* n2, int* n3) {
	// Check first dimension
	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	// Check second dimension
	lua_rawgeti(L, index, 1);
	if(!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return false;
	}

	if (*n2 && *n2 != luaL_getn(L, -1)) {
		lua_pop(L, 1);
		return false;
	}
	else
		*n2 = luaL_getn(L, -1);

	// Check third dimension
	lua_rawgeti(L, -1, 1);
	if (!lua_istable(L, -1)) {
		lua_pop(L, 2);
		return false;
	}

	if (*n3 && *n3 != luaL_getn(L, -1)) {
		lua_pop(L, 2);
		return false;
	}
	else
		*n3 = luaL_getn(L, -1);

	lua_pop(L, 2);
	return true;
}

bool check4 (lua_State* L, int index, int* n1, int* n2, int* n3, int* n4) {
	// Check first dimension
	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	// Check second dimension
	lua_rawgeti(L, index, 1);
	if(!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return false;
	}

	if (*n2 && *n2 != luaL_getn(L, -1)) {
		lua_pop(L, 1);
		return false;
	}
	else
		*n2 = luaL_getn(L, -1);

	// Check third dimension
	lua_rawgeti(L, -1, 1);
	if (!lua_istable(L, -1)) {
		lua_pop(L, 2);
		return false;
	}

	if (*n3 && *n3 != luaL_getn(L, -1)) {
		lua_pop(L, 2);
		return false;
	}
	else
		*n3 = luaL_getn(L, -1);

	// Check fourth dimension
	lua_rawgeti(L, -1, 1);
	if (!lua_istable(L, -1)) {
		lua_pop(L, 3);
		return false;
	}

	if (*n4 && *n4 != luaL_getn(L, -1)) {
		lua_pop(L, 3);
		return false;
	}
	else
		*n4 = luaL_getn(L, -1);

	lua_pop(L, 3);
	return true;
}

bool checkSub (lua_State* L, int index, int i, int n, int depth) {
	lua_rawgeti(L, index, i + 1);
	if(!lua_istable(L, -1) || luaL_getn(L, -1) != n) {
		lua_pop(L, depth);
		return false;
	}

	return true;
}

// -----

int getDimensions (lua_State* L, int index) {
	int dimCount = 1;

	lua_rawgeti(L, index, 1);
	while (lua_istable(L, -1)) {
		dimCount++;
		lua_rawgeti(L, -1, 1);
	}

	lua_pop(L, dimCount);
	return dimCount;
}

int getDimLength (lua_State* L, int index, int dim) {
	int dCount = 1;

	lua_pushvalue(L, index);
	while (dCount < dim) {
		lua_rawgeti(L, -1, 1);
		dCount++;
	}

	int len = luaL_getn(L, -1);

	lua_pop(L, dim);
	return len;
}

// -----

void setField (lua_State *L, unsigned int index, lua_Number value) {
	lua_pushnumber(L, index);
	lua_pushnumber(L, value);
	lua_settable(L, -3);
}

// -----

bool getArray1bool (lua_State* L, int index, GLboolean** arr, int* n1) {

	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	int lim1 = *n1;

	*arr =  new GLboolean[lim1];
	for(int i = 0; i < lim1; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*arr)[i] = (GLboolean)lua_toboolean(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

bool getArray1b (lua_State* L, int index, GLbyte** arr, int* n1) {

	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	int lim1 = *n1;

	*arr =  new GLbyte[lim1];
	for(int i = 0; i < lim1; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*arr)[i] = (GLbyte)lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

bool getArray1ub (lua_State* L, int index, GLubyte** arr, int* n1) {

	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	int lim1 = *n1;

	*arr =  new GLubyte[lim1];
	for(int i = 0; i < lim1; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*arr)[i] = (GLubyte)lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

bool getArray1s (lua_State* L, int index, GLshort** arr, int* n1) {

	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	int lim1 = *n1;

	*arr =  new GLshort[lim1];
	for(int i = 0; i < lim1; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*arr)[i] = (GLshort)lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

bool getArray1us (lua_State* L, int index, GLushort** arr, int* n1) {

	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	int lim1 = *n1;

	*arr =  new GLushort[lim1];
	for(int i = 0; i < lim1; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*arr)[i] = (GLushort)lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

bool getArray1i (lua_State* L, int index, GLint** arr, int* n1) {

	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	int lim1 = *n1;

	*arr =  new GLint[lim1];
	for(int i = 0; i < lim1; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*arr)[i] = (GLint)lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

bool getArray1ui (lua_State* L, int index, GLuint** arr, int* n1) {

	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	int lim1 = *n1;

	*arr =  new GLuint[lim1];
	for(int i = 0; i < lim1; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*arr)[i] = (GLuint)lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

bool getArray1f (lua_State* L, int index, GLfloat** arr, int* n1) {

	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	int lim1 = *n1;

	*arr =  new GLfloat[lim1];
	for(int i = 0; i < lim1; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*arr)[i] = (GLfloat)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

bool getArray1d (lua_State* L, int index, GLdouble** arr, int* n1) {

	if (*n1 && *n1 != luaL_getn(L, index))
		return false;
	else
		*n1 = luaL_getn(L, index);

	int lim1 = *n1;

	*arr =  new GLdouble[lim1];
	for(int i = 0; i < lim1; i++)
	{
		lua_rawgeti(L, index, i + 1);
		(*arr)[i] = (GLdouble)lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	return true;
}

// -----

bool getArray2b (lua_State* L, int index, GLbyte** arr, int* n1, int* n2) {

	// Check first dimension
	if (!check2(L, index, n1, n2))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;

	// Allocate storage
	*arr = new GLbyte[lim1 * lim2];

	for (int i = 0; i < lim1; i++)
	{
		if (!checkSub(L, index, i, lim2, 1))
			return false;

		for (int j = 0; j < lim2; j++) {
			lua_rawgeti(L, -1, j + 1);
			(*arr)[i*lim2 + j] = (GLbyte)lua_tointeger(L, -1);
			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray2ub (lua_State* L, int index, GLubyte** arr, int* n1, int* n2) {

	// Check first dimension
	if (!check2(L, index, n1, n2))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;

	// Allocate storage
	*arr = new GLubyte[lim1 * lim2];

	for (int i = 0; i < lim1; i++)
	{
		if (!checkSub(L, index, i, lim2, 1))
			return false;

		for (int j = 0; j < lim2; j++) {
			lua_rawgeti(L, -1, j + 1);
			(*arr)[i*lim2 + j] = (GLubyte)lua_tointeger(L, -1);
			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray2s (lua_State* L, int index, GLshort** arr, int* n1, int* n2) {

	// Check first dimension
	if (!check2(L, index, n1, n2))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;

	// Allocate storage
	*arr = new GLshort[lim1 * lim2];

	for (int i = 0; i < lim1; i++)
	{
		if (!checkSub(L, index, i, lim2, 1))
			return false;

		for (int j = 0; j < lim2; j++) {
			lua_rawgeti(L, -1, j + 1);
			(*arr)[i*lim2 + j] = (GLshort)lua_tointeger(L, -1);
			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray2us (lua_State* L, int index, GLushort** arr, int* n1, int* n2) {

	// Check first dimension
	if (!check2(L, index, n1, n2))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;

	// Allocate storage
	*arr = new GLushort[lim1 * lim2];

	for (int i = 0; i < lim1; i++)
	{
		if (!checkSub(L, index, i, lim2, 1))
			return false;

		for (int j = 0; j < lim2; j++) {
			lua_rawgeti(L, -1, j + 1);
			(*arr)[i*lim2 + j] = (GLushort)lua_tointeger(L, -1);
			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray2i (lua_State* L, int index, GLint** arr, int* n1, int* n2) {

	// Check first dimension
	if (!check2(L, index, n1, n2))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;

	// Allocate storage
	*arr = new GLint[lim1 * lim2];

	for (int i = 0; i < lim1; i++)
	{
		if (!checkSub(L, index, i, lim2, 1))
			return false;

		for (int j = 0; j < lim2; j++) {
			lua_rawgeti(L, -1, j + 1);
			(*arr)[i*lim2 + j] = (GLint)lua_tointeger(L, -1);
			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray2ui (lua_State* L, int index, GLuint** arr, int* n1, int* n2) {

	// Check first dimension
	if (!check2(L, index, n1, n2))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;

	// Allocate storage
	*arr = new GLuint[lim1 * lim2];

	for (int i = 0; i < lim1; i++)
	{
		if (!checkSub(L, index, i, lim2, 1))
			return false;

		for (int j = 0; j < lim2; j++) {
			lua_rawgeti(L, -1, j + 1);
			(*arr)[i*lim2 + j] = (GLuint)lua_tointeger(L, -1);
			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray2f (lua_State* L, int index, GLfloat** arr, int* n1, int* n2) {

	// Check first dimension
	if (!check2(L, index, n1, n2))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;

	// Allocate storage
	*arr = new GLfloat[lim1 * lim2];

	for (int i = 0; i < lim1; i++)
	{
		if (!checkSub(L, index, i, lim2, 1))
			return false;

		for (int j = 0; j < lim2; j++) {
			lua_rawgeti(L, -1, j + 1);
			(*arr)[i*lim2 + j] = (GLfloat)lua_tonumber(L, -1);
			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray2d (lua_State* L, int index, GLdouble** arr, int* n1, int* n2) {

	// Check first dimension
	if (!check2(L, index, n1, n2))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;

	// Allocate storage
	*arr = new GLdouble[lim1 * lim2];

	for (int i = 0; i < lim1; i++)
	{
		if (!checkSub(L, index, i, lim2, 1))
			return false;

		for (int j = 0; j < lim2; j++) {
			lua_rawgeti(L, -1, j + 1);
			(*arr)[i*lim2 + j] = (GLdouble)lua_tointeger(L, -1);
			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

// -----

bool getArray3b (lua_State* L, int index, GLbyte** arr, int* n1, int* n2, int* n3) {

	// Check first dimension
	if (!check3(L, index, n1, n2, n3))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;

	// Allocate storage
	*arr = new GLbyte[lim1 * lim2 * lim3];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				lua_rawgeti(L, -1, k + 1);
				(*arr)[i*lim2*lim3 + j*lim3 + k] = (GLbyte)lua_tointeger(L, -1);
				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray3ub (lua_State* L, int index, GLubyte** arr, int* n1, int* n2, int* n3) {

	// Check first dimension
	if (!check3(L, index, n1, n2, n3))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;

	// Allocate storage
	*arr = new GLubyte[lim1 * lim2 * lim3];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				lua_rawgeti(L, -1, k + 1);
				(*arr)[i*lim2*lim3 + j*lim3 + k] = (GLubyte)lua_tointeger(L, -1);
				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray3s (lua_State* L, int index, GLshort** arr, int* n1, int* n2, int* n3) {

	// Check first dimension
	if (!check3(L, index, n1, n2, n3))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;

	// Allocate storage
	*arr = new GLshort[lim1 * lim2 * lim3];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				lua_rawgeti(L, -1, k + 1);
				(*arr)[i*lim2*lim3 + j*lim3 + k] = (GLshort)lua_tointeger(L, -1);
				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray3us (lua_State* L, int index, GLushort** arr, int* n1, int* n2, int* n3) {

	// Check first dimension
	if (!check3(L, index, n1, n2, n3))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;

	// Allocate storage
	*arr = new GLushort[lim1 * lim2 * lim3];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				lua_rawgeti(L, -1, k + 1);
				(*arr)[i*lim2*lim3 + j*lim3 + k] = (GLushort)lua_tointeger(L, -1);
				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray3i (lua_State* L, int index, GLint** arr, int* n1, int* n2, int* n3) {

	// Check first dimension
	if (!check3(L, index, n1, n2, n3))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;

	// Allocate storage
	*arr = new GLint[lim1 * lim2 * lim3];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				lua_rawgeti(L, -1, k + 1);
				(*arr)[i*lim2*lim3 + j*lim3 + k] = (GLint)lua_tointeger(L, -1);
				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray3ui (lua_State* L, int index, GLuint** arr, int* n1, int* n2, int* n3) {

	// Check first dimension
	if (!check3(L, index, n1, n2, n3))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;

	// Allocate storage
	*arr = new GLuint[lim1 * lim2 * lim3];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				lua_rawgeti(L, -1, k + 1);
				(*arr)[i*lim2*lim3 + j*lim3 + k] = (GLuint)lua_tointeger(L, -1);
				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray3f (lua_State* L, int index, GLfloat** arr, int* n1, int* n2, int* n3) {

	// Check first dimension
	if (!check3(L, index, n1, n2, n3))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;

	// Allocate storage
	*arr = new GLfloat[lim1 * lim2 * lim3];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				lua_rawgeti(L, -1, k + 1);
				(*arr)[i*lim2*lim3 + j*lim3 + k] = (GLfloat)lua_tonumber(L, -1);
				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray3d (lua_State* L, int index, GLdouble** arr, int* n1, int* n2, int* n3) {

	// Check first dimension
	if (!check3(L, index, n1, n2, n3))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;

	// Allocate storage
	*arr = new GLdouble[lim1 * lim2 * lim3];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				lua_rawgeti(L, -1, k + 1);
				(*arr)[i*lim2*lim3 + j*lim3 + k] = (GLdouble)lua_tonumber(L, -1);
				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

// -----

bool getArray4b (lua_State* L, int index, GLbyte** arr, int* n1, int* n2, int* n3, int* n4) {

	// Check first dimension
	if (!check4(L, index, n1, n2, n3, n4))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;
	int lim4 = *n4;

	// Allocate storage
	*arr = new GLbyte[lim1 * lim2 * lim3 * lim4];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				if (!checkSub(L, -1, k, lim4, 3))
					return false;

				for (int l = 0; l < lim4; l++) {
					lua_rawgeti(L, -1, l + 1);
					(*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (GLbyte)lua_tointeger(L, -1);
					lua_remove(L, -1);
				}

				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray4ub (lua_State* L, int index, GLubyte** arr, int* n1, int* n2, int* n3, int* n4) {

	// Check first dimension
	if (!check4(L, index, n1, n2, n3, n4))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;
	int lim4 = *n4;

	// Allocate storage
	*arr = new GLubyte[lim1 * lim2 * lim3 * lim4];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				if (!checkSub(L, -1, k, lim4, 3))
					return false;

				for (int l = 0; l < lim4; l++) {
					lua_rawgeti(L, -1, l + 1);
					(*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (GLubyte)lua_tointeger(L, -1);
					lua_remove(L, -1);
				}

				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray4s (lua_State* L, int index, GLshort** arr, int* n1, int* n2, int* n3, int* n4) {

	// Check first dimension
	if (!check4(L, index, n1, n2, n3, n4))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;
	int lim4 = *n4;

	// Allocate storage
	*arr = new GLshort[lim1 * lim2 * lim3 * lim4];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				if (!checkSub(L, -1, k, lim4, 3))
					return false;

				for (int l = 0; l < lim4; l++) {
					lua_rawgeti(L, -1, l + 1);
					(*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (GLshort)lua_tointeger(L, -1);
					lua_remove(L, -1);
				}

				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray4us (lua_State* L, int index, GLushort** arr, int* n1, int* n2, int* n3, int* n4) {

	// Check first dimension
	if (!check4(L, index, n1, n2, n3, n4))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;
	int lim4 = *n4;

	// Allocate storage
	*arr = new GLushort[lim1 * lim2 * lim3 * lim4];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				if (!checkSub(L, -1, k, lim4, 3))
					return false;

				for (int l = 0; l < lim4; l++) {
					lua_rawgeti(L, -1, l + 1);
					(*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (GLushort)lua_tointeger(L, -1);
					lua_remove(L, -1);
				}

				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray4i (lua_State* L, int index, GLint** arr, int* n1, int* n2, int* n3, int* n4) {

	// Check first dimension
	if (!check4(L, index, n1, n2, n3, n4))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;
	int lim4 = *n4;

	// Allocate storage
	*arr = new GLint[lim1 * lim2 * lim3 * lim4];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				if (!checkSub(L, -1, k, lim4, 3))
					return false;

				for (int l = 0; l < lim4; l++) {
					lua_rawgeti(L, -1, l + 1);
					(*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (GLint)lua_tointeger(L, -1);
					lua_remove(L, -1);
				}

				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray4ui (lua_State* L, int index, GLuint** arr, int* n1, int* n2, int* n3, int* n4) {

	// Check first dimension
	if (!check4(L, index, n1, n2, n3, n4))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;
	int lim4 = *n4;

	// Allocate storage
	*arr = new GLuint[lim1 * lim2 * lim3 * lim4];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				if (!checkSub(L, -1, k, lim4, 3))
					return false;

				for (int l = 0; l < lim4; l++) {
					lua_rawgeti(L, -1, l + 1);
					(*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (GLuint)lua_tointeger(L, -1);
					lua_remove(L, -1);
				}

				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray4f (lua_State* L, int index, GLfloat** arr, int* n1, int* n2, int* n3, int* n4) {

	// Check first dimension
	if (!check4(L, index, n1, n2, n3, n4))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;
	int lim4 = *n4;

	// Allocate storage
	*arr = new GLfloat[lim1 * lim2 * lim3 * lim4];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				if (!checkSub(L, -1, k, lim4, 3))
					return false;

				for (int l = 0; l < lim4; l++) {
					lua_rawgeti(L, -1, l + 1);
					(*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (GLfloat)lua_tointeger(L, -1);
					lua_remove(L, -1);
				}

				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}

bool getArray4d (lua_State* L, int index, GLdouble** arr, int* n1, int* n2, int* n3, int* n4) {

	// Check first dimension
	if (!check4(L, index, n1, n2, n3, n4))
		return false;

	int lim1 = *n1;
	int lim2 = *n2;
	int lim3 = *n3;
	int lim4 = *n4;

	// Allocate storage
	*arr = new GLdouble[lim1 * lim2 * lim3 * lim4];

	for (int i = 0; i < lim1; i++) {
		if (!checkSub(L, index, i, lim2, 1))
			return false;
		
		for (int j = 0; j < lim2; j++) {
			if (!checkSub(L, -1, j, lim3, 2))
				return false;

			for (int k = 0; k < lim3; k++) {
				if (!checkSub(L, -1, k, lim4, 3))
					return false;

				for (int l = 0; l < lim4; l++) {
					lua_rawgeti(L, -1, l + 1);
					(*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (GLdouble)lua_tointeger(L, -1);
					lua_remove(L, -1);
				}

				lua_remove(L, -1);
			}

			lua_remove(L, -1);
		}

		lua_pop(L, 1);
	}

	return true;
}
