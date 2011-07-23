#ifndef XSDK_SURFACE_REF_H_
#define XSDK_SURFACE_REF_H_

#include	"xsdk.h"
#include	"surface.h"

/**
 * BaseSurfaceRef
 *
 * BaseSurfaceRefs are an abstract representation of surface references, and most
 * SDK structures and opreations deal with BSFs to allow them to operate under a
 * variety of surface reference semantics.  Any new reference semantics must
 * inherit from BaseSurfaceRef.
 *
 * Reference implementations are not interchangable.  Any copy or assignment of
 * concrete references must be to references of the same type, therefor as an
 * example, it would be impossible to directly copy an ExternSurfaceRef to an
 * AutoSurfaceRef, even if both are held as BaseSurfaceRefs.
 */

class XAPI BaseSurfaceRef {
public:

	virtual ~BaseSurfaceRef ();

	virtual BaseSurfaceRef*	clone () const = 0;

	virtual	cSurface*	getSurface () const = 0;
	virtual	bool		valid () const = 0;
};

class XAPI BaseSurfaceRefFactory {
public:

	virtual BaseSurfaceRef*	create () = 0;
	//virtual void			destroy (BaseSurfaceRef* sr) = 0;
};

/**
 * AutoSurfaceRef
 *
 * AutoSurfaceRefs create and own cSurface resources, so it is always safe to access
 * the surface of an AutoSurfaceRef.  The cSurface will not be released until all
 * ASRs holding a reference to the resource have been deleted.  ASRs can never be
 * bound to a new cSurface resource.
 */

class XAPI AutoSurfaceRef : public BaseSurfaceRef {
protected:

	cSurface*	_surf;
	int*		_refCount;

public:

	AutoSurfaceRef ();
	AutoSurfaceRef (const AutoSurfaceRef& sr);

	~AutoSurfaceRef ();

	AutoSurfaceRef& operator= (const AutoSurfaceRef& sr);

	AutoSurfaceRef* clone () const;

	cSurface*	getSurface() const;
	bool		valid () const;

	// Static wrappers to keep memory management inside DLL

	//static AutoSurfaceRef*	Create ();
	//static void				Destroy (AutoSurfaceRef* sr);
};

class XAPI AutoSurfaceRefFactory : public BaseSurfaceRefFactory {
public:

	AutoSurfaceRef*	create ();
	void			destroy (AutoSurfaceRef* sr);
};

/**
 * ExternSurfaceRef
 *
 * ExternSurfaceRefs hold a reference to an externally created cSurface resource.
 * ESRs can also be rebound to a different cSurface resource at any time.  The user
 * is entirely responsible for ensuring that the backing cSurface resource is valid
 * for the entire time that any ESR is holding a reference to the resource, or
 * behavior is undefined (read: your app will crash).  ESRs can be bound to the
 * null pointer (0) to flag them as invalid and safe for any operation that checks
 * the valid() status.
 */

class XAPI ExternSurfaceRef : public BaseSurfaceRef {
protected:

	cSurface*	_surf;

public:

	ExternSurfaceRef ();
	ExternSurfaceRef (cSurface* cs);
	ExternSurfaceRef (const ExternSurfaceRef& sr);

	~ExternSurfaceRef ();

	ExternSurfaceRef& operator= (const ExternSurfaceRef& sr);

	ExternSurfaceRef* clone () const;

	cSurface*	getSurface () const;
	bool		valid () const;

	void		bind (cSurface* cs);

	// Static wrappers to keep memory management inside DLL

	//static ExternSurfaceRef*	Create ();
	//static ExternSurfaceRef*	Create (cSurface* cs);
	//static void					Destroy (ExternSurfaceRef* sr);
};

class XAPI ExternSurfaceRefFactory : public BaseSurfaceRefFactory {
public:

	ExternSurfaceRef*	create ();
	ExternSurfaceRef*	create (cSurface* cs);
	void				destroy (ExternSurfaceRef* sr);
};

// Temp

struct RDSurface;

class SurfaceObjSurfaceRef : public BaseSurfaceRef {
protected:

	RDSurface*	_rds;
	int			_slot;

public:

	SurfaceObjSurfaceRef ();
	SurfaceObjSurfaceRef (RDSurface* rds, int i = 0);
	SurfaceObjSurfaceRef (const SurfaceObjSurfaceRef& sr);

	~SurfaceObjSurfaceRef ();

	SurfaceObjSurfaceRef& operator= (const SurfaceObjSurfaceRef& sr);

	SurfaceObjSurfaceRef* clone () const;

	cSurface*	getSurface () const;
	bool		valid () const;

	void		bind (RDSurface* rds, int i = 0);
};

class SurfaceObjSurfaceRefFactory : public BaseSurfaceRefFactory {
public:

	SurfaceObjSurfaceRef*	create ();
	SurfaceObjSurfaceRef*	create (RDSurface* rds);
	void					destroy (SurfaceObjSurfaceRef* sr);
};

#if 0
class XAPI SurfaceRef {
private:

	class _SRRec {
	public:
		int		_refCount;
		int		_ext;

		_SRRec (int refCount, int ext) : _refCount(refCount), _ext(ext) { }
	};

	cSurface*	_surf;
	_SRRec*		_rec;

public:

	SurfaceRef ();
	SurfaceRef (cSurface* c);
	SurfaceRef (const SurfaceRef& sr);

	SurfaceRef& operator= (const SurfaceRef& sr);

	~SurfaceRef ();

	cSurface&	getSurface ();
	bool		valid () const;

	// Static wrappers to keep memory management inside DLL

	static SurfaceRef*	Create ();
	static SurfaceRef*	Create (cSurface* c);
	static void			Destroy (SurfaceRef* sr);
};
#endif

#endif
