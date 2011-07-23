// ============================================================================
//
// This file contains the actions, conditions and expressions your object uses
// 
// ============================================================================

#include "common.h"

// ============================================================================
//
// CONDITIONS
// 
// ============================================================================

CONDITION(
	/* ID */			0,
	/* Name */			"%o: %0 == %1",
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(2,PARAM_NUMBER,"First number",PARAM_NUMBER,"Second number")
) {
	int p1=Param(TYPE_INT);
	int p2=Param(TYPE_INT);
	
	return p1==p2;
}


CONDITION(
	/* ID */			1,
	/* Name */			"%o : On trigger",
	/* Flags */			0,
	/* Params */		(0)
) {
	return true;
}

// ============================================================================
//
// ACTIONS
// 
// ============================================================================

ACTION(
	/* ID */			0,
	/* Name */			"Set angle to %0 (Quality = %1)",
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,"Enter angle in degrees between 0 and 359",PARAM_NUMBER,"Enter 0 (maximum speed), or 1 (maximum quality)")
) {
	int p1 = (int) Param(TYPE_INT);
	int p2 = ((int) Param(TYPE_INT) == 1) ? 1 : 0;

	rdPtr->rc.rcOldAngle = rdPtr->rc.rcAngle;
	rdPtr->rc.rcAngle = p1 % 360;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_ROTATE_ANTIA) | (p2 << 4);
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			1,
	/* Name */			"Set scale to %0 (Quality = %1)",
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,"Enter scale value - 1.0 = normal dimension",PARAM_NUMBER,"Enter 0 (maximum speed), or 1 (maximum quality)")
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = ((int) Param(TYPE_INT) == 1) ? 1 : 0;

	rdPtr->rc.rcScaleX = p1;
	rdPtr->rc.rcScaleY = p1;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_SCALE_RESAMPLE) | (p2 << 3);
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			2,
	/* Name */			"Set X scale to %0 (Quality = %1)",
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,"Enter scale value - 1.0 = normal dimension",PARAM_NUMBER,"Enter 0 (maximum speed), or 1 (maximum quality)")
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = ((int) Param(TYPE_INT) == 1) ? 1 : 0;

	rdPtr->rc.rcScaleX = p1;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_SCALE_RESAMPLE) | (p2 << 3);
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			3,
	/* Name */			"Set Y scale to %0 (Quality = %1)",
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,"Enter scale value - 1.0 = normal dimension",PARAM_NUMBER,"Enter 0 (maximum speed), or 1 (maximum quality)")
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = ((int) Param(TYPE_INT) == 1) ? 1 : 0;

	rdPtr->rc.rcScaleY = p1;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_SCALE_RESAMPLE) | (p2 << 3);
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			4,
	/* Name */			"Set Hot Spot to (%0, %1)",
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,"X coordinate",PARAM_NUMBER,"Y coordinate")
) {
	int p1 = (int) Param(TYPE_INT);
	int p2 = (int) Param(TYPE_INT);

	rdPtr->hotX = p1;
	rdPtr->hotY = p2;
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			5,
	/* Name */			"Set Hot Spot to top-left",
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->hotX = 0;
	rdPtr->hotY = 0;
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			6,
	/* Name */			"Set Hot Spot to top-center",
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->hotX = rdPtr->width / 2;
	rdPtr->hotY = 0;
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			7,
	/* Name */			"Set Hot Spot to top-right",
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->hotX = rdPtr->width;
	rdPtr->hotY = 0;
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			8,
	/* Name */			"Set Hot Spot to left-center",
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->hotX = 0;
	rdPtr->hotY = rdPtr->height / 2;
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			9,
	/* Name */			"Set Hot Spot to center",
	/* Flags */			0,
	/* Params */		(0)
) {
	//int oldHotX = rdPtr->hotX;
	//int oldHotY = rdPtr->hotY;
	rdPtr->hotX = rdPtr->width / 2;
	rdPtr->hotY = rdPtr->height / 2;
	//int mvX = rdPtr->rHo.hoX + (rdPtr->hotX - oldHotX);
	//int mvY = rdPtr->rHo.hoY + (rdPtr->hotY - oldHotY);
	//rdPtr->rHo.hoAdRunHeader->rh4.rh4KpxFunctions[RFUNCTION_SETPOSITION].routine(&(rdPtr->rHo), mvX, mvY);
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			10,
	/* Name */			"Set Hot Spot to right-center",
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->hotX = rdPtr->width;
	rdPtr->hotY = rdPtr->height / 2;
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			11,
	/* Name */			"Set Hot Spot to bottom-left",
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->hotX = 0;
	rdPtr->hotY = rdPtr->height;
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			12,
	/* Name */			"Set Hot Spot to bottom-center",
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->hotX = rdPtr->width / 2;
	rdPtr->hotY = rdPtr->height;
	rdPtr->invalid = 1;
}

ACTION(
	/* ID */			13,
	/* Name */			"Set Hot Spot to bottom-right",
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->hotX = rdPtr->width;
	rdPtr->hotY = rdPtr->height;
	rdPtr->invalid = 1;
}


// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================

EXPRESSION(
	/* ID */			0,
	/* Name */			"Width(",
	/* Flags */			0,
	/* Params */		(0)
) {
	ReturnInteger(rdPtr->width);
}

