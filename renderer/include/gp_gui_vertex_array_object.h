#ifndef GP_GUI_VERTEX_ARRAY_OBJECT_H
#define GP_GUI_VERTEX_ARRAY_OBJECT_H

#include "gp_gui_renderer_api.h"


namespace gridpro_gui
{
    class GeometryDescriptor;

    class VertexArrayObject 
    {
       public :
       VertexArrayObject();
       VertexArrayObject(GeometryDescriptor* geometry_descriptor);
       VertexArrayObject(std::vector<float>* position_data = nullptr, std::vector<float>* normal_data = nullptr, std::vector<GLubyte>* color_data = nullptr);
      ~VertexArrayObject();
       
       void set_vertex_attribute(std::vector<float>* position_data, std::vector<float>* normal_data, std::vector<GLubyte>* color_data);
       void set_indices(std::vector<uint32_t>* index_data); 

       void bind();
       void unbind();
       
       /// @brief Calculate the offsets for the vertex attributes
       void calculate_offsets();

       /// @brief Create the vertex buffer object
       void create_vbo();
       void create_ibo();
       
       void update_vbo();
       void update_ibo();

       void delete_vbo();
       void delete_ibo();
       void delete_vao();
       
       /// @brief   Get the vertex array object size in bytes
       /// @return  The size of the vertex array object in bytes
       const size_t get_vbo_size() const { return m_vbo_curr_size; }


       /// @brief Get which vertex attribute data is present
       const bool has_normal_attrib() const { return (NormalData != nullptr && NormalData->size()) ? 1 : 0; }
       const bool has_color_attrib()  const { return (ColorData  != nullptr && ColorData->size())  ? 1 : 0; }
       
         /// @brief Get if element array buffer is present
       const bool has_index_data() const { return (IndexData != nullptr && IndexData->size()) ? 1 : 0; }

       private :
       uint32_t m_vao, m_vbo, m_ibo;
       
       uint32_t vSize, nSize, cSize;
            
       uint32_t vOffset, nOffset, cOffset;

       uint32_t m_vbo_curr_size, m_ibo_curr_size;
        
       GeometryDescriptor* m_geometry_descriptor;

       std::vector<float>*    PositionData;
       std::vector<float>*    NormalData;
       std::vector<GLubyte>*  ColorData;
       std::vector<uint32_t>* IndexData;

       std::vector<GLfloat>  DummyData1;
       std::vector<GLubyte>  DummyData2;
       std::vector<uint32_t> DummyData3;

    };

}    

#endif // GP_GUI_VERTEX_ARRAY_OBJECT_H