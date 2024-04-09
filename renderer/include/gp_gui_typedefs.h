#ifndef _GP_GUI_RENDERER_DEFINITIONS_H_
#define _GP_GUI_RENDERER_DEFINITIONS_H_


/// @brief Disable this flag to remove runtime safety checks and to improve performance
#define _ENABLE_RUNTIME_SAFETY_CHECKS_

/// @brief Enable this flag to do automatic dirty flag management but at the cost of performance
/// @details This flag is useful when you want to do automatic dirty flag management
/// @details disable this flag to do manual dirty flag management for slightly performance improvement
#define _ENABLE_AUTOMATIC_DIRTY_FLAG_MANAGEMENT_

/// @brief inline macro for header only implementation (optional)
#define __INLINE__  


template <typename T>
using const_ptr = const T* const*;

#define GL_NONE_NULL			-1

#ifndef GLAPI
/// If GLAPI is not defined, then define the necessary OpenGL datatypes and constants for Compatibility
/*
 * Datatypes
 */
typedef unsigned int	GLenum;
typedef unsigned char	GLboolean;
typedef unsigned int	GLbitfield;
typedef void		    GLvoid;
typedef signed char	    GLbyte;		/* 1-byte signed */
typedef short		    GLshort;	/* 2-byte signed */
typedef int		        GLint;		/* 4-byte signed */
typedef unsigned char	GLubyte;	/* 1-byte unsigned */
typedef unsigned short	GLushort;	/* 2-byte unsigned */
typedef unsigned int	GLuint;		/* 4-byte unsigned */
typedef int		        GLsizei;	/* 4-byte signed */
typedef float		    GLfloat;	/* single precision float */
typedef float		    GLclampf;	/* single precision float in [0,1] */
typedef double		    GLdouble;	/* double precision float */
typedef double		    GLclampd;	/* double precision float in [0,1] */

/*
 * Constants
*/

/* Boolean values */
#define GL_FALSE				0
#define GL_TRUE					1

#define GL_NONE		         	0
#define GL_LEFT					0x0406
#define GL_RIGHT				0x0407

/* Data types */
#define GL_BYTE					0x1400
#define GL_UNSIGNED_BYTE		0x1401
#define GL_SHORT				0x1402
#define GL_UNSIGNED_SHORT		0x1403
#define GL_INT					0x1404
#define GL_UNSIGNED_INT			0x1405
#define GL_FLOAT				0x1406
#define GL_2_BYTES				0x1407
#define GL_3_BYTES				0x1408
#define GL_4_BYTES				0x1409
#define GL_DOUBLE				0x140A

/* Primitives */
#define GL_POINTS				0x0000
#define GL_LINES				0x0001
#define GL_LINE_LOOP			0x0002
#define GL_LINE_STRIP			0x0003
#define GL_TRIANGLES			0x0004
#define GL_TRIANGLE_STRIP		0x0005
#define GL_TRIANGLE_FAN			0x0006
#define GL_QUADS				0x0007
#define GL_QUAD_STRIP			0x0008
#define GL_POLYGON				0x0009

/* Color Formats */
#define GL_RGB					0x1907
#define GL_RGBA					0x1908

/* Vertex Arrays */
#define GL_VERTEX_ARRAY			0x8074
#define GL_NORMAL_ARRAY			0x8075
#define GL_COLOR_ARRAY			0x8076
#define GL_INDEX_ARRAY			0x8077

/* Shading */
#define GL_SHADE_MODEL			0x0B54
#define GL_FLAT					0x1D00
#define GL_SMOOTH				0x1D01
#define GL_COLOR_MATERIAL		0x0B57
#define GL_AMBIENT				0x1200
#define GL_DIFFUSE				0x1201
#define GL_SPECULAR				0x1202
#define GL_SHININESS			0x1601
#define GL_EMISSION				0x1600

#endif

// Special Layers 

#define GL_LAYER_HIDDEN  -1.0f
#define GL_LAYER_PICKABLE 0.0f
#define GL_LAYER_DEFAULT  1.0f
#define GL_LAYER_MAX      100.0f
#define GL_LAYER_ALL     -100.0f

// Traditional Layers
#define GL_LAYER_1     1.0f
#define GL_LAYER_2     2.0f
#define GL_LAYER_3     3.0f
#define GL_LAYER_4     4.0f
#define GL_LAYER_5     5.0f
#define GL_LAYER_6     6.0f
#define GL_LAYER_7     7.0f
#define GL_LAYER_8     8.0f
#define GL_LAYER_9     9.0f
#define GL_LAYER_10   10.0f
#define GL_LAYER_11   11.0f
#define GL_LAYER_12   12.0f
#define GL_LAYER_13   13.0f
#define GL_LAYER_14   14.0f
#define GL_LAYER_15   15.0f
#define GL_LAYER_16   16.0f
#define GL_LAYER_17   17.0f
#define GL_LAYER_18   18.0f
#define GL_LAYER_19   19.0f
#define GL_LAYER_20   20.0f

// Pick Modes
#define GL_PICK_NONE 0
#define GL_PICK_BY_VERTEX 1
#define GL_PICK_BY_PRIMITIVE 2
#define GL_PICK_GEOMETRY 3

// Wireframe Modes
#define GL_WIREFRAME_NONE 0
#define GL_WIREFRAME_ONLY 1
#define GL_WIREFRAME_OVERLAY 2

// Color Schemes
#define GL_COLOR_PER_VERTEX 0
#define GL_COLOR_PER_PRIMITIVE 1
#define GL_COLOR_PER_PRIMITIVE_SET 2

#endif