#include <windows.h>
#include "surfaceref.h"
#include "ext.h"

BaseSurfaceRef::~BaseSurfaceRef () { }

// -----

AutoSurfaceRef::AutoSurfaceRef () {
	_surf = new cSurface();
	_refCount = new int(1);
}

AutoSurfaceRef::AutoSurfaceRef (const AutoSurfaceRef& sr) {
	_refCount = sr._refCount;
	(*_refCount)++;

	_surf = sr.getSurface();
}

AutoSurfaceRef::~AutoSurfaceRef () {
	(*_refCount)--;
	if (*_refCount == 0) {
		delete _refCount;
		delete _surf;
	}
}

AutoSurfaceRef& AutoSurfaceRef::operator= (const AutoSurfaceRef& sr) {
	if (this == &sr)
		return *this;

	(*_refCount)--;
	if (*_refCount == 0) {
		delete _refCount;
		delete _surf;
	}

	_surf = sr._surf;
	_refCount = sr._refCount;

	(*_refCount)++;
	return *this;
}

AutoSurfaceRef* AutoSurfaceRef::clone () const {
	return new AutoSurfaceRef(*this);
}

cSurface* AutoSurfaceRef::getSurface () const {
	return _surf;
}

bool AutoSurfaceRef::valid () const {
	return true;
}


/*AutoSurfaceRef* AutoSurfaceRef::Create () {
	return new AutoSurfaceRef();
}

void AutoSurfaceRef::Destroy (AutoSurfaceRef* sr) {
	delete sr;
}*/

AutoSurfaceRef* AutoSurfaceRefFactory::create () {
	return new AutoSurfaceRef();
}

void AutoSurfaceRefFactory::destroy (AutoSurfaceRef* sr) {
	delete sr;
}

// -----

ExternSurfaceRef::ExternSurfaceRef () {
	_surf = 0;
}

ExternSurfaceRef::ExternSurfaceRef (cSurface* cs) {
	_surf = cs;
}

ExternSurfaceRef::ExternSurfaceRef (const ExternSurfaceRef& sr) : BaseSurfaceRef() {
	_surf = sr.getSurface();
}

ExternSurfaceRef::~ExternSurfaceRef () { }

ExternSurfaceRef& ExternSurfaceRef::operator= (const ExternSurfaceRef& sr) {
	if (this == &sr)
		return *this;

	_surf = sr._surf;
	return *this;
}

ExternSurfaceRef* ExternSurfaceRef::clone () const {
	return new ExternSurfaceRef(*this);
}

cSurface* ExternSurfaceRef::getSurface () const {
	return _surf;
}

bool ExternSurfaceRef::valid () const {
	return (_surf != 0);
}

void ExternSurfaceRef::bind (cSurface* cs) {
	_surf = cs;
}


/*ExternSurfaceRef* ExternSurfaceRef::Create () {
	return new ExternSurfaceRef();
}

ExternSurfaceRef* ExternSurfaceRef::Create (cSurface* cs) {
	return new ExternSurfaceRef(cs);
}

void ExternSurfaceRef::Destroy (ExternSurfaceRef* sr) {
	delete sr;
}*/

ExternSurfaceRef* ExternSurfaceRefFactory::create () {
	return new ExternSurfaceRef();
}

ExternSurfaceRef* ExternSurfaceRefFactory::create (cSurface* cs) {
	return new ExternSurfaceRef(cs);
}

void ExternSurfaceRefFactory::destroy (ExternSurfaceRef* sr) {
	delete sr;
}

// TEMP!!

SurfaceObjSurfaceRef::SurfaceObjSurfaceRef () {
	_rds = 0;
	_slot = 0;
}

SurfaceObjSurfaceRef::SurfaceObjSurfaceRef (RDSurface* rds, int i) {
	_rds = rds;
	_slot = i;
}

SurfaceObjSurfaceRef::SurfaceObjSurfaceRef (const SurfaceObjSurfaceRef& sr) {
	_rds = sr._rds;
	_slot = sr._slot;
}

SurfaceObjSurfaceRef::~SurfaceObjSurfaceRef () { }

SurfaceObjSurfaceRef& SurfaceObjSurfaceRef::operator= (const SurfaceObjSurfaceRef& sr) {
	if (this == &sr)
		return *this;

	_rds = sr._rds;
	_slot = sr._slot;
	return *this;
}

SurfaceObjSurfaceRef* SurfaceObjSurfaceRef::clone () const {
	return new SurfaceObjSurfaceRef(*this);
}

cSurface* SurfaceObjSurfaceRef::getSurface () const {
	if (!valid())
		return 0;

	return _rds->imageAt(_rds, _slot);
}

bool SurfaceObjSurfaceRef::valid () const {
	if (!_rds)
		return false;

	if (_rds->imageCount(_rds) <= _slot)
		return false;

	return true;
}

void SurfaceObjSurfaceRef::bind (RDSurface* rds, int i) {
	_rds = rds;
	_slot = i;
}


SurfaceObjSurfaceRef* SurfaceObjSurfaceRefFactory::create () {
	return new SurfaceObjSurfaceRef();
}

SurfaceObjSurfaceRef* SurfaceObjSurfaceRefFactory::create (RDSurface* rds) {
	return new SurfaceObjSurfaceRef(rds);
}

void SurfaceObjSurfaceRefFactory::destroy (SurfaceObjSurfaceRef* sr) {
	delete sr;
}


#if 0
SurfaceRef::SurfaceRef () {
	_surf = new cSurface();
	_rec = new _SRRec(1, 0);
}

SurfaceRef::SurfaceRef (cSurface* c) {
	_surf = c;
	_rec = new _SRRec(1, 1);
}

SurfaceRef::SurfaceRef (const SurfaceRef& sr) {
	_surf = sr._surf;
	_rec->_refCount = sr._rec->_refCount;
	_rec->_ext = sr._rec->_ext;

	(_rec->_refCount)++;
}

SurfaceRef& SurfaceRef::operator= (const SurfaceRef& sr) {
	if (this == &sr)
		return *this;

	(_rec->_refCount)--;
	if (_rec->_refCount <= 0) {
		if (!_rec->_ext)
			delete _surf;
		delete _rec;
	}
	
	_surf = sr._surf;
	_rec->_refCount = sr._rec->_refCount;
	_rec->_ext = sr._rec->_ext;

	(_rec->_refCount)++;
	return *this;
}

SurfaceRef::~SurfaceRef () {
	(_rec->_refCount)--;
	if (_rec->_refCount <= 0) {
		if (!_rec->_ext)
			delete _surf;
		delete _rec;
	}
}

cSurface& SurfaceRef::getSurface () {
	return *_surf;
}

bool SurfaceRef::valid () const {
	return (_surf != 0);
}

// ---

SurfaceRef* SurfaceRef::Create () {
	return new SurfaceRef();
}

SurfaceRef* SurfaceRef::Create (cSurface* c) {
	return new SurfaceRef(c);
}

void SurfaceRef::Destroy (SurfaceRef* sr) {
	delete sr;
}
#endif