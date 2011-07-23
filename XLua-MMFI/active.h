#ifndef ACTIVE_H_
#define ACTIVE_H_

enum {
	MF_ANY_CLASS	= -1,

	MF_SAME_LAYER	= 0x01,
	MF_ANY_LAYER	= 0x02,
};

class Active {
public:

	// Overrides

	static int	Layer (lua_State* L);
	static int	SetLayer (lua_State* L);

	// State Getters

	static int	Direction (lua_State* L);

	static int	Visible (lua_State* L);

	static int	Angle (lua_State* L);
	static int	ScaleX (lua_State* L);
	static int	ScaleY (lua_State* L);

	static int	AnimationSet (lua_State* L);
	static int	AnimationFrame (lua_State* L);

	static int	ActionX (lua_State* L);
	static int	ActionY (lua_State* L);

	static int	ZOrder (lua_State* L);

	// State Mutators

	static int	SetDirection (lua_State* L);

	static int	SetVisible (lua_State* L);

	static int	SetAngle (lua_State* L);
	static int	SetScaleX (lua_State* L);
	static int	SetScaleY (lua_State* L);

	static int	SetAnimationSet (lua_State* L);
	static int	SetAnimationFrame (lua_State* L);

	// State Queries

	static int	IsAbove (lua_State* L);
	static int	IsBelow (lua_State* L);

	static int	OverlapsBG (lua_State* L);
	static int	OverlapsObject (lua_State* L);
	static int	OverlapsInstance (lua_State* L);

	static int	GetOverlapping (lua_State* L);

	// State Actions

	static int	MoveToFront (lua_State* L);
	static int	MoveToBack (lua_State* L);
	static int	MoveAbove (lua_State* L);
	static int	MoveBelow (lua_State* L);

	static int	SwapOrder (lua_State* L);

	static int	Update (lua_State* L);

public:

	static int	ActiveIndex (lua_State* L, LPRVAL rv);
	static int	ActiveNewIndex (lua_State* L, LPRVAL rv);

	static int	IndexMetamethod (lua_State* L);
	static int	NewIndexMetamethod (lua_State* L);

};

#endif
