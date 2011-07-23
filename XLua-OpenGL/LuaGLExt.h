#ifndef LUAGLEXT_H_
#define LUAGLEXT_H_

#include <windows.h>
#include <GL/gl.h>

#ifndef GLAPI
#define GLAPI extern
#endif

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
typedef char GLchar;

// EXT_blend_color (1.2)

#define GL_CONSTANT_COLOR								0x8001
#define GL_CONSTANT_COLOR_EXT							0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR						0x8002
#define GL_ONE_MINUS_CONSTANT_COLOR_EXT					0x8002
#define GL_CONSTANT_ALPHA								0x8003
#define GL_CONSTANT_ALPHA_EXT							0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA						0x8004
#define GL_ONE_MINUS_CONSTANT_ALPHA_EXT					0x8004
#define GL_BLEND_COLOR									0x8005
#define GL_BLEND_COLOR_EXT								0x8005

GLAPI void APIENTRY glBlendColorEXT (GLclampf, GLclampf, GLclampf, GLclampf);
typedef void (APIENTRY* PGLBLENDCOLOREXT) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

// EXT_blend_minmax (1.2)

#define GL_FUNC_ADD										0x8006
#define GL_FUNC_ADD_EXT									0x8006
#define GL_MIN											0x8007
#define GL_MIN_EXT										0x8007
#define GL_MAX											0x8008
#define GL_MAX_EXT										0x8008
#define GL_BLEND_EQUATION								0x8009
#define GL_BLEND_EQUATION_EXT							0x8009


GLAPI void APIENTRY glBlendEquationEXT (GLenum);
typedef void (APIENTRY* PGLBLENDEQUATIONEXT) (GLenum mode);

// EXT_blend_subtract (1.2)

#define GL_FUNC_SUBTRACT								0x800A
#define GL_FUNC_SUBTRACT_EXT							0x800A
#define GL_FUNC_REVERSE_SUBTRACT						0x800B
#define GL_FUNC_REVERSE_SUBTRACT_EXT					0x800B

// ARB_multitexture (1.3)

#define GL_TEXTURE0										0x84C0
#define GL_TEXTURE0_ARB									0x84C0
#define GL_TEXTURE1										0x84C1
#define GL_TEXTURE1_ARB									0x84C1
#define GL_TEXTURE2										0x84C2
#define GL_TEXTURE2_ARB									0x84C2
#define GL_TEXTURE3										0x84C3
#define GL_TEXTURE3_ARB									0x84C3
#define GL_TEXTURE4										0x84C4
#define GL_TEXTURE4_ARB									0x84C4
#define GL_TEXTURE5										0x84C5
#define GL_TEXTURE5_ARB									0x84C5
#define GL_TEXTURE6										0x84C6
#define GL_TEXTURE6_ARB									0x84C6
#define GL_TEXTURE7										0x84C7
#define GL_TEXTURE7_ARB									0x84C7
#define GL_TEXTURE8										0x84C8
#define GL_TEXTURE8_ARB									0x84C8
#define GL_TEXTURE9										0x84C9
#define GL_TEXTURE9_ARB									0x84C9
#define GL_TEXTURE10									0x84CA
#define GL_TEXTURE10_ARB								0x84CA
#define GL_TEXTURE11									0x84CB
#define GL_TEXTURE11_ARB								0x84CB
#define GL_TEXTURE12									0x84CC
#define GL_TEXTURE12_ARB								0x84CC
#define GL_TEXTURE13									0x84CD
#define GL_TEXTURE13_ARB								0x84CD
#define GL_TEXTURE14									0x84CE
#define GL_TEXTURE14_ARB								0x84CE
#define GL_TEXTURE15									0x84CF
#define GL_TEXTURE15_ARB								0x84CF
#define GL_TEXTURE16									0x84D0
#define GL_TEXTURE16_ARB								0x84D0
#define GL_TEXTURE17									0x84D1
#define GL_TEXTURE17_ARB								0x84D1
#define GL_TEXTURE18									0x84D2
#define GL_TEXTURE18_ARB								0x84D2
#define GL_TEXTURE19									0x84D3
#define GL_TEXTURE19_ARB								0x84D3
#define GL_TEXTURE20									0x84D4
#define GL_TEXTURE20_ARB								0x84D4
#define GL_TEXTURE21									0x84D5
#define GL_TEXTURE21_ARB								0x84D5
#define GL_TEXTURE22									0x84D6
#define GL_TEXTURE22_ARB								0x84D6
#define GL_TEXTURE23									0x84D7
#define GL_TEXTURE23_ARB								0x84D7
#define GL_TEXTURE24									0x84D8
#define GL_TEXTURE24_ARB								0x84D8
#define GL_TEXTURE25									0x84D9
#define GL_TEXTURE25_ARB								0x84D9
#define GL_TEXTURE26									0x84DA
#define GL_TEXTURE26_ARB								0x84DA
#define GL_TEXTURE27									0x84DB
#define GL_TEXTURE27_ARB								0x84DB
#define GL_TEXTURE28									0x84DC
#define GL_TEXTURE28_ARB								0x84DC
#define GL_TEXTURE29									0x84DD
#define GL_TEXTURE29_ARB								0x84DD
#define GL_TEXTURE30									0x84DE
#define GL_TEXTURE30_ARB								0x84DE
#define GL_TEXTURE31									0x84DF
#define GL_TEXTURE31_ARB								0x84DF
#define GL_ACTIVE_TEXTURE								0x84E0
#define GL_ACTIVE_TEXTURE_ARB							0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE						0x84E1
#define GL_CLIENT_ACTIVE_TEXTURE						0x84E1
#define GL_MAX_TEXTURE_UNITS							0x84E2
#define GL_MAX_TEXTURE_UNITS_ARB						0x84E2

GLAPI void APIENTRY glActiveTextureARB (GLenum);
typedef void (APIENTRY* PGLACTIVETEXTUREARB) (GLenum texture);

GLAPI void APIENTRY glClientActiveTextureARB (GLenum);
typedef void (APIENTRY* PGLCLIENTACTIVETEXTUREARB) (GLenum texture);

GLAPI void APIENTRY glMultiTexCoord1sARB (GLenum, GLshort);
GLAPI void APIENTRY glMultiTexCoord1iARB (GLenum, GLint);
GLAPI void APIENTRY glMultiTexCoord1fARB (GLenum, GLfloat);
GLAPI void APIENTRY glMultiTexCoord1dARB (GLenum, GLdouble);
GLAPI void APIENTRY glMultiTexCoord2sARB (GLenum, GLshort, GLshort);
GLAPI void APIENTRY glMultiTexCoord2iARB (GLenum, GLint, GLint);
GLAPI void APIENTRY glMultiTexCoord2fARB (GLenum, GLfloat, GLfloat);
GLAPI void APIENTRY glMultiTexCoord2dARB (GLenum, GLdouble, GLdouble);
GLAPI void APIENTRY glMultiTexCoord3sARB (GLenum, GLshort, GLshort, GLshort);
GLAPI void APIENTRY glMultiTexCoord3iARB (GLenum, GLint, GLint, GLint);
GLAPI void APIENTRY glMultiTexCoord3fARB (GLenum, GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glMultiTexCoord3dARB (GLenum, GLdouble, GLdouble, GLdouble);
GLAPI void APIENTRY glMultiTexCoord4sARB (GLenum, GLshort, GLshort, GLshort, GLshort);
GLAPI void APIENTRY glMultiTexCoord4iARB (GLenum, GLint, GLint, GLint, GLint);
GLAPI void APIENTRY glMultiTexCoord4fARB (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glMultiTexCoord4dARB (GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
typedef void (APIENTRY* PGLMULTITEXCOORD1SARB) (GLenum target, GLshort s);
typedef void (APIENTRY* PGLMULTITEXCOORD1IARB) (GLenum target, GLint s);
typedef void (APIENTRY* PGLMULTITEXCOORD1FARB) (GLenum target, GLfloat s);
typedef void (APIENTRY* PGLMULTITEXCOORD1DARB) (GLenum target, GLdouble s);
typedef void (APIENTRY* PGLMULTITEXCOORD2SARB) (GLenum target, GLshort s, GLshort t);
typedef void (APIENTRY* PGLMULTITEXCOORD2IARB) (GLenum target, GLint s, GLint t);
typedef void (APIENTRY* PGLMULTITEXCOORD2FARB) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY* PGLMULTITEXCOORD2DARB) (GLenum target, GLdouble s, GLdouble t);
typedef void (APIENTRY* PGLMULTITEXCOORD3SARB) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (APIENTRY* PGLMULTITEXCOORD3IARB) (GLenum target, GLint s, GLint t, GLint r);
typedef void (APIENTRY* PGLMULTITEXCOORD3FARB) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (APIENTRY* PGLMULTITEXCOORD3DARB) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (APIENTRY* PGLMULTITEXCOORD4SARB) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (APIENTRY* PGLMULTITEXCOORD4IARB) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (APIENTRY* PGLMULTITEXCOORD4FARB) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (APIENTRY* PGLMULTITEXCOORD4DARB) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);

GLAPI void APIENTRY glMultiTexCoord1svARB (GLenum, const GLshort*);
GLAPI void APIENTRY glMultiTexCoord1ivARB (GLenum, const GLint*);
GLAPI void APIENTRY glMultiTexCoord1fvARB (GLenum, const GLfloat*);
GLAPI void APIENTRY glMultiTexCoord1dvARB (GLenum, const GLdouble*);
GLAPI void APIENTRY glMultiTexCoord2svARB (GLenum, const GLshort*);
GLAPI void APIENTRY glMultiTexCoord2ivARB (GLenum, const GLint*);
GLAPI void APIENTRY glMultiTexCoord2fvARB (GLenum, const GLfloat*);
GLAPI void APIENTRY glMultiTexCoord2dvARB (GLenum, const GLdouble*);
GLAPI void APIENTRY glMultiTexCoord3svARB (GLenum, const GLshort*);
GLAPI void APIENTRY glMultiTexCoord3ivARB (GLenum, const GLint*);
GLAPI void APIENTRY glMultiTexCoord3fvARB (GLenum, const GLfloat*);
GLAPI void APIENTRY glMultiTexCoord3dvARB (GLenum, const GLdouble*);
GLAPI void APIENTRY glMultiTexCoord4svARB (GLenum, const GLshort*);
GLAPI void APIENTRY glMultiTexCoord4ivARB (GLenum, const GLint*);
GLAPI void APIENTRY glMultiTexCoord4fvARB (GLenum, const GLfloat*);
GLAPI void APIENTRY glMultiTexCoord4dvARB (GLenum, const GLdouble*);
typedef void (APIENTRY* PGLMULTITEXCOORD1SVARB) (GLenum target, const GLshort* v);
typedef void (APIENTRY* PGLMULTITEXCOORD1IVARB) (GLenum target, const GLint* v);
typedef void (APIENTRY* PGLMULTITEXCOORD1FVARB) (GLenum target, const GLfloat* v);
typedef void (APIENTRY* PGLMULTITEXCOORD1DVARB) (GLenum target, const GLdouble* v);
typedef void (APIENTRY* PGLMULTITEXCOORD2SVARB) (GLenum target, const GLshort* v);
typedef void (APIENTRY* PGLMULTITEXCOORD2IVARB) (GLenum target, const GLint* v);
typedef void (APIENTRY* PGLMULTITEXCOORD2FVARB) (GLenum target, const GLfloat* v);
typedef void (APIENTRY* PGLMULTITEXCOORD2DVARB) (GLenum target, const GLdouble* v);
typedef void (APIENTRY* PGLMULTITEXCOORD3SVARB) (GLenum target, const GLshort* v);
typedef void (APIENTRY* PGLMULTITEXCOORD3IVARB) (GLenum target, const GLint* v);
typedef void (APIENTRY* PGLMULTITEXCOORD3FVARB) (GLenum target, const GLfloat* v);
typedef void (APIENTRY* PGLMULTITEXCOORD3DVARB) (GLenum target, const GLdouble* v);
typedef void (APIENTRY* PGLMULTITEXCOORD4SVARB) (GLenum target, const GLshort* v);
typedef void (APIENTRY* PGLMULTITEXCOORD4IVARB) (GLenum target, const GLint* v);
typedef void (APIENTRY* PGLMULTITEXCOORD4FVARB) (GLenum target, const GLfloat* v);
typedef void (APIENTRY* PGLMULTITEXCOORD4DVARB) (GLenum target, const GLdouble* v);

// EXT_blend_func_separate (1.4)

#define GL_BLEND_DST_RGB								0x80C8
#define GL_BLEND_DST_RGB_EXT							0x80C8
#define GL_BLEND_SRC_RGB								0x80C9
#define GL_BLEND_SRC_RGB_EXT							0x80C9
#define GL_BLEND_DST_ALPHA								0x80CA
#define GL_BLEND_DST_ALPHA_EXT							0x80CA
#define GL_BLEND_SRC_ALPHA								0x80CB
#define GL_BLEND_SRC_ALPHA_EXT							0x80CB

GLAPI void APIENTRY glBlendFuncSeparateEXT (GLenum, GLenum, GLenum, GLenum);
typedef void (APIENTRY* PGLBLENDFUNCSEPARATEEXT) (GLenum srcRGB, GLenum destRGB, GLenum srcAlpha, GLenum destAlpha);

// EXT_fog_coord (1.4)

#define GL_FOG_COORD_SRC								0x8450
#define GL_FOG_COORDINATE_SOURCE						0x8450
#define GL_FOG_COORDINATE_SOURCE_EXT					0x8450
#define GL_FOG_COORD									0x8451
#define GL_FOG_COORDINATE								0x8451
#define GL_FOG_COORDINATE_EXT							0x8451
#define GL_FRAGMENT_DEPTH								0x8452
#define GL_FRAGMENT_DEPTH_EXT							0x8452
#define GL_CURRENT_FOG_COORD							0x8453
#define GL_CURRENT_FOG_COORDINATE						0x8453
#define GL_CURRENT_FOG_COORDINATE_EXT					0x8453
#define GL_FOG_COORD_ARRAY_TYPE							0x8454
#define GL_FOG_COORDINATE_ARRAY_TYPE					0x8454
#define GL_FOG_COORDINATE_ARRAY_TYPE_EXT				0x8454
#define GL_FOG_COORD_ARRAY_STRIDE						0x8455
#define GL_FOG_COORDINATE_ARRAY_STRIDE					0x8455
#define GL_FOG_COORDINATE_ARRAY_STRIDE_EXT				0x8455
#define GL_FOG_COORD_ARRAY_POINTER						0x8456
#define GL_FOG_COORDINATE_ARRAY_POINTER					0x8456
#define GL_FOG_COORDINATE_ARRAY_POINTER_EXT				0x8456
#define GL_FOG_COORD_ARRAY								0x8457
#define GL_FOG_COORDINATE_ARRAY							0x8457
#define GL_FOG_COORDINATE_ARRAY_EXT						0x8457

GLAPI void APIENTRY glFogCoorddEXT (GLdouble);
GLAPI void APIENTRY glFogCoordfEXT (GLfloat);
typedef void (APIENTRY* PGLFOGCOORDDEXT) (GLdouble coord);
typedef void (APIENTRY* PGLFOGCOORDFEXT) (GLfloat coord);

GLAPI void APIENTRY glFogCoorddvEXT (GLdouble*);
GLAPI void APIENTRY glFogCoorddfEXT (GLfloat*);
typedef void (APIENTRY* PGLFOGCOORDDVEXT) (GLdouble* coord);
typedef void (APIENTRY* PGLFOGCOORDDFEXT) (GLfloat* coord);

GLAPI void APIENTRY glFogCoordPointerEXT (GLenum, GLsizei, GLvoid*);
typedef void (APIENTRY* PGLFOGCOORDPOINTEREXT) (GLenum type, GLsizei stride, GLvoid* pointer);

// EXT_secondary_color (1.4)

#define GL_COLOR_SUM									0x8458
#define GL_COLOR_SUM_EXT								0x8458
#define GL_CURRENT_SECONDARY_COLOR						0x8459
#define GL_CURRENT_SECONDARY_COLOR_EXT					0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE					0x845A
#define GL_SECONDARY_COLOR_ARRAY_SIZE_EXT				0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE					0x845B
#define GL_SECONDARY_COLOR_ARRAY_TYPE_EXT				0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE					0x845C
#define GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT				0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER				0x845D
#define GL_SECONDARY_COLOR_ARRAY_POINTER_EXT			0x845D
#define GL_SECONDARY_COLOR_ARRAY						0x845E
#define GL_SECONDARY_COLOR_ARRAY_EXT					0x845E

GLAPI void APIENTRY glSecondaryColor3bEXT (GLbyte, GLbyte, GLbyte);
GLAPI void APIENTRY glSecondaryColor3sEXT (GLshort, GLshort, GLshort);
GLAPI void APIENTRY glSecondaryColor3iEXT (GLint, GLint, GLint);
GLAPI void APIENTRY glSecondaryColor3fEXT (GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glSecondaryColor3dEXT (GLdouble, GLdouble, GLdouble);
GLAPI void APIENTRY glSecondaryColor3ubEXT (GLubyte, GLubyte, GLubyte);
GLAPI void APIENTRY glSecondaryColor3usEXT (GLushort, GLushort, GLushort);
GLAPI void APIENTRY glSecondaryColor3uiEXT (GLuint, GLuint, GLuint);
typedef void (APIENTRY* PGLSECONDARYCOLOR3BEXT) (GLbyte red, GLbyte blue, GLbyte green);
typedef void (APIENTRY* PGLSECONDARYCOLOR3SEXT) (GLshort red, GLshort blue, GLshort green);
typedef void (APIENTRY* PGLSECONDARYCOLOR3IEXT) (GLint red, GLint blue, GLint green);
typedef void (APIENTRY* PGLSECONDARYCOLOR3FEXT) (GLfloat red, GLfloat blue, GLfloat green);
typedef void (APIENTRY* PGLSECONDARYCOLOR3DEXT) (GLdouble red, GLdouble blue, GLdouble green);
typedef void (APIENTRY* PGLSECONDARYCOLOR3UBEXT) (GLubyte red, GLubyte blue, GLubyte green);
typedef void (APIENTRY* PGLSECONDARYCOLOR3USEXT) (GLushort red, GLushort blue, GLushort green);
typedef void (APIENTRY* PGLSECONDARYCOLOR3UIEXT) (GLuint red, GLuint blue, GLuint green);

GLAPI void APIENTRY glSecondaryColor3bvEXT (const GLbyte*);
GLAPI void APIENTRY glSecondaryColor3svEXT (const GLshort*);
GLAPI void APIENTRY glSecondaryColor3ivEXT (const GLint*);
GLAPI void APIENTRY glSecondaryColor3fvEXT (const GLfloat*);
GLAPI void APIENTRY glSecondaryColor3dvEXT (const GLdouble*);
GLAPI void APIENTRY glSecondaryColor3ubvEXT (const GLubyte*);
GLAPI void APIENTRY glSecondaryColor3usvEXT (const GLushort*);
GLAPI void APIENTRY glSecondaryColor3uivEXT (const GLuint*);
typedef void (APIENTRY* PGLSECONDARYCOLOR3BVEXT) (const GLbyte* v);
typedef void (APIENTRY* PGLSECONDARYCOLOR3SVEXT) (const GLshort* v);
typedef void (APIENTRY* PGLSECONDARYCOLOR3IVEXT) (const GLint* v);
typedef void (APIENTRY* PGLSECONDARYCOLOR3FVEXT) (const GLfloat* v);
typedef void (APIENTRY* PGLSECONDARYCOLOR3DVEXT) (const GLdouble* v);
typedef void (APIENTRY* PGLSECONDARYCOLOR3UBVEXT) (const GLubyte* v);
typedef void (APIENTRY* PGLSECONDARYCOLOR3USVEXT) (const GLushort* v);
typedef void (APIENTRY* PGLSECONDARYCOLOR3UIVEXT) (const GLuint* v);

GLAPI void APIENTRY glSecondaryColorPointerEXT (GLint, GLenum, GLsizei, GLvoid*);
typedef void (APIENTRY* PGLSECONDARYCOLORPOINTEREXT) (GLint size, GLenum type, GLsizei stride, GLvoid* pointer);

// ARB_vertex_buffer_object (1.5)

#define GL_BUFFER_SIZE									0x8764
#define GL_BUFFER_SIZE_ARB								0x8764
#define GL_BUFFER_USAGE									0x8765
#define GL_BUFFER_USAGE_ARB								0x8765

#define GL_ARRAY_BUFFER									0x8892
#define GL_ARRAY_BUFFER_ARB								0x8892
#define GL_ELEMENT_ARRAY_BUFFER							0x8893
#define GL_ELEMENT_ARRAY_BUFFER_ARB						0x8893
#define GL_ARRAY_BUFFER_BINDING							0x8894
#define GL_ARRAY_BUFFER_BINDING_ARB						0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING					0x8895
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB				0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING					0x8896
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB				0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING					0x8897
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB				0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING					0x8898
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB				0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING					0x8899
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB				0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING			0x889A
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB		0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING				0x889B
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB			0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING			0x889C
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB		0x889C
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING				0x889D
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING			0x889D
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB		0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING					0x889E
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB				0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING			0x889F
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB		0x889F

#define GL_READ_ONLY									0x88B8
#define GL_READ_ONLY_ARB								0x88B8
#define GL_WRITE_ONLY									0x88B9
#define GL_WRITE_ONLY_ARB								0x88B9
#define GL_READ_WRITE									0x88BA
#define GL_READ_WRITE_ARB								0x88BA
#define GL_BUFFER_ACCESS								0x88BB
#define GL_BUFFER_ACCESS_ARB							0x88BB
#define GL_BUFFER_MAPPED								0x88BC
#define GL_BUFFER_MAPPED_ARB							0x88BC
#define GL_BUFFER_MAP_POINTER							0x88BD
#define GL_BUFFER_MAP_POINTER_ARB						0x88BD

#define GL_STREAM_DRAW									0x88E0
#define GL_STREAM_DRAW_ARB								0x88E0
#define GL_STREAM_READ									0x88E1
#define GL_STREAM_READ_ARB								0x88E1
#define GL_STREAM_COPY									0x88E2
#define GL_STREAM_COPY_ARB								0x88E2
#define GL_STATIC_DRAW									0x88E4
#define GL_STATIC_DRAW_ARB								0x88E4
#define GL_STATIC_READ									0x88E5
#define GL_STATIC_READ_ARB								0x88E5
#define GL_STATIC_COPY									0x88E6
#define GL_STATIC_COPY_ARB								0x88E6
#define GL_DYNAMIC_DRAW									0x88E8
#define GL_DYNAMIC_DRAW_ARB								0x88E8
#define GL_DYNAMIC_READ									0x88E9
#define GL_DYNAMIC_READ_ARB								0x88E9
#define GL_DYNAMIC_COPY									0x88EA
#define GL_DYNAMIC_COPY_ARB								0x88EA

GLAPI void APIENTRY glBindBufferARB (GLenum, GLuint);
typedef void (APIENTRY* PGLBINDBUFFERARB) (GLenum target, GLuint buffer);

GLAPI void APIENTRY glBufferDataARB (GLenum, GLsizeiptr, const GLvoid*, GLenum);
typedef void (APIENTRY* PGLBUFFERDATAARB) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);

GLAPI void APIENTRY glBufferSubDataARB (GLenum, GLintptr, GLsizeiptr, const GLvoid*);
typedef void (APIENTRY* PGLBUFFERSUBDATAARB) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);

GLAPI void APIENTRY glDeleteBuffersARB (GLsizei, const GLuint*);
typedef void (APIENTRY* PGLDELETEBUFFERSARB) (GLsizei n, const GLuint* buffers);

GLAPI void APIENTRY glGenBuffersARB (GLsizei, GLuint*);
typedef void (APIENTRY* PGLGENBUFFERSARB) (GLsizei n, GLuint* buffers);

GLAPI void APIENTRY glGetBufferParameterivARB (GLenum, GLenum, GLint*);
typedef void (APIENTRY* PGLGETBUFFERPARAMETERIVARB) (GLenum target, GLenum value, GLint* data);

GLAPI void APIENTRY glGetBufferPointervARB (GLenum, GLenum, GLvoid**);
typedef void (APIENTRY* PGLGETBUFFERPOINTERVARB) (GLenum target, GLenum pname, GLvoid** params);

GLAPI void APIENTRY glGetBufferSubDataARB (GLenum, GLintptr, GLsizeiptr, const GLvoid*);
typedef void (APIENTRY* PGLGETBUFFERSUBDATAARB) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);

GLAPI GLboolean APIENTRY glIsBufferARB (GLuint);
typedef GLboolean (APIENTRY* PGLISBUFFERARB) (GLuint buffer);

GLAPI void* APIENTRY glMapBufferARB (GLenum, GLenum);
typedef void* (APIENTRY* PGLMAPBUFFERARB) (GLenum target, GLenum access);

GLAPI GLboolean APIENTRY glUnmapBufferARB (GLenum);
typedef void (APIENTRY* PGLUNMAPBUFFERARB) (GLenum target);

// EXT_blend_equation_separate (2.0)

#define GL_BLEND_EQUATION_RGB							0x8009
#define GL_BLEND_EQUATION_RGB_EXT						0x8009
#define GL_BLEND_EQUATION_ALPHA							0x883D
#define GL_BLEND_EQUATION_ALPHA_EXT						0x883D

GLAPI void APIENTRY glBlendEquationSeparateEXT (GLenum, GLenum);
typedef void (APIENTRY* PGLBLENDEQUATIONSEPARATEEXT) (GLenum modeRGB, GLenum modeAlpha);

// ARB_fragment_shader (2.0)

#define GL_FRAGMENT_SHADER								0x8B30
#define GL_FRAGMENT_SHADER_ARB							0x8B30

#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS				0x8B49
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB			0x8B49

#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT				0x8B8B
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB			0x8B8B

// ARB_vertex_shader (2.0)

#define GL_VERTEX_SHADER								0x8B31
#define GL_VERTEX_SHADER_ARB							0x8B31

#define GL_MAX_VERTEX_UNIFORM_COMPONENTS				0x8B4A
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB			0x8B4A
#define GL_MAX_VARYING_FLOATS							0x8B4B
#define GL_MAX_VARYING_FLOATS_ARB						0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS				0x8B4C
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB			0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS				0x8B4D
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB			0x8B4D

#define GL_ACTIVE_ATTRIBUTES							0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB					0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH					0x8B8A
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB		0x8B8A

// ARB_shader_objects (2.0)

#define GL_PROGRAM_OBJECT_ARB							0x8B40
#define GL_SHADER_OBJECT_ARB							0x8B48

#define GL_OBJECT_TYPE_ARB								0x8B4E
#define GL_SHADER_TYPE									0x8B4F
#define GL_OBJECT_SUBTYPE_ARB							0x8B4F
#define GL_FLOAT_VEC2									0x8B50
#define GL_FLOAT_VEC2_ARB								0x8B50
#define GL_FLOAT_VEC3									0x8B51
#define GL_FLOAT_VEC3_ARB								0x8B51
#define GL_FLOAT_VEC4									0x8B52
#define GL_FLOAT_VEC4_ARB								0x8B52
#define GL_INT_VEC2										0x8B53
#define GL_INT_VEC2_ARB									0x8B53
#define GL_INT_VEC3										0x8B54
#define GL_INT_VEC3_ARB									0x8B54
#define GL_INT_VEC4										0x8B55
#define GL_INT_VEC4_ARB									0x8B55
#define GL_BOOL											0x8B56
#define GL_BOOL_ARB										0x8B56
#define GL_BOOL_VEC2									0x8B57
#define GL_BOOL_VEC2_ARB								0x8B57
#define GL_BOOL_VEC3									0x8B58
#define GL_BOOL_VEC3_ARB								0x8B58
#define GL_BOOL_VEC4									0x8B59
#define GL_BOOL_VEC4_ARB								0x8B59
#define GL_FLOAT_MAT2									0x8B5A
#define GL_FLOAT_MAT2_ARB								0x8B5A
#define GL_FLOAT_MAT3									0x8B5B
#define GL_FLOAT_MAT3_ARB								0x8B5B
#define GL_FLOAT_MAT4									0x8B5C
#define GL_FLOAT_MAT4_ARB								0x8B5C
#define GL_SAMPLER_1D									0x8B5D
#define GL_SAMPLER_1D_ARB								0x8B5D
#define GL_SAMPLER_2D									0x8B5E
#define GL_SAMPLER_2D_ARB								0x8B5E
#define GL_SAMPLER_3D									0x8B5F
#define GL_SAMPLER_3D_ARB								0x8B5F
#define GL_SAMPLER_CUBE									0x8B60
#define GL_SAMPLER_CUBE_ARB								0x8B60
#define GL_SAMPLER_1D_SHADOW							0x8B61
#define GL_SAMPLER_1D_SHADOW_ARB						0x8B61
#define GL_SAMPLER_2D_SHADOW							0x8B62
#define GL_SAMPLER_2D_SHADOW_ARB						0x8B62
#define GL_SAMPLER_2D_RECT_ARB							0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB					0x8B64
#define GL_FLOAT_MAT2x3									0x8B65
#define GL_FLOAT_MAT2x4									0x8B66
#define GL_FLOAT_MAT3x2									0x8B67
#define GL_FLOAT_MAT3x4									0x8B68
#define GL_FLOAT_MAT4x2									0x8B69
#define GL_FLOAT_MAT4x3									0x8B6A

#define GL_DELETE_STATUS								0x8B80
#define GL_OBJECT_DELETE_STATUS_ARB						0x8B80
#define GL_COMPILE_STATUS								0x8B81
#define GL_OBJECT_COMPILE_STATUS_ARB					0x8B81
#define GL_LINK_STATUS									0x8B82
#define GL_OBJECT_LINK_STATUS_ARB						0x8B82
#define GL_VALIDATE_STATUS								0x8B83
#define GL_OBJECT_VALIDATE_STATUS_ARB					0x8B83
#define GL_INFO_LOG_LENGTH								0x8B84
#define GL_OBJECT_INFO_LOG_LENGTH_ARB					0x8B84
#define GL_ATTACHED_SHADERS								0x8B85
#define GL_OBJECT_ATTACHED_SHADERS_ARB					0x8B85
#define GL_ACTIVE_UNIFORMS								0x8B86
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB					0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH					0x8B87
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB			0x8B87
#define GL_SHADER_SOURCE_LENGTH							0x8B88
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB				0x8B88

// ARB_shading_language_100 (2.0)

#define GL_SHADING_LANGUAGE_VERSION						0x8B8C
#define GL_SHADING_LANGUAGE_VERSION_ARB					0x8B8C

GLAPI void APIENTRY glAttachShader (GLuint, GLuint);
typedef void (APIENTRY* PGLATTACHSHADER) (GLuint program, GLuint shader);

GLAPI GLuint APIENTRY glCompileShader (GLuint);
typedef GLuint (APIENTRY* PGLCOMPILESHADER) (GLenum shader);

GLAPI GLuint APIENTRY glCreateProgram (void);
typedef GLuint (APIENTRY* PGLCREATEPROGRAM) (void);

GLAPI GLuint APIENTRY glCreateShader (GLuint);
typedef GLuint (APIENTRY* PGLCREATESHADER) (GLenum shaderType);

GLAPI void APIENTRY glDeleteProgram (GLuint);
typedef void (APIENTRY* PGLDELETEPROGRAM) (GLuint program);

GLAPI void APIENTRY glDeleteShader (GLuint);
typedef void (APIENTRY* PGLDELETESHADER) (GLuint shader);

GLAPI void APIENTRY glDetachShader (GLuint, GLuint);
typedef void (APIENTRY* PGLDETACHSHADER) (GLuint program, GLuint shader);

GLAPI void APIENTRY glGetActiveAttrib (GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar*);
typedef void (APIENTRY* PGLGETACTIVEATTRIB) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);

GLAPI void APIENTRY glGetActiveUniform (GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar*);
typedef void (APIENTRY* PGLGETACTIVEUNIFORM) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);

GLAPI void APIENTRY glGetAttachedShaders (GLuint, GLsizei, GLsizei*, GLuint*);
typedef void (APIENTRY* PGLGETATTACHEDSHADERS) (GLuint program, GLsizei maxCount, GLsizei* count , GLuint* shaders);

GLAPI void APIENTRY glGetAttribLocation (GLuint, const GLchar*);
typedef void (APIENTRY* PGLGETATTRIBLOCATION) (GLuint program, const GLchar* name);

GLAPI void APIENTRY glGetProgramiv (GLuint, GLenum, GLint*);
typedef void (APIENTRY* PGLGETPROGRAMIV) (GLuint program, GLenum pname, GLint* params);

GLAPI void APIENTRY glGetProgramInfoLog (GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (APIENTRY* PGLGETPROGRAMINFOLOG) (GLuint shader, GLsizei maxLength, GLsizei* length , GLchar* infoLog);

GLAPI void APIENTRY glGetShaderiv (GLuint, GLenum, GLint*);
typedef void (APIENTRY* PGLGETSHADERIV) (GLuint shader, GLenum pname, GLint* params);

GLAPI void APIENTRY glGetShaderInfoLog (GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (APIENTRY* PGLGETSHADERINFOLOG) (GLuint shader, GLsizei maxLength, GLsizei* length , GLchar* infoLog);

GLAPI void APIENTRY glGetShaderSource (GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (APIENTRY* PGLGETSHADERSOURCE) (GLuint shader, GLsizei bufSize, GLsizei* length , GLchar* source);

GLAPI void APIENTRY glGetUniformfv (GLuint, GLint, GLfloat*);
GLAPI void APIENTRY glGetUniformiv (GLuint, GLint, GLint*);
typedef void (APIENTRY* PGLGETUNIFORMFV) (GLuint program, GLint location, GLfloat* params);
typedef void (APIENTRY* PGLGETUNIFORMIV) (GLuint program, GLint location, GLint* params);

GLAPI GLint APIENTRY glGetUniformLocation (GLuint, const GLchar*);
typedef GLint (APIENTRY* PGLGETUNIFORMLOCATION) (GLuint program, const GLchar* name);

GLAPI GLboolean APIENTRY glIsProgram (GLuint);
typedef GLboolean (APIENTRY* PGLISPROGRAM) (GLuint program);

GLAPI GLboolean APIENTRY glIsShader (GLuint);
typedef GLboolean (APIENTRY* PGLISSHADER) (GLuint shader);

GLAPI GLuint APIENTRY glLinkProgram (GLuint);
typedef GLuint (APIENTRY* PGLLINKPROGRAM) (GLuint program);

GLAPI void APIENTRY glShaderSource (GLuint, GLsizei, const GLchar**, const GLint*);
typedef void (APIENTRY* PGLSHADERSOURCE) (GLuint shader, GLsizei count, const GLchar** string, const GLint* length);

GLAPI void APIENTRY glUniform1f (GLint, GLfloat);
GLAPI void APIENTRY glUniform2f (GLint, GLfloat, GLfloat);
GLAPI void APIENTRY glUniform3f (GLint, GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glUniform4f (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glUniform1i (GLint, GLint);
GLAPI void APIENTRY glUniform2i (GLint, GLint, GLint);
GLAPI void APIENTRY glUniform3i (GLint, GLint, GLint, GLint);
GLAPI void APIENTRY glUniform4i (GLint, GLint, GLint, GLint, GLint);
typedef void (APIENTRY* PGLUNIFORM1F) (GLint location, GLfloat v0);
typedef void (APIENTRY* PGLUNIFORM2F) (GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY* PGLUNIFORM3F) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY* PGLUNIFORM4F) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY* PGLUNIFORM1I) (GLint location, GLint v0);
typedef void (APIENTRY* PGLUNIFORM2I) (GLint location, GLint v0, GLint v1);
typedef void (APIENTRY* PGLUNIFORM3I) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (APIENTRY* PGLUNIFORM4I) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

GLAPI void APIENTRY glUniform1fv (GLint, GLsizei, const GLfloat*);
GLAPI void APIENTRY glUniform2fv (GLint, GLsizei, const GLfloat*);
GLAPI void APIENTRY glUniform3fv (GLint, GLsizei, const GLfloat*);
GLAPI void APIENTRY glUniform4fv (GLint, GLsizei, const GLfloat*);
GLAPI void APIENTRY glUniform1iv (GLint, GLsizei, const GLint*);
GLAPI void APIENTRY glUniform2iv (GLint, GLsizei, const GLint*);
GLAPI void APIENTRY glUniform3iv (GLint, GLsizei, const GLint*);
GLAPI void APIENTRY glUniform4iv (GLint, GLsizei, const GLint*);
typedef void (APIENTRY* PGLUNIFORM1FV) (GLint location, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORM2FV) (GLint location, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORM3FV) (GLint location, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORM4FV) (GLint location, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORM1IV) (GLint location, GLsizei count, GLint* value);
typedef void (APIENTRY* PGLUNIFORM2IV) (GLint location, GLsizei count, GLint* value);
typedef void (APIENTRY* PGLUNIFORM3IV) (GLint location, GLsizei count, GLint* value);
typedef void (APIENTRY* PGLUNIFORM4IV) (GLint location, GLsizei count, GLint* value);

GLAPI void APIENTRY glUniformMatrix2fv (GLint, GLsizei, GLboolean, const GLfloat*);
GLAPI void APIENTRY glUniformMatrix3fv (GLint, GLsizei, GLboolean, const GLfloat*);
GLAPI void APIENTRY glUniformMatrix4fv (GLint, GLsizei, GLboolean, const GLfloat*);
GLAPI void APIENTRY glUniformMatrix2x3fv (GLint, GLsizei, GLboolean, const GLfloat*);
GLAPI void APIENTRY glUniformMatrix3x2fv (GLint, GLsizei, GLboolean, const GLfloat*);
GLAPI void APIENTRY glUniformMatrix2x4fv (GLint, GLsizei, GLboolean, const GLfloat*);
GLAPI void APIENTRY glUniformMatrix4x2fv (GLint, GLsizei, GLboolean, const GLfloat*);
GLAPI void APIENTRY glUniformMatrix3x4fv (GLint, GLsizei, GLboolean, const GLfloat*);
GLAPI void APIENTRY glUniformMatrix4x3fv (GLint, GLsizei, GLboolean, const GLfloat*);
typedef void (APIENTRY* PGLUNIFORMMATRIX2FV) (GLint location, GLboolean transpose, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORMMATRIX3FV) (GLint location, GLboolean transpose, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORMMATRIX4FV) (GLint location, GLboolean transpose, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORMMATRIX2x3FV) (GLint location, GLboolean transpose, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORMMATRIX3x2FV) (GLint location, GLboolean transpose, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORMMATRIX2x4FV) (GLint location, GLboolean transpose, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORMMATRIX4x2FV) (GLint location, GLboolean transpose, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORMMATRIX3x4FV) (GLint location, GLboolean transpose, GLsizei count, GLfloat* value);
typedef void (APIENTRY* PGLUNIFORMMATRIX4x3FV) (GLint location, GLboolean transpose, GLsizei count, GLfloat* value);

GLAPI void APIENTRY glUseProgram (GLuint);
typedef void (APIENTRY* PGLUSEPROGRAM) (GLuint program);

GLAPI void APIENTRY glValidateProgram (GLuint);
typedef void (APIENTRY* PGLVALIDATEPROGRAM) (GLuint program);

// ARB_vertex_program (2.0)

#define GL_MAX_VERTEX_ATTRIBS							0x8869
#define GL_MAX_VERTEX_ATTRIBS_ARB						0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED				0x886A
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB			0x886A

GLAPI void APIENTRY glBindAttribLocation (GLuint, GLuint, const GLchar*);
typedef void (APIENTRY* PGLBINDATTRIBLOCATION) (GLuint program, GLuint index, const GLchar* name);

// ARB_fragment_program (2.0)

#define GL_MAX_TEXTURE_COORDS							0x8871
#define GL_MAX_TEXTURE_COORDS_ARB						0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS						0x8872
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB					0x8872
#define GL_PROGRAM_ERROR_STRING_ARB						0x8874
#define GL_PROGRAM_FORMAT_ASCII_ARB						0x8875
#define GL_PROGRAM_FORMAT_ARB							0x8876

#endif