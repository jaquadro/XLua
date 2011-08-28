#ifndef XSDK_ARRAY_H_
#define XSDK_ARRAY_H_

#include "lua.hpp"

namespace XSDK {

	enum XSDataCat {
		TYPE_INTEGER,
		TYPE_REAL,
		TYPE_BOOLEAN,
	};

	static bool _checkDim (lua_State* L, int index, int dim, int* n) {
		lua_rawgeti(L, index, 1);
		if(!lua_istable(L, -1)) {
			lua_pop(L, dim - 1);
			return false;
		}

		if (*n && *n != luaL_getn(L, -1)) {
			lua_pop(L, dim - 1);
			return false;
		}
		else
			*n = luaL_getn(L, -1);

		return true;
	}

	static bool _checkSub (lua_State* L, int index, int i, int n, int depth) {
		lua_rawgeti(L, index, i + 1);
		if(!lua_istable(L, -1) || luaL_getn(L, -1) != n) {
			lua_pop(L, depth);
			return false;
		}

		return true;
	}

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

	template <typename T>
	bool getArray1D (lua_State* L, int index, XSDataCat dcat, T** arr, int* n1) {

		if (*n1 && *n1 != luaL_getn(L, index))
			return false;
		else
			*n1 = luaL_getn(L, index);

		int lim1 = *n1;

		*arr =  new T[lim1];
		for(int i = 0; i < lim1; i++) {
			lua_rawgeti(L, index, i + 1);
			switch (dcat) {
				case TYPE_INTEGER: (*arr)[i] = (T)lua_tointeger(L, -1); break;
				case TYPE_REAL: (*arr)[i] = (T)lua_tonumber(L, -1); break;
				case TYPE_BOOLEAN: (*arr)[i] = (T)lua_toboolean(L, -1); break;
			}
			lua_pop(L, 1);
		}

		return true;
	}

	template <typename T>
	bool getArray2D (lua_State* L, int index, XSDataCat dcat, T** arr, int* n1, int* n2) {

		// Check dimensions
		if (*n1 && *n1 != luaL_getn(L, index))
			return false;
		else
			*n1 = luaL_getn(L, index);

		if (!_checkDim(L, index, 2, n2))
			return false;

		lua_pop(L, 1);

		int lim1 = *n1;
		int lim2 = *n2;

		// Allocate storage
		*arr = new T[lim1 * lim2];

		for (int i = 0; i < lim1; i++)
		{
			if (!_checkSub(L, index, i, lim2, 1))
				return false;

			for (int j = 0; j < lim2; j++) {
				lua_rawgeti(L, -1, j + 1);
				switch (dcat) {
					case TYPE_INTEGER: (*arr)[i*lim2 + j] = (T)lua_tointeger(L, -1); break;
					case TYPE_REAL: (*arr)[i*lim2 + j] = (T)lua_tonumber(L, -1); break;
					case TYPE_BOOLEAN: (*arr)[i*lim2 + j] = (T)lua_toboolean(L, -1); break;
				}
				lua_pop(L, 1);
			}

			lua_pop(L, 1);
		}

		return true;
	}

	template <typename T>
	bool getArray3D (lua_State* L, int index, XSDataCat dcat, T** arr, int* n1, int* n2, int* n3) {

		// Check dimensions
		if (*n1 && *n1 != luaL_getn(L, index))
			return false;
		else
			*n1 = luaL_getn(L, index);

		if (!_checkDim(L, index, 2, n2))
			return false;
		if (!_checkDim(L, -1, 3, n3))
			return false;

		lua_pop(L, 2);

		int lim1 = *n1;
		int lim2 = *n2;
		int lim3 = *n3;

		// Allocate storage
		*arr = new GLbyte[lim1 * lim2 * lim3];

		for (int i = 0; i < lim1; i++) {
			if (!_checkSub(L, index, i, lim2, 1))
				return false;
			
			for (int j = 0; j < lim2; j++) {
				if (!_checkSub(L, -1, j, lim3, 2))
					return false;

				for (int k = 0; k < lim3; k++) {
					lua_rawgeti(L, -1, k + 1);
					switch (dcat) {
						case TYPE_INTEGER: (*arr)[i*lim2*lim3 + j*lim3 + k] = (T)lua_tointeger(L, -1); break;
						case TYPE_REAL: (*arr)[i*lim2*lim3 + j*lim3 + k] = (T)lua_tonumber(L, -1); break;
						case TYPE_BOOLEAN: (*arr)[i*lim2*lim3 + j*lim3 + k] = (T)lua_toboolean(L, -1); break;
					}
					lua_pop(L, 1);
				}

				lua_pop(L, 1);
			}

			lua_pop(L, 1);
		}

		return true;
	}

	template <typename T>
	bool getArray4D (lua_State* L, int index, XSDataCat dcat, T** arr, int* n1, int* n2, int* n3, int* n4) {

		// Check dimensions
		if (*n1 && *n1 != luaL_getn(L, index))
			return false;
		else
			*n1 = luaL_getn(L, index);

		if (!_checkDim(L, index, 2, n2))
			return false;
		if (!_checkDim(L, -1, 3, n3))
			return false;
		if (!_checkDim(L, -1, 4, n4))
			return false;

		lua_pop(L, 3);

		int lim1 = *n1;
		int lim2 = *n2;
		int lim3 = *n3;
		int lim4 = *n4;

		// Allocate storage
		*arr = new GLbyte[lim1 * lim2 * lim3 * lim4];

		for (int i = 0; i < lim1; i++) {
			if (!_checkSub(L, index, i, lim2, 1))
				return false;
			
			for (int j = 0; j < lim2; j++) {
				if (!_checkSub(L, -1, j, lim3, 2))
					return false;

				for (int k = 0; k < lim3; k++) {
					if (!_checkSub(L, -1, k, lim4, 3))
						return false;

					for (int l = 0; l < lim4; l++) {
						lua_rawgeti(L, -1, l + 1);
						switch (dcat) {
							case TYPE_INTEGER: (*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (T)lua_tointeger(L, -1); break;
							case TYPE_REAL: (*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (T)lua_tonumber(L, -1); break;
							case TYPE_BOOLEAN: (*arr)[i*lim2*lim3*lim4 + j*lim3*lim4 + k*lim4 + l] = (T)lua_toboolean(L, -1); break;
						}
						lua_pop(L, 1);
					}

					lua_pop(L, 1);
				}

				lua_pop(L, 1);
			}

			lua_pop(L, 1);
		}

		return true;
	}

}

#endif
