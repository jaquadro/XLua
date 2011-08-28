#ifndef DLL_STATIC_H_
#define DLL_STATIC_H_

#include "xlua.h"

enum DataType {
	DATA_BYTE, DATA_SHORT, DATA_LONG, DATA_FLOAT, DATA_DOUBLE
};

static const int DataSize[] = {1, 2, 4, 4, 8};

class StaticArray {
private:

	int elementType;
	int elementSize;
	int elementCount;
	int elementDims;

	int*	dims;
	void*	data;

	bool errorBit;

public:

	StaticArray ()
		: elementType(DATA_DOUBLE), elementSize(8), elementCount(0), elementDims(1), dims(0), data(0) {
	}

	StaticArray (int type, int count)
		: elementType(type), elementSize(0), elementCount(count), elementDims(1), dims(0), data(0), errorBit(0) {
		if (type < 0 || type > 4 || count <= 0) {
			return;
		}

		elementSize = DataSize[type];
		data = malloc(elementSize * elementCount);

		if (!data) {
			elementCount = 0;
			errorBit = true;
			return;
		}

		dims = (int*)malloc(1 * sizeof(int));
		dims[0] = count;

		reset();
	}

	StaticArray (int type, int numDims, int* dimSizes)
		: elementType(type), elementSize(0), elementCount(1), elementDims(numDims), dims(0), data(0), errorBit(0) {
		if (type < 0 || type > 4 || dimSizes == 0) {
			return;
		}
		
		elementSize = DataSize[type];
		for (int i = 0; i < numDims; i++)
			elementCount *= dimSizes[i];

		if (elementCount <= 0)
			return;

		data = malloc(elementSize * elementCount);

		if (!data) {
			elementCount = 0;
			errorBit = true;
			return;
		}
		
		dims = (int*)malloc(numDims * sizeof(int));
		for (int i = 0; i < numDims; i++)
			dims[i] = dimSizes[i];

		reset();
	}

	StaticArray (const StaticArray& sa)
		: elementType(sa.elementType), elementSize(sa.elementSize), elementCount(sa.elementCount), elementDims(sa.elementDims),
		dims(0), data(0) {

		dims = (int*)malloc(elementDims * sizeof(int));
		memcpy(dims, sa.dims, elementDims * sizeof(int));

		data = malloc(elementCount * elementSize);
		if (!data) {
			elementCount = 0;
			errorBit = true;
			return;
		}

		memcpy(data, sa.data, elementCount * elementSize);
	}

	~StaticArray () {
		free(dims);
		free(data);
		dims = 0;
		data = 0;
	}

	StaticArray& operator= (const StaticArray& sa) {
		if (&sa == this)
			return *this;

		errorBit = false;

		elementType = sa.elementType;
		elementSize = sa.elementSize;
		elementCount = sa.elementCount;
		elementDims = sa.elementDims;

		dims = (int*)malloc(elementDims * sizeof(int));
		memcpy(dims, sa.dims, elementDims * sizeof(int));

		data = malloc(elementCount * elementSize);
		if (!data) {
			elementCount = 0;
			errorBit = true;
			return *this;
		}

		memcpy(data, sa.data, elementCount * elementSize);

		return *this;
	}

	void resize (int nsize) {
		if (nsize <= 0 || elementDims > 1) {
			return;
		}

		errorBit = false;

		void* redata = realloc(data, nsize * elementSize);
		if (!redata) {
			errorBit = true;
			return;
		}

		data = redata;
		elementCount = nsize;
		dims[0] = nsize;
	}

	void resize (int szCount, int* nsizes) {
		if (szCount <= 0 || nsizes == 0)
			return;

		errorBit = false;

		int oldSize = elementCount * elementSize;

		int newCount = 1;
		for (int i = 0; i < szCount; i++)
			newCount *= nsizes[i];

		if (newCount <= 0)
			return;

		void* redata = realloc(data, newCount * elementSize);
		if (!redata) {
			errorBit = true;
			return;
		}

		data = redata;
		elementCount = newCount;
		elementDims = szCount;

		dims = (int*)realloc(dims, szCount * sizeof(int));

		for (int i = 0; i < szCount; i++)
			dims[i] = nsizes[i];

		memset((char*)data + oldSize, 0, (elementSize * elementCount) - oldSize);
	}

	void reset () {
		if (data) {
			memset(data, 0, elementSize * elementCount);
		}
	}

	bool valid () const {
		return data != 0;
	}

	bool error () const {
		return errorBit;
	}

	void clearError () {
		errorBit = false;
	}

	int type () const {
		return elementType;
	}

	int size () const {
		return elementSize;
	}

	int dimSize (int idx) const {
		if (idx < 0 || idx >= elementDims)
			return 0;
		return dims[idx];
	}

	int dimCount () const {
		return elementDims;
	}

	int count () const {
		return elementCount;
	}

	const void* raw () const {
		return data;
	}

	char getByte (int idx) const {
		if (idx < 0 || idx >= elementCount) {
			return 0;
		}
		return ((char*)data)[idx];
	}

	char getByte (int* idxs) const {
		return getByte(_calcIndex(idxs));
	}

	short getShort (int idx) const {
		if (idx < 0 || idx >= elementCount) {
			return 0;
		}
		return ((short*)data)[idx];
	}

	short getShort (int* idxs) const {
		return getShort(_calcIndex(idxs));
	}

	long getLong (int idx) const {
		if (idx < 0 || idx >= elementCount) {
			return 0;
		}
		return ((long*)data)[idx];
	}

	long getLong (int* idxs) const {
		return getLong(_calcIndex(idxs));
	}

	float getFloat (int idx) const {
		if (idx < 0 || idx >= elementCount) {
			return 0;
		}
		return ((float*)data)[idx];
	}

	float getFloat (int* idxs) const {
		return getFloat(_calcIndex(idxs));
	}

	double getDouble (int idx) const {
		if (idx < 0 || idx >= elementCount) {
			return 0;
		}
		return ((double*)data)[idx];
	}

	double getDouble (int* idxs) const {
		return getDouble(_calcIndex(idxs));
	}

	void setChar (int idx, char c) {
		if (idx < 0 || idx >= elementCount) {
			return;
		}
		((char*)data)[idx] = c;
	}

	void setChar (int* idxs, char c) {
		setChar(_calcIndex(idxs), c);
	}

	void setShort (int idx, short c) {
		if (idx < 0 || idx >= elementCount) {
			return;
		}
		((short*)data)[idx] = c;
	}

	void setShort (int* idxs, short c) {
		setShort(_calcIndex(idxs), c);
	}

	void setLong (int idx, long c) {
		if (idx < 0 || idx >= elementCount) {
			return;
		}
		((long*)data)[idx] = c;
	}

	void setLong (int* idxs, long c) {
		setLong(_calcIndex(idxs), c);
	}

	void setFloat (int idx, float c) {
		if (idx < 0 || idx >= elementCount) {
			return;
		}
		((float*)data)[idx] = c;
	}

	void setFloat (int* idxs, float c) {
		setFloat(_calcIndex(idxs), c);
	}

	void setDouble (int idx, double c) {
		if (idx < 0 || idx >= elementCount) {
			return;
		}
		((double*)data)[idx] = c;
	}

	void setDouble (int* idxs, double c) {
		setDouble(_calcIndex(idxs), c);
	}

protected:

	int _calcIndex (int* idxs) const {
		int idx = idxs[elementDims - 1];

		for (int i = elementDims - 2, jmp = dims[i]; i >= 0; i--) {
			idx += idxs[i] * jmp;
			jmp *= dims[i];
		}

		return idx;
	}
};

struct ArrayUD {
	StaticArray* arr;
};

static int dll_static_new_array (lua_State* L);
static int dll_static_at (lua_State* L);
static int dll_static_set (lua_State* L);
static int dll_static_reset (lua_State* L);
static int dll_static_destroy (lua_State* L);

#endif