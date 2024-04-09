#ifndef _GP_GUI_GEOMETRY_DESCRIPTOR_DRAWABLE_H_
#define _GP_GUI_GEOMETRY_DESCRIPTOR_DRAWABLE_H_

/// @author srikar@gridpro.com

/// @file    _GP_GUI_GEOMETRY_DESCRIPTOR_H_
/// @brief   This file contains the definition of the GeometryDescriptor class
/// @details This file contains the definition of the GeometryDescriptor class, which is used to store and manage drawable objects, including points, lines, triangles, and other primitives.
/// @details The class is designed to be used with OpenGL, but it can be used with other rendering systems as well.
/// Provides a standard interface for rendering engines to use.

/// @dependencies 
/// @details   This class depends on the following libraries:
/// @details - STL (for data structures and algorithms)

/// @note      Can be used independently as a standalone class


#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <string>
#include <iostream>
#include <array>

#include "gp_gui_typedefs.h"

/*
 * Functions
 */

/// @brief   An Inheritable Class for drawable objects ranging from simple points to complex 3D models
/// @details This class is used to store and manage drawable objects, including points, lines, triangles, and other primitives.
/// @details The class is designed to be used with OpenGL, but it can be used with other rendering systems as well.
/// Provides a standard interface for rendering engines to use.
/// Currently, the class supports only vertex positions, normals, and colors. Texture coordinates and other attributes can be added in the future.
namespace gridpro_gui {

class GeometryDescriptor 
{
public:
    struct PrimitiveSetInstance {
        /// @brief Enumeration for primitive types
        enum PrimitiveType {
            NONE           = GL_NONE_NULL,
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
            PER_VERTEX = GL_COLOR_PER_VERTEX, 
            PER_PRIMITIVE = GL_COLOR_PER_PRIMITIVE,
            PER_PRIMITIVE_SET = GL_COLOR_PER_PRIMITIVE_SET
        };

        /// @brief  Enumeration for Shading Model
        enum ShadingModel {
            FLAT   = GL_FLAT,
            SMOOTH = GL_SMOOTH
        };

        enum WireframeMode {
            WIREFRAME_NONE = GL_NONE,
            WIREFRAME_ONLY = GL_WIREFRAME_ONLY,
            WIREFRAME_OVERLAY = GL_WIREFRAME_OVERLAY
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
            PICK_NONE = GL_PICK_NONE,
            PICK_BY_VERTEX = GL_PICK_BY_VERTEX,
            PICK_BY_PRIMITIVE = GL_PICK_BY_PRIMITIVE,
            PICK_GEOMETRY = GL_PICK_GEOMETRY
        };

        /// @brief  Enumeration for vertex array types
        enum VertexArrayType {
            POSITION_ARRAY = GL_VERTEX_ARRAY,
            NORMAL_ARRAY   = GL_NORMAL_ARRAY,
            COLOR_ARRAY    = GL_COLOR_ARRAY,
            INDEX_ARRAY    = GL_INDEX_ARRAY
        };

        /// @brief  Enumeration for dirty flags
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
        
        struct unique_color_reservation {
            uint32_t EntityID;
            uint32_t start, end;
        }; 
   
        PrimitiveSetInstance(const std::string& _InstanceName,  GLenum _PrimitiveType) : InstanceName(_InstanceName), primitiveType(static_cast<PrimitiveType>(_PrimitiveType)), colorFormat(RGB), dirtyFlags(DIRTY_NONE), colorScheme(PER_PRIMITIVE_SET), pickScheme(PICK_NONE) , shadingModel(FLAT) , materialProperty(COLOR_MATERIAL), wireframecolor(0, 0, 200, 255)
        {
            positions = std::make_shared<std::vector<float>>(0);
            normals   = std::make_shared<std::vector<float>>(0);
            colors    = std::make_shared<std::vector<uint8_t>>(0);
            indices   = std::make_shared<std::vector<uint32_t>>(0);
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
        void set_color_scheme(const ColorScheme scheme)     { colorScheme = scheme; }
        void set_color_scheme(const uint32_t scheme)        { colorScheme = static_cast<ColorScheme>(scheme); }
        
        /// @brief Get the color scheme
        /// @return PrimitiveSetInstance::ColorScheme
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
            pick_color_reservation.start = min + 1;
            uint32_t reserve = get_pickable_entities_count();
            if(reserve == 1) reserve = 0;
            max = min + reserve;
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
        
        /// @brief Set Wireframe Mode
        /// @param mode
        void set_wireframe_mode(WireframeMode mode)    { wireframeMode = mode; }
        void set_wireframe_mode(GLenum mode)           { wireframeMode = static_cast<WireframeMode>(mode); }
        
        /// @brief Get Wireframe Mode
        const WireframeMode get_wireframe_mode() const { return wireframeMode; }
        const GLenum get_wireframe_mode_enum() const   { return static_cast<GLenum>(wireframeMode); }

        /// @brief Get Weak Pointer to the Positions
        std::weak_ptr<std::vector<float>> get_position_weak_ptr()  const   
        { return positions; }
        
        /// @brief Get Weak Pointer to the Normals
        std::weak_ptr<std::vector<float>> get_normals_weak_ptr()    const  
        { return normals;  }
        
        /// @brief Get Weak Pointer to the Colors
        std::weak_ptr<std::vector<uint8_t>> get_colors_weak_ptr()   const  
        { return colors;   }

        /// @brief Get Weak Pointer to the Indices
        std::weak_ptr<std::vector<uint32_t>> get_indices_weak_ptr() const  
        { return indices;  }

        /// @brief Share Pointer to the Positions
        void share_position_shared_ptr(std::shared_ptr<std::vector<float>>& in_position) 
        { positions = in_position; }

        /// @brief Share Pointer to the Normals
        void share_normals_shared_ptr(std::shared_ptr<std::vector<float>>& in_normal) 
        { normals = in_normal; }

        /// @brief Share Pointer to the Colors
        void share_colors_shared_ptr(std::shared_ptr<std::vector<uint8_t>>& in_color) 
        { colors = in_color; }

        /// @brief SharePointer to the Indices
        void share_indices_shared_ptr(std::shared_ptr<std::vector<uint32_t>>& in_indices) 
        { indices = in_indices; }


        /// @brief Get the number of vertices
        const size_t get_num_vertices() const         { return indices->size() ? indices->size() : positions->size() / 3; }
        const size_t get_num_indices()  const         { return indices->size(); }
        const size_t get_num_normals()  const         { return normals->size() / 3; }
        const size_t get_num_colors()   const         { return colors->size() / (colorFormat == RGB ? 3 : 4); }
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
                case NONE:           throw std::runtime_error("PrimitiveType is set to None\n"); break;  
                default:             vertices_per_primitive = 1; break;
            }
            return vertices_per_primitive;
        }

        const size_t get_num_primitives() const       { return get_num_vertices() / get_num_vertices_per_primitive(); }

        /// @brief Reset the primitive set
        void release_ref_all() {

            positions.reset(); normals.reset();
            colors.reset();    indices.reset();
            
            positions = std::make_shared<std::vector<float>>(0);
            normals   = std::make_shared<std::vector<float>>(0);
            colors    = std::make_shared<std::vector<uint8_t>>(0);
            indices   = std::make_shared<std::vector<uint32_t>>(0);

            dirtyFlags = DIRTY_ALL;
        }

        /// @brief clear the primitive set
        void clear_all() {
            clear_positions(); clear_normals(); 
            clear_colors();    clear_indices();
        }        
        
        void clear_positions() 
        { positions->resize(0); dirtyFlags |= DIRTY_POSITIONS; }

        void clear_normals() 
        { normals->resize(0);   dirtyFlags |= DIRTY_NORMALS;   }

        void clear_colors() 
        { colors->resize(0);    dirtyFlags |= DIRTY_COLORS;    }

        void clear_indices() 
        { indices->resize(0);   dirtyFlags |= DIRTY_INDICES;   }

        void release_positions_ref() 
        { positions.reset(); positions = std::make_shared<std::vector<float>>(0);     dirtyFlags |= DIRTY_POSITIONS; }

        void release_normals_ref() 
        { normals.reset();   normals   = std::make_shared<std::vector<float>>(0);     dirtyFlags |= DIRTY_NORMALS;   }

        void release_colors_ref() 
        { colors.reset();    colors    = std::make_shared<std::vector<uint8_t>>(0);   dirtyFlags |= DIRTY_COLORS;    }

        void release_indices_ref() 
        { indices.reset();   indices   = std::make_shared<std::vector<uint32_t>>(0);  dirtyFlags |= DIRTY_INDICES;   }


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
        
        const uint32_t getDirtyFlags() const          { return dirtyFlags; }

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
            if(get_num_colors()  > 0) valid &= get_num_colors()  == get_num_vertices();

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
             
           /// @brief Get Positions Vector const ref
           /// @warning Do not const cast !!!
           const std::vector<float>& positions_vector() {  return *positions;  } 

           /// @brief Get Normals Vector
           /// @warning Do not const cast !!!
           const std::vector<float>& normals_vector()   { return *normals; }   

           /// @brief Get Colors Vector
           /// @warning Do not const cast !!!
           const std::vector<uint8_t>& colors_vector()  { return *colors;} 

           /// @brief Get Indices Vector
           /// @warning Do not const cast !!!
           const std::vector<uint32_t>& indices_vector() { return *indices;}   
           
           void flatten_postion_array()
           {
            if(indices_vector().size() == 0) return;
            
            std::vector<float> temp_positions(get_num_vertices() * 3);
            
            auto& indices = indices_vector();
            auto& positions = positions_vector();
            const uint32_t num_vertices = get_num_vertices();
            for(size_t i = 0; i < num_vertices; i++)
            {
                temp_positions[i * 3 + 0] = positions[indices[i] * 3 + 0];
                temp_positions[i * 3 + 1] = positions[indices[i] * 3 + 1];
                temp_positions[i * 3 + 2] = positions[indices[i] * 3 + 2];
            }
             
            *(this->positions) = (std::move(temp_positions));
              release_indices_ref();
           }

            private : 
            friend class GeometryDescriptor;
             /// @brief Name of the primitive set instance
            const std::string   InstanceName;
            
            /// @brief Primitive type (e.g., GL_TRIANGLES, GL_LINES, etc.)
            const PrimitiveType primitiveType; 
            
            /// @brief RGB or RGBA values for each vertex
            ColorFormat colorFormat; 

            /// @brief Color scheme for the primitive set
            ColorScheme colorScheme; 

            /// @brief Shading model for the primitive set
            ShadingModel shadingModel; 

            /// @brief Wireframe mode for the primitive set
            WireframeMode wireframeMode;

            /// @brief Material properties for the primitive set
            MaterialProperty materialProperty; 

            /// @brief Pick scheme for the primitive set
            PickScheme pickScheme;  

            /// @brief Pick color reservation for the primitive set
            struct unique_color_reservation  pick_color_reservation; 
        
            /// @brief Positions for this primitive set
            std::shared_ptr<std::vector<float>>    positions;

            /// @brief Normals for this primitive set
            std::shared_ptr<std::vector<float>>    normals;

             /// @brief RGB or RGBA values for each vertex
            std::shared_ptr<std::vector<uint8_t>>  colors;

            /// @brief Indices for this primitive set
            std::shared_ptr<std::vector<uint32_t>> indices;   
        
            /// @brief Flags to indicate which data has changed
            uint32_t dirtyFlags; 
            
            public :
            /// @brief Color if(if Mono Color Scheme)
            struct Color { 
               uint8_t r, g, b, a;
               Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a) {}
               Color() : r(255), g(255), b(255), a(255) {}
               ~Color() {}
               std::array<float, 4> get_color() { return {r/255.0f, g/255.0f, b/255.0f, a/255.0f}; }
            };
            
            Color color;
            Color wireframecolor; 
            };  // Struct PrimitiveSetInstance

    //+---------------------------------------------------------------------------------------------------------+
    #define primitive_set_iterator std::unordered_map<std::string, std::shared_ptr<PrimitiveSetInstance>>::iterator
    //+---------------------------------------------------------------------------------------------------------+
    /// @brief Member Variables of Geometry Descriptor
    std::unordered_map<std::string, std::shared_ptr<PrimitiveSetInstance>> primitives;
    std::string currentPrimitiveSetInstanceName;
    std::shared_ptr<PrimitiveSetInstance> currentPrimitiveSet;
    uint32_t id;
    //+---------------------------------------------------------------------------------------------------------+
    const uint32_t get_id() const { return id; }
    void set_id(uint32_t _id)     { id = _id;  }
    
    void set_color_id_reserve_start(uint32_t start)  {  currentPrimitiveSet->pick_color_reservation.start = start; }
    const uint32_t& get_color_id_reserve_start()     {  return currentPrimitiveSet->pick_color_reservation.start;  }
    const uint32_t get_color_id_reserve_end()        
    {   
        uint32_t reserve = currentPrimitiveSet->get_pickable_entities_count();
        if(reserve == 1) reserve = 0;
        currentPrimitiveSet->pick_color_reservation.end  = currentPrimitiveSet->pick_color_reservation.start + reserve;   
        return currentPrimitiveSet->pick_color_reservation.end; 
    }
    
    void set_pick_scheme(const GLenum& scheme)       { currentPrimitiveSet->set_pick_scheme(scheme); }

    /// @brief Constructor
    GeometryDescriptor();
    
    /// @brief Destructor
    virtual ~GeometryDescriptor();
    
    /// @brief Assignment operator
    GeometryDescriptor& operator=(const GeometryDescriptor& src) 
    {        
        primitives = src.primitives;
        currentPrimitiveSetInstanceName = src.currentPrimitiveSetInstanceName;
        currentPrimitiveSet = src.currentPrimitiveSet;
        id = src.id;
        
        return *this;
    }
    
    /// @brief Copy Constructor
    GeometryDescriptor(const GeometryDescriptor& src) 
    {
        primitives = src.primitives;
        currentPrimitiveSetInstanceName = src.currentPrimitiveSetInstanceName;
        currentPrimitiveSet = src.currentPrimitiveSet;
        id = src.id;
    }

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
    __INLINE__ void copy_vertex_attributes(const std::string& src, const std::string& dst);
    
    /// @brief move a primitive set
    __INLINE__ void move_vertex_attributes(const std::string& src, const std::string& dst);

    /// @brief share a primitive set
    __INLINE__ void share_vertex_attributes(const std::string& src, const std::string& dst);

    /// @brief copy all primitive sets
    __INLINE__ void copy_all_primitive_sets(const GeometryDescriptor& src);
 
    /// @brief move all primitive sets
    __INLINE__ void move_all_primitive_sets(GeometryDescriptor&& src);

    /// @brief clear all primitive sets
    __INLINE__ void clear_all_primitive_sets();

    /// @brief  Get number of primitive sets
    /// @return size_t
    __INLINE__ const size_t get_num_primitive_sets() const { return primitives.size(); }

    /// @brief  Get Begin iterator of the Primitive Set
    /// @return std::unordered_map<std::string, std::shared_ptr<PrimitiveSetInstance>>::iterator 
    __INLINE__ primitive_set_iterator begin() {return primitives.begin();}

    /// @brief  Get End iterator of the Primitive Set
    /// @return std::unordered_map<std::string, std::shared_ptr<PrimitiveSetInstance>>::iterator 
    __INLINE__ primitive_set_iterator end() {return primitives.end();}

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

    /// @brief    copy by values vertex attrib array
    /// @details  Copy the vertex attrib array by values to all other primitive set
    /// @details  This is useful when you want to copy the same vertex attrib array between multiple primitive sets
    /// @param src , 
    /// @param dst
    __INLINE__ void copy_attrib_array_with_all(const std::string& src, PrimitiveSetInstance::VertexArrayType type);

    /// @brief    copy by ref vertex attrib array
    /// @details  Copy the vertex attrib array as a reference to all other primitive set
    /// @details  This is useful when you want to share the same vertex attrib array between multiple primitive sets
    /// @param src 
    /// @param dst 
    __INLINE__ void share_attrib_array_with_all(const std::string& src, PrimitiveSetInstance::VertexArrayType type);   
    

    /// @brief   Other methods for dirty flags, etc. can be added here
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
}
#endif // _HLM_DRAWABLE_H_