#ifndef KC_H_
#define KC_H_

#define	KCARRAY_ISNUMERIC	0x01
#define KCARRAY_ISTEXT		0x02
#define	KCARRAY_BASE0		0x04
#define KCARRAY_GLOBAL		0x08

// Z {
//   Y {
//     X { ... }
//   ... }
// ... }

struct KCArrayData {
	int		xDimension;
	int		yDimension;
	int		zDimension;

	DWORD	flags;

	int		xIndex;
	int		yIndex;
	int		zIndex;

	int		dataLen;
	
	union {
		int		intData[1];
		char*	strData[1];
	};
};

struct KCArray {
	headerObject	ho;
	KCArrayData*	data;
};

#endif