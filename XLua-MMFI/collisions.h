#ifndef COLLISIONS_H_
#define COLLISIONS_H_

class SpriteSupport {
public:

	SpriteSupport (LPRH rh, npSpr spr);

	inline int		GetX ();
	inline int		GetY ();
	inline int		GetFlags ();

	inline int		GetAngle ();
	inline void		GetScale (float& sx, float& sy);
	inline float	GetScaleX ();
	inline float	GetScaleY ();

	inline sMask*	GetMask ();
	inline LPBYTE	GetMaskPtr ();
	
	inline LPHO		GetHO ();
	inline LPLO		GetLO ();

	static inline	LPBYTE	GetMaskPtr (sMask* mask);

private:

	LPRH	_rh;
	npSpr	_spr;
	int		_hwa;
};

class Collisions {
public:

	static int	MaskTestPoint	(sMask* mask, int x, int y);
	static int	MaskTestRect	(sMask* mask, int x, int y, int w, int h);
	static int	MaskTestMask	(sMask* mask1, sMask* mask2, int x1, int y1, int x2, int y2);

	static int	SpriteTestPoint	(LPRH rh, fpSpr spr, int x, int y);
	static int	SpriteTestRect	(LPRH rh, fpSpr spr, int x, int y, int w, int h);
	static int	SpriteTestMask	(LPRH rh, fpSpr spr1, sMask* mask2, int x2, int y2);
	static int	SpriteTestSprite (LPRH rh, fpSpr spr1, fpSpr spr2, int x2off = 0, int y2off = 0);

	static void	SpriteListByPoint (GenList& list, LPRH rh, int x, int y, int layer = LAYER_ALL, int flag = 0);
	static void	SpriteListByRect (GenList& list, LPRH rh, int x, int y, int w, int h, int layer = LAYER_ALL, int flag = 0);
	static void SpriteListByMask (GenList& list, LPRH rh, sMask* mask, int x, int y, int layer = LAYER_ALL, int flag = 0);
	static void SpriteListBySprite (GenList& list, LPRH rh, fpSpr spr, int xoff = 0, int yoff = 0, int layer = LAYER_ALL, int flag = 0);

	static void	ObjectListByPoint (GenList& list, LPRH rh, int x, int y, int layer = LAYER_ALL);
	static void ObjectListByRect (GenList& list, LPRH rh, int x, int y, int w, int h, int layer = LAYER_ALL);
	static void ObjectListByMask (GenList& list, LPRH rh, sMask* mask, int x, int y, int layer = LAYER_ALL);
	static void ObjectListBySprite (GenList& list, LPRH rh, fpSpr spr, int xoff = 0, int yoff = 0, int layer = LAYER_ALL);

	static void	ObjectListByPoint (GenList& list, LPRH rh, LPOIL oi, int x, int y, int layer = LAYER_ALL, int comp = SAME_CLASS);
	static void	ObjectListByRect (GenList& list, LPRH rh, LPOIL oi, int x, int y, int w, int h, int layer = LAYER_ALL, int comp = SAME_CLASS);
	static void	ObjectListByMask (GenList& list, LPRH rh, LPOIL oi, sMask* mask, int x, int y, int layer = LAYER_ALL, int comp = SAME_CLASS);
	static void	ObjectListBySprite (GenList& list, LPRH rh, LPOIL oi, npSpr spr, int xoff = 0, int yoff = 0, int layer = LAYER_ALL, int comp = SAME_CLASS);

	static int	ClassTestPoint (LPRH rh, LPOIL oi, int x, int y, int layer = LAYER_ALL, int comp = SAME_CLASS);
	static int	ClassTestRect (LPRH rh, LPOIL oi, int x, int y, int w, int h, int layer = LAYER_ALL, int comp = SAME_CLASS);
	static int	ClassTestMask (LPRH rh, LPOIL oi, sMask* mask, int x, int y, int layer = LAYER_ALL, int comp = SAME_CLASS);
	static int	ClassTestSprite (LPRH rh, LPOIL oi, npSpr spr, int xoff = 0, int yoff = 0, int layer = LAYER_ALL, int comp = SAME_CLASS);

	static void	BackdropListByPoint (GenList& list, LPRH rh, int x, int y, int layer = LAYER_ALL);
	static void BackdropListByRect (GenList& list, LPRH rh, int x, int y, int w, int h, int layer = LAYER_ALL);
	static void BackdropListByMask (GenList& list, LPRH rh, sMask* mask, int x, int y, int layer = LAYER_ALL);
	static void BackdropListBySprite (GenList& list, LPRH rh, fpSpr spr, int xoff = 0, int yoff = 0, int layer = LAYER_ALL);

	static int	BackdropTestPoint (LPRH rh, int x, int y, int layer = LAYER_ALL);
	static int	BackdropTestRect (LPRH rh, int x, int y, int w, int h, int layer = LAYER_ALL);
	static int	BackdropTestSprite (LPRH rh, npSpr spr, int xoff = 0, int yoff = 0, int layer = LAYER_ALL);

	// Filters

	static void	SpritesToObjects (GenList& list, LPRH rh, int layer = LAYER_ALL);
	static void	SpritesToBackdrops (GenList& list, LPRH rh, int layer = LAYER_ALL);

	static void ObjectsMatchClass (GenList& list, LPOIL oi, int comp = SAME_CLASS);

	// Parameter Processing

	//static const int ANY_LAYER	= -1;
	//static const int SAME_LAYER	= -2;
	//static const int DIFF_LAYER	= -3;

	static int	Collisions::LayerCheck (lua_State* L, int index, int layer1 = 0, int layer2 = 0, int layer = ANY_LAYER);
	static int	Collisions::LayerParam (lua_State* L, int index, int layer1 = -1, int layer = ANY_LAYER);
};

#endif
