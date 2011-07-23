#include <string.h>
#include <malloc.h>

#include "lauxlib.h"
#include "LuaGL.h"

/* void gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
 *				  GLdouble centerX, GLdouble centerY, GLdouble centerZ,
 *				  GLdouble upX, GLdouble upY, GLdouble upZ);
 *
 * LookAt(ex, ey, ez, cx, cy, cz, ux, uy, uz) -> nil
 */
int glu_look_at(lua_State *L)
{
	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) &&
		  lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6) &&
		  lua_isnumber(L, 7) && lua_isnumber(L, 8) && lua_isnumber(L, 9)))
	  luaL_error(L, "incorrect argument to function 'glu.LookAt'");

	gluLookAt(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3),
			  lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6),
			  lua_tonumber(L, 7), lua_tonumber(L, 8), lua_tonumber(L, 9));

	return 0;
}

/* void gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
 *
 * Ortho2D(left, right, bottom, top) -> nil
 */
int glu_ortho_2d(lua_State *L)
{
	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)))
		luaL_error(L, "incorrect argument to function 'glu.Ortho2D'");

	gluOrtho2D(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));

	return 0;
}

/* void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
 *
 * Perspective(fovy, aspect, znear, zfar)
 */
int glu_perspective(lua_State *L)
{
	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)))
		luaL_error(L, "incorrect argument to function 'glu.Perspective'");

	gluPerspective(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));

	return 0;
}

/* void gluPickMatrix(GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint* viewport)
 *
 * PickMatrix(x, y, width, height)
 */
int glu_pick_matrix(lua_State *L)
{
	GLint viewport[4];

	if (!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)))
		luaL_error(L, "incorrect argument to function 'glu.PickMatrix'");

	glGetIntegerv(GL_VIEWPORT, (GLint*)&viewport);

	gluPickMatrix(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), (GLint*)&viewport);

	return 0;
}


/* GLint gluProject(GLdouble objX, GLdouble objY, GLdouble objZ, 
 *					const GLdouble* model, const GLdouble* proj, const GLint* view,
 *					GLdouble* winX, GLdouble* winY, GLdouble* winZ)
 * 
 * Project(x, y, z) -> wx, wy, wz
 */
int glu_project(lua_State *L)
{
	GLdouble* model;
	GLdouble* proj;
	GLint* view;
	GLdouble winX, winY, winZ;

	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)))
		luaL_error(L, "incorrect argument to function 'glu.Project'");

	model = (GLdouble*)malloc(16 * sizeof(GLdouble));
	proj = (GLdouble*)malloc(16 * sizeof(GLdouble));
	view = (GLint*)malloc(4 * sizeof(GLint));

	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, view);

	if (gluProject((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2), (GLdouble)lua_tonumber(L, 3),
			model, proj, view, &winX, &winY, &winZ) != GLU_TRUE)
		luaL_error(L, "glu.Project encountered an error");

	free(model);
	free(proj);
	free(view);

	lua_pushnumber(L, winX);
	lua_pushnumber(L, winY);
	lua_pushnumber(L, winZ);

	return 3;
}

/* GLint gluUnProject(GLdouble winX, GLdouble winY, GLdouble winZ, 
 *					  const GLdouble* model, const GLdouble* proj, const GLint* view,
 *					  GLdouble* objX, GLdouble* objY, GLdouble* objZ)
 * 
 * UnProject(wx, wy, wz) -> x, y, z
 */
int glu_unproject(lua_State *L)
{
	GLdouble* model;
	GLdouble* proj;
	GLint* view;
	GLdouble objX, objY, objZ;

	if(!(lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)))
		luaL_error(L, "incorrect argument to function 'glu.UnProject'");

	model = (GLdouble*)malloc(16 * sizeof(GLdouble));
	proj = (GLdouble*)malloc(16 * sizeof(GLdouble));
	view = (GLint*)malloc(4 * sizeof(GLint));

	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, view);

	if (gluUnProject((GLdouble)lua_tonumber(L, 1), (GLdouble)lua_tonumber(L, 2), (GLdouble)lua_tonumber(L, 3),
			model, proj, view, &objX, &objY, &objZ) != GLU_TRUE)
		luaL_error(L, "glu.Project encountered an error");

	free(model);
	free(proj);
	free(view);

	lua_pushnumber(L, objX);
	lua_pushnumber(L, objY);
	lua_pushnumber(L, objZ);

	return 3;
}
