#pragma once
#ifndef _GP_GUI_GEOMETRY_DESCRIPTOR_DRAWABLE_H_
#define _GP_GUI_GEOMETRY_DESCRIPTOR_DRAWABLE_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

/// @brief Disable this flag to remove runtime safety checks and to improve performance
#define _ENABLE_RUNTIME_SAFETY_CHECKS_

/// @brief Enable this flag to do automatic dirty flag management but at the cost of performance
/// @details This flag is useful when you want to do automatic dirty flag management
/// @details disable this flag to do manual dirty flag management for slightly performance improvement
#define _ENABLE_AUTOMATIC_DIRTY_FLAG_MANAGEMENT_

/// @brief inline macro for cross-platform compatibility
#define __INLINE__ inline 

#ifndef GLAPI
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

#define GL_NONE					0
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

/*
 * Functions
 */

/// @brief   An Inheritable Class for drawable objects ranging from simple points to complex 3D models
/// @details This class is used to store and manage drawable objects, including points, lines, triangles, and other primitives.
/// @details The class is designed to be used with OpenGL, but it can be used with other rendering systems as well.
/// Provides a standard interface for rendering engines to use.
/// Currently, the class supports only vertex positions, normals, and colors. Texture coordinates and other attributes can be added in the future.

class GeometryDescriptor 
{
public:
    struct PrimitiveSetInstance {
        /// @brief Enumeration for primitive types
        enum PrimitiveType {
            NONE           = GL_NONE,
            POINTS         = GL_POINTS,
            LINES          = GL_LINES,
            LINE_STRIP     = GL_LINE_STRIP,
            LINE_LOOP      = GL_LINE_LOOP,
            TRIANGLES      = GL_TRIANGLES,
            TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
            TRIANGLE_FAN   = GL_TRIANGLE_FAN,
            QUADS          = GL_QUADS,
            QUAD_STRIP     = GL_QUAD_STRIP,
            POLYGON        = GL_POLYGON
        };

        /// @brief Enumeration for color formats
        enum ColorFormat {
            RGB  = GL_RGB,
            RGBA = GL_RGBA
        };

        /// @brief Enumeration for color schemes        
        enum ColorScheme {
            PER_PRIMITIVE_SET = 0,
            PER_PRIMITIVE = 2,
            PER_VERTEX = 1 
        };

        /// @brief  Enumeration for Shading Model
        enum ShadingModel {
            FLAT   = GL_FLAT,
            SMOOTH = GL_SMOOTH
        };
       
        /// @brief  Enumeration for Material Properties
        enum MaterialProperty {
            AMBIENT   = GL_AMBIENT,
            DIFFUSE   = GL_DIFFUSE,
            SPECULAR  = GL_SPECULAR,
            SHININESS = GL_SHININESS,
            EMISSION  = GL_EMISSION,
            COLOR_MATERIAL = GL_COLOR_MATERIAL
        };

        /// @brief  Enumeration for Pick Schemes
        enum PickScheme {
            PICK_NONE = 0,
            PICK_BY_VERTEX = 1,
            PICK_BY_PRIMITIVE = 2,
            PICK_GEOMETRY = 3
        };

        /// @brief  Enumeration for vertex array types
        enum VertexArrayType {
            POSITION_ARRAY = GL_VERTEX_ARRAY,
            NORMAL_ARRAY   = GL_NORMAL_ARRAY,
            COLOR_ARRAY    = GL_COLOR_ARRAY,
            INDEX_ARRAY    = GL_INDEX_ARRAY
        };

        const std::string   InstanceName; /// @brief Name of the primitive set instance
        const PrimitiveType primitiveType; /// @brief Primitive type (e.g., GL_TRIANGLES, GL_LINES, etc.)

        ColorFormat   colorFormat; /// @brief RGB or RGBA values for each vertex
        ColorScheme   colorScheme; /// @brief Color scheme for the primitive set
        ShadingModel  shadingModel; /// @brief Shading model for the primitive set
        PickScheme    pickScheme;  /// @brief Pick scheme for the primitive set
        struct unique_color_reservation {
            uint32_t start, end;
        } pick_color_reservation; /// @brief Pick color reservation for the primitive set
        MaterialProperty materialProperty; /// @brief Material properties for the primitive set

        std::shared_ptr<std::vector<float>>    positions; /// @brief Positions for this primitive set
        std::shared_ptr<std::vector<float>>    normals;   /// @brief Normals for this primitive set
        std::shared_ptr<std::vector<uint8_t>>  colors;    /// @brief RGB or RGBA values for each vertex
        std::shared_ptr<std::vector<uint32_t>> indices;   /// @brief Indices for this primitive set
        
        enum DirtyFlags {
            DIRTY_NONE           = 0,
            DIRTY_POSITIONS      = 1 << 0,
            DIRTY_NORMALS        = 1 << 1,
            DIRTY_COLORS         = 1 << 2,
            DIRTY_INDICES        = 1 << 3,
            DIRTY_SHADE_MODEL    = 1 << 4,
            DIRTY_COLOR_SCHEME   = 1 << 5,
            DIRTY_PICK_SCHEME    = 1 << 6,
            DIRTY_ALL            = DIRTY_POSITIONS | DIRTY_NORMALS | DIRTY_COLORS | DIRTY_INDICES | DIRTY_SHADE_MODEL | DIRTY_COLOR_SCHEME | DIRTY_PICK_SCHEME
            
        };

        uint32_t dirtyFlags; /// @brief Flags to indicate which data has changed

        PrimitiveSetInstance(const std::string& _InstanceName,  GLenum _PrimitiveType) : InstanceName(_InstanceName), primitiveType(static_cast<PrimitiveType>(_PrimitiveType)), colorFormat(RGB), dirtyFlags(DIRTY_NONE), colorScheme(PER_PRIMITIVE_SET), pickScheme(PICK_NONE) , shadingModel(FLAT) , materialProperty(COLOR_MATERIAL)
        {
            positions = std::make_shared<std::vector<float>>();
            normals   = std::make_shared<std::vector<float>>();
            colors    = std::make_shared<std::vector<uint8_t>>();
            indices   = std::make_shared<std::vector<uint32_t>>();
        }

        virtual ~PrimitiveSetInstance() {}
        
        /*
        @warning 
        Do not use the following function to set primitive type. Once set in constructor its immutable
        @warning Do not use the following function to set the primitive type
        +--------------------------------------------------------------------------------------------------+
        Following function is used to set the primitive type
        /// @brief  Set Primitive type for the current PrimitiveSet
     
        /// @brief Set the primitive type
        void set_primitive_type(PrimitiveType type)   { primitiveType = type;  }
        void set_primitive_type(GLenum type)          { primitiveType = static_cast<PrimitiveType>(type); }
        */

       /// @brief Get the Instance Name
        const std::string get_instance_name() const   { return InstanceName; }

        /// @brief  Get the primitive type
        const PrimitiveType get_primitive_type() const { return primitiveType; }
        const GLenum get_primitive_type_enum() const   { return static_cast<GLenum>(primitiveType); }

        /// @brief Set the color format
        /// @param format 
        void set_color_format(ColorFormat format)     { colorFormat = format; }
        void set_color_format(GLenum format)          { colorFormat = static_cast<ColorFormat>(format); }
        
        /// @brief  Get the color format
        const ColorFormat get_color_format() const    { return colorFormat; }
        const GLenum get_color_format_enum() const    { return static_cast<GLenum>(colorFormat); }

        /// @brief Set the color scheme
        /// @param scheme
        void set_color_scheme(ColorScheme scheme)     { colorScheme = scheme; }
        void set_color_scheme(uint32_t scheme)        { colorScheme = static_cast<ColorScheme>(scheme); }
        
        /// @brief Get the color scheme
        const ColorScheme get_color_scheme() const    { return colorScheme; }
        const GLenum get_color_scheme_enum() const    { return static_cast<GLenum>(colorScheme); }

        /// @brief Set the pick scheme
        /// @param scheme
        void set_pick_scheme(PickScheme scheme)       { pickScheme = scheme; }
        void set_pick_scheme(uint32_t scheme)         { pickScheme = static_cast<PickScheme>(scheme); }

        /// @brief Get the pick scheme
        const PickScheme get_pick_scheme() const      { return pickScheme; }
        const GLenum get_pick_scheme_enum() const     { return static_cast<GLenum>(pickScheme); }   

        /// @brief Get Pickable entites count for the current PrimitiveSet
        const size_t get_pickable_entities_count() const
        {
            size_t count = 0;
            switch(pickScheme)
            {
                case PICK_NONE: count = 0; break;
                case PICK_BY_VERTEX: count = get_num_vertices(); break;
                case PICK_BY_PRIMITIVE: count = get_num_primitives(); break;
                case PICK_GEOMETRY: count = 1; break;
                default: count = 0; break;
            }
            return count;
        }

        /// @brief Set Min and Max Pickable entites for the current PrimitiveSet
        /// @param const min(starting index) generated by SceneManager
        /// @param max(ending index) evaluated and passed to SceneManager

        void set_pickable_entities_range(const size_t& min, size_t& max)
        {
            pick_color_reservation.start = min;
            max = min + get_pickable_entities_count();
            pick_color_reservation.end = max;
        }

        /// @brief Get Min and Max Pickable entites for the current PrimitiveSet
        /// @param const min(starting index) generated by SceneManager
        /// @param max(ending index) evaluated and passed to SceneManager

        const unique_color_reservation & get_pickable_entities_range()
        {
            return pick_color_reservation;
        }
        
        /// @brief Set shader model
        /// @param model
        void set_shading_model(ShadingModel model)    { shadingModel = model; }
        void set_shading_model(GLenum model)          { shadingModel = static_cast<ShadingModel>(model); }

        /// @brief Get the shading model
        const ShadingModel get_shading_model() const  { return shadingModel; }
        const GLenum get_shading_model_enum() const   { return static_cast<GLenum>(shadingModel); }

        /// @brief Get the number of vertices
        const size_t get_num_vertices() const         { return indices->size() ? indices->size() : positions->size() / 3; }
        const size_t get_num_indices() const          { return indices->size(); }
        const size_t get_num_normals() const          { return normals->size() / 3; }
        const size_t get_num_colors() const           { return colors->size() / (colorFormat == RGB ? 3 : 4); }
        const size_t get_num_vertices_per_primitive() const 
        {
            GLuint vertices_per_primitive = 1 ;
            switch(primitiveType) {
                case POINTS:         vertices_per_primitive = 1; break;
                case LINES:          vertices_per_primitive = 2; break;
                case LINE_STRIP:     vertices_per_primitive = 2; break;
                case LINE_LOOP:      vertices_per_primitive = 2; break;
                case TRIANGLES:      vertices_per_primitive = 3; break;
                case TRIANGLE_STRIP: vertices_per_primitive = 3; break;
                case TRIANGLE_FAN:   vertices_per_primitive = 3; break;
                case QUADS:          vertices_per_primitive = 4; break;
                case QUAD_STRIP:     vertices_per_primitive = 4; break;
                case POLYGON:        vertices_per_primitive = 3; break;
                default:             vertices_per_primitive = 1; break;
            }
            return vertices_per_primitive;
        }

        const size_t get_num_primitives() const       { return get_num_vertices() / get_num_vertices_per_primitive(); }

        /// @brief Reset the primitive set
        void reset() {
            positions.reset(); normals.reset();
            colors.reset();    indices.reset();
            dirtyFlags = DIRTY_ALL;
        }
        
        void reset_positions() 
        { positions->resize(0); dirtyFlags |= DIRTY_POSITIONS; }

        void reset_normals() 
        { normals->resize(0);   dirtyFlags |= DIRTY_NORMALS;   }

        void reset_colors() 
        { colors->resize(0);    dirtyFlags |= DIRTY_COLORS;    }

        void reset_indices() 
        { indices->resize(0);   dirtyFlags |= DIRTY_INDICES;   }


        /// @brief Get Dirty Flags
        /// @param Dirtyflag 
        /// @return bool
        const bool isDirty(DirtyFlags flag) const     { return (dirtyFlags & static_cast<int32_t>(flag)) != 0; }
        const bool isDirty(uint32_t flag) const       { return (dirtyFlags & flag) != 0; }
        const bool isDirty() const                    { return  dirtyFlags != 0; }

        /// @brief Set and Clear Dirty Flags
        /// @param flag
        void setDirty(DirtyFlags flag)                { dirtyFlags |= static_cast<int32_t>(flag); }
        void setDirty(uint32_t flag)                  { dirtyFlags |= flag; }
        
        /// @brief Clear Dirty Flags of a specific flag
        void clearDirty(DirtyFlags flag)              { dirtyFlags &= ~static_cast<int32_t>(flag); }
        void clearDirty(uint32_t flag)                { dirtyFlags &= ~flag; }
        
        /// @brief Clear all dirty flags
        void clearDirty()                             { dirtyFlags = 0; }

        /// @brief Validate the primitive set
        const bool isDrawable() const                 { return positions->size() > 0 && primitiveType != PrimitiveType::NONE; }

        /// @brief Validate the primitive set (will throw an exception if the primitive set is not valid)
        /// @details This function is used to validate the primitive set
        /// @details It will throw an exception if the primitive set is not valid
        /// @return bool
        /// @throws std::runtime_error
        /// @warning Do not Modify ! This Function will provide necessary information about the error in the exception message
        const bool isValid() const                    
        {   
            std::string err;
            bool valid = true;
            valid &= isDrawable();

            if(positions->size() % 3 != 0) valid = false;
            if(get_num_indices() > 0) valid &= get_num_indices() % get_num_vertices_per_primitive() == 0;
            if(get_num_normals() > 0) valid &= get_num_normals() == get_num_vertices();
            if(get_num_colors() > 0)  valid &= get_num_colors()  == get_num_vertices();

            if (!valid) {
                std::string errorMessage = std::string("GeometryDescriptor with ID: [") + InstanceName + std::string("] failed validation due to :");
                if (!isDrawable()) {
                    errorMessage += "\n- No drawable primitives ";
                    if(positions->size() == 0)
                       errorMessage += "\n-  No positions !!! (positions->size() == 0) !!!";
                    if(primitiveType == PrimitiveType::NONE)
                       errorMessage += "\n-  No primitive type set !!! (primitiveType == NONE) !!!";
                }
                if (positions->size() % 3 != 0) {
                    errorMessage += "\n- Invalid number of positions (not a multiple of 3)";
                }
                if (get_num_indices() > 0 && get_num_indices() % get_num_vertices_per_primitive() != 0) {
                    errorMessage += "\n- Invalid number of indices (not a multiple of vertices per primitive)";
                }
                if (get_num_normals() > 0 && get_num_normals() != get_num_vertices()) {
                    errorMessage += "\n- Invalid number of normals (not equal to number of vertices)";
                }
                if (get_num_colors() > 0 && get_num_colors() != get_num_vertices()) {
                    errorMessage += "\n- Invalid number of colors (not equal to number of vertices)";
                }

                throw std::runtime_error(errorMessage);
            }

            return valid;
            } 

            };  // Struct PrimitiveSetInstance

    std::unordered_map<std::string, std::shared_ptr<PrimitiveSetInstance>> primitives;
    std::string currentPrimitiveSetInstanceName;
    std::shared_ptr<PrimitiveSetInstance> currentPrimitiveSet;
    

    /// @brief Constructor
    GeometryDescriptor() : currentPrimitiveSetInstanceName("_DEFAULT_") { currentPrimitiveSet = std::make_shared<PrimitiveSetInstance>(currentPrimitiveSetInstanceName, GL_POINTS);}
    
    /// @brief Destructor
    virtual ~GeometryDescriptor() {}
 
 /* 
    @brief 
    Functions for managing primitive sets
    @details
    These functions are used to manage primitive sets in the drawable object 
 */

    /// @brief Set the current primitive set
    __INLINE__ void set_new_primitive_set(const std::string& name, GLenum Primitivetype);

    /// @brief Set the current primitive set
    __INLINE__ void set_current_primitive_set(const std::string& name, GLenum Primitivetype);

    /// @brief get current primitive set name
    /// @return std::string
    __INLINE__ const std::string get_current_primitive_set_name() const { return currentPrimitiveSetInstanceName; }

    /// @brief get a primitive set by name
    /// @param name 
    /// @return std::weak_ptr<PrimitiveSetInstance>. call lock() to get a shared_ptr
    __INLINE__ std::weak_ptr<PrimitiveSetInstance> get_primitive_set(const std::string& name);
    
    /// @brief get the current primitive set
    /// @param name
    /// @return std::weak_ptr<PrimitiveSetInstance>. call lock() to get a shared_ptr
    __INLINE__ std::weak_ptr<PrimitiveSetInstance> get_current_primitive_set();

    /// @brief -> operator to get the current primitive set
    /// @return std::shared_ptr<PrimitiveSetInstance>
    /// @details This is useful when you want to access the current primitive set directly
    __INLINE__ std::shared_ptr<PrimitiveSetInstance>& operator->() { return currentPrimitiveSet; }

    /// @brief remove a primitive set by name
    /// @param name
    __INLINE__ void remove_primitive_set(const std::string& name);

    /// @brief remove all primitive sets
    __INLINE__ void remove_all_primitive_sets();

    /// @brief copy a primitive set
    __INLINE__ void copy_primitive_set(const std::string& src, const std::string& dst);
    
    /// @brief move a primitive set
    __INLINE__ void move_primitive_set(const std::string& src, const std::string& dst);

    /// @brief share a primitive set
    __INLINE__ void share_primitive_set(const std::string& src, const std::string& dst);

    /// @brief copy all primitive sets
    __INLINE__ void copy_all_primitive_sets(const GeometryDescriptor& src);
 
    /// @brief move all primitive sets
    __INLINE__ void move_all_primitive_sets(GeometryDescriptor&& src);

    /// @brief clear all primitive sets
    __INLINE__ void clear_all_primitive_sets();

    /// @brief Get number of primitive sets
    /// @return size_t
    __INLINE__ const size_t get_num_primitive_sets() const { return primitives.size(); }

/*
  @brief 
  Functions for adding vertex attributes data
  @details
  These functions are used to add vertex attributes data to the current primitive set
*/

    /// @brief Push a position vector (x, y, z) to the current primitive set
    __INLINE__ void push_pos3f(const float& x, const float& y, const float& z);

    /// @brief Push a normal vector (n1, n2, n3) to the current primitive set
    __INLINE__ void push_normal3f(const float& n1, const float& n2, const float& n3);

    /// @brief Push a color vector (r, g, b) to the current primitive set
    __INLINE__ void push_color3ub(const uint8_t& r, const uint8_t& g, const uint8_t& b);

    /// @brief Push a color vector (r, g, b, a) to the current primitive set
    __INLINE__ void push_color4ub(const uint8_t& r, const uint8_t& g, const uint8_t& b , const uint8_t a);

    /// @brief Push indices to the current primitive set
    __INLINE__ void push_index(const uint32_t& index);    

    /// @brief Push a position array to the current primitive set (inserts the array at the end of the current array)
    __INLINE__ void push_pos_array(const std::vector<float>& position_array);

    /// @brief Push a normal array to the current primitive set (inserts the array at the end of the current array)
    __INLINE__ void push_normal_array(const std::vector<float>& normal_array);

    /// @brief Push a color array to the current primitive set (inserts the array at the end of the current array)
    __INLINE__ void push_color_array(const std::vector<uint8_t>& color_array);

    /// @brief Push a Index array to the current primitive set (inserts the array at the end of the current array)
    __INLINE__ void push_index_array(const std::vector<uint32_t>& index_array);

    /// @brief Copy a position array to the current primitive set (replaces the current array)
    __INLINE__ void copy_pos_array(const std::vector<float>& position_array);

    /// @brief Copy a normal array to the current primitive set (replaces the current array)
    __INLINE__ void copy_normal_array(const std::vector<float>& normal_array);

    /// @brief Copy a color array to the current primitive set (replaces the current array)
    __INLINE__ void copy_color_array(const std::vector<uint8_t>& color_array);     

    /// @brief Copy a Index array to the current primitive set (replaces the current array)
    __INLINE__ void copy_index_array(const std::vector<uint32_t>& index_array);

    /// @brief Move a position array to the current primitive set (replaces the current array)
    __INLINE__ void move_pos_array(std::vector<float>&& position_array);

    /// @brief Move a normal array to the current primitive set (replaces the current array)
    __INLINE__ void move_normal_array(std::vector<float>&& normal_array);
     
    /// @brief Move a color array to the current primitive set (replaces the current array)
    __INLINE__ void move_color_array(std::vector<uint8_t>&& color_array);
    
    /// @brief Move a Index array to the current primitive set (replaces the current array)
    __INLINE__ void move_index_array(std::vector<uint32_t>&& index_array);

    /// @brief    copy by values vertex attrib array
    /// @details  Copy the vertex attrib array by values to another primitive set
    /// @details  This is useful when you want to copy the same vertex attrib array between multiple primitive sets
    /// @param src , 
    /// @param dst
    __INLINE__ void copy_attrib_array(const std::string& src, const std::string& dst, PrimitiveSetInstance::VertexArrayType type);

    /// @brief    copy by ref vertex attrib array
    /// @details  Copy the vertex attrib array as a reference to another primitive set
    /// @details  This is useful when you want to share the same vertex attrib array between multiple primitive sets
    /// @param src 
    /// @param dst 
    __INLINE__ void share_attrib_array(const std::string& src, const std::string& dst, PrimitiveSetInstance::VertexArrayType type);
   
    /// @brief Other methods for dirty flags, etc. can be added here
    /// @details clears all dirty flags for all primitive sets
    /// @warning Use -> operator to instead to clear the current primitive set dirty flags
    __INLINE__ void clearDirtyFlags();

    /// @brief  Clear dirty flags for a specific primitive set
    /// @param name 
    __INLINE__ void clearDirtyFlags(const std::string& name);

    /// @brief  Clear dirty flags for a specific primitive set and a specific attribute flag
    /// @param name 
    /// @param flag 
    __INLINE__ void clearDirtyFlags(const std::string& name, PrimitiveSetInstance::DirtyFlags flag) ;

    /// @brief  check if the current PrimitiveSet is drawable.
    /// @return bool
    __INLINE__ bool isDrawable() const;

    /// @brief  check if the geometry has any drawable primitives
    /// @return bool
    __INLINE__ bool hasAnyDrawables() const;

    /// @brief  check if the current PrimitiveSet is valid.
    /// @return bool
    __INLINE__ bool isValid() const;

};

#endif // _HLM_DRAWABLE_H_