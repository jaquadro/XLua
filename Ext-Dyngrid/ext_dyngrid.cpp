#include "ext_dyngrid.h"

static int ext_dyngrid_get_grid_width (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	lua_pushinteger(L, grid->gridWidth);
	return 1;
}

static int ext_dyngrid_get_grid_height (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	lua_pushinteger(L, grid->gridHeight);
	return 1;
}

static int ext_dyngrid_set_grid_width (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int sz = lua_tointeger(L, 2);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->gridWidth = sz;
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_set_grid_height (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int sz = lua_tointeger(L, 2);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->gridHeight = sz;
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_get_cell_width (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	lua_pushinteger(L, grid->cellWidth);
	return 1;
}

static int ext_dyngrid_get_cell_height (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	lua_pushinteger(L, grid->cellHeight);
	return 1;
}

static int ext_dyngrid_set_cell_width (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int sz = lua_tointeger(L, 2);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->cellWidth = sz;
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_set_cell_height (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int sz = lua_tointeger(L, 2);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->cellHeight = sz;
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_get_xoffset (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	lua_pushinteger(L, grid->xOffset);
	return 1;
}

static int ext_dyngrid_get_yoffset (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	lua_pushinteger(L, grid->yOffset);
	return 1;
}

static int ext_dyngrid_set_xoffset (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int sz = lua_tointeger(L, 2);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->xOffset = sz;
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_set_yoffset (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int sz = lua_tointeger(L, 2);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->yOffset = sz;
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_get_line_color (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	lua_pushinteger(L, grid->lineColor & 0xFF);
	lua_pushinteger(L, (grid->lineColor & 0xFF00) >> 8);
	lua_pushinteger(L, (grid->lineColor & 0xFF0000) >> 16);

	return 3;
}

static int ext_dyngrid_set_line_color (lua_State* L) {
	if (lua_gettop(L) < 4) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int r = lua_tointeger(L, 2);
	int g = lua_tointeger(L, 3);
	int b = lua_tointeger(L, 4);

	if (r < 0) r = 0; if (r > 255) r = 255;
	if (g < 0) g = 0; if (g > 255) g = 255;
	if (b < 0) b = 0; if (b > 255) b = 255;

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->lineColor = RGB(r, g, b);
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_get_line_weight (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	lua_pushinteger(L, grid->lineWeight);

	return 1;
}

static int ext_dyngrid_set_line_weight (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	int w = lua_tointeger(L, 2);
	if (w < 1) {
		w = 1;
	}

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->lineWeight = w;
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_get_type (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	if (grid->type == 0) {
		lua_pushstring(L, "images");
	}
	else if (grid->type == 1) {
		lua_pushstring(L, "images");
	}
	else {
		return 0;
	}

	return 1;
}


static int ext_dyngrid_use_images (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->type = 0;
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_use_lines (lua_State* L) {
	if (lua_gettop(L) < 1) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	grid->type = 1;
	grid->rebuild = 1;

	return 0;
}

static int ext_dyngrid_component_enabled (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	const char* c = lua_tostring(L, 2);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	if (!strcmp(c, "horizontal")) {
		lua_pushboolean(L, grid->useHLine);
	}
	else if (!strcmp(c, "vertical")) {
		lua_pushboolean(L, grid->useVLine);
	}
	else if (!strcmp(c, "intersection")) {
		lua_pushboolean(L, grid->useIntersect);
	}
	else if (!strcmp(c, "positive")) {
		lua_pushboolean(L, grid->usePosSlope);
	}
	else if (!strcmp(c, "negative")) {
		lua_pushboolean(L, grid->useNegSlope);
	}
	else {
		return 0;
	}

	grid->rebuild = 1;
	return 1;
}

static int ext_dyngrid_enable_component (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	const char* c = lua_tostring(L, 2);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	if (!strcmp(c, "horizontal")) {
		grid->useHLine = true;
	}
	else if (!strcmp(c, "vertical")) {
		grid->useVLine = true;
	}
	else if (!strcmp(c, "intersection")) {
		grid->useIntersect = true;
	}
	else if (!strcmp(c, "positive")) {
		grid->usePosSlope = true;
	}
	else if (!strcmp(c, "negative")) {
		grid->useNegSlope = true;
	}

	grid->rebuild = 1;
	return 0;
}

static int ext_dyngrid_disable_component (lua_State* L) {
	if (lua_gettop(L) < 2) {
		return 0;
	}

	int objid = lua_tointeger(L, 1);
	const char* c = lua_tostring(L, 2);

	EXTDYNGRID* grid = (EXTDYNGRID*) xlua_get_object(L, objid);
	if (!grid) {
		return 0;
	}

	if (!strcmp(c, "horizontal")) {
		grid->useHLine = false;
	}
	else if (!strcmp(c, "vertical")) {
		grid->useVLine = false;
	}
	else if (!strcmp(c, "intersection")) {
		grid->useIntersect = false;
	}
	else if (!strcmp(c, "positive")) {
		grid->usePosSlope = false;
	}
	else if (!strcmp(c, "negative")) {
		grid->useNegSlope = false;
	}

	grid->rebuild = 1;
	return 0;
}

static const luaL_reg ext_dyngridlib[] = {
	{"GetWidth", ext_dyngrid_get_grid_width},
	{"GetHeight", ext_dyngrid_get_grid_height},
	{"SetWidth", ext_dyngrid_set_grid_width},
	{"SetHeight", ext_dyngrid_set_grid_height},
	{"GetCellWidth", ext_dyngrid_get_cell_width},
	{"GetCellHeight", ext_dyngrid_get_cell_height},
	{"SetCellWidth", ext_dyngrid_set_cell_width},
	{"SetCellHeight", ext_dyngrid_set_cell_height},
	{"GetXOffset", ext_dyngrid_get_xoffset},
	{"GetYOffset", ext_dyngrid_get_yoffset},
	{"SetXOffset", ext_dyngrid_set_xoffset},
	{"SetYOffset", ext_dyngrid_set_yoffset},
	{"GetLineColor", ext_dyngrid_get_line_color},
	{"SetLineColor", ext_dyngrid_set_line_color},
	{"GetLineWeight", ext_dyngrid_get_line_weight},
	{"SetLineWeight", ext_dyngrid_set_line_weight},
	{"GetDisplayType", ext_dyngrid_get_type},
	{"UseImages", ext_dyngrid_use_images},
	{"UseLines", ext_dyngrid_use_lines},
	{"ComponentEnabled", ext_dyngrid_component_enabled},
	{"EnableComponent", ext_dyngrid_enable_component},
	{"DisableComponent", ext_dyngrid_disable_component},
	{NULL, NULL}
};

// The name of this routine is critical to the lua package system.
// require "super.sub.lib" would translate to a function named luaopen_super_sub_lib.
// The package would be opened in the table super.sub.lib.

// package ext.dyngrid
extern "C" __declspec(dllexport) int luaopen_ext_dyngrid (lua_State *L) {
  luaL_register(L, "dyngrid", ext_dyngridlib);
  return 1;
}
