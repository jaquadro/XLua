#ifndef SURFACE_REF_H_
#define SURFACE_REF_H_

#include "Surface.h"

#include "dll_mmfi/surfaceref.h"

/*class SurfaceRef {
private:

	cSurface*	_surf;
	int*		_refCount;

public:

	SurfaceRef () {
		_surf = new cSurface();
		_refCount = new int(1);
	}

	SurfaceRef (const SurfaceRef& sr) {
		_surf = sr._surf;
		_refCount = sr._refCount;

		(*_refCount)++;
	}

	SurfaceRef& operator= (const SurfaceRef& sr) {
		if (this == &sr)
			return *this;

		(*_refCount)--;
		if (*_refCount <= 0) {
			delete _surf;
			delete _refCount;
		}
		
		_surf = sr._surf;
		_refCount = sr._refCount;

		(*_refCount)++;
		return *this;
	}

	~SurfaceRef () {
		(*_refCount)--;
		if (*_refCount <= 0) {
			delete _surf;
			delete _refCount;
		}
	}

	cSurface& getSurface () {
		return *_surf;
	}
};*/

#endif
