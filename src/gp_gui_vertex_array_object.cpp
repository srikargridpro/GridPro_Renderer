#include "gp_gui_vertex_array_object.h"
#include "gp_gui_geometry_descriptor.h"

namespace gridpro_gui
{

   VertexArrayObject::VertexArrayObject() : m_vao(0), m_vbo(0), m_ibo(0), m_vbo_curr_size(0), m_ibo_curr_size(0)
    {
         PositionData = &DummyData1;
         NormalData   = &DummyData1;
         ColorData    = &DummyData2;
         IndexData    = &DummyData3;
         DummyData1.resize(0);
         DummyData2.resize(0);
         DummyData3.resize(0);
    }

    VertexArrayObject::VertexArrayObject(std::vector<float>* position_data , std::vector<float>* normal_data , std::vector<GLubyte>* color_data) :
        m_vao(0), m_vbo(0), m_ibo(0), m_vbo_curr_size(0), m_ibo_curr_size(0)
    { 
         PositionData = &DummyData1;
         NormalData   = &DummyData1;
         ColorData    = &DummyData2;
         IndexData    = &DummyData3;
         DummyData1.resize(0);
         DummyData2.resize(0);
         DummyData3.resize(0);

        if(position_data != nullptr)  
           PositionData = position_data;
        if(normal_data  != nullptr)  
           NormalData = normal_data;
        if(color_data != nullptr)
           ColorData  = color_data;
        
        if(PositionData == nullptr) 
          throw std::runtime_error(" Position Data is empty\n");
 
        calculate_offsets();
        create_vbo();
    }     


    VertexArrayObject::VertexArrayObject(GeometryDescriptor* geometry_descriptor) :
        m_geometry_descriptor(geometry_descriptor) , m_vao(0), m_vbo(0), m_ibo(0), m_vbo_curr_size(0), m_ibo_curr_size(0)
    {
        PositionData = (*m_geometry_descriptor)->get_position_weak_ptr().lock().get();
        NormalData   = (*m_geometry_descriptor)->get_normals_weak_ptr().lock().get();
        ColorData    = (*m_geometry_descriptor)->get_colors_weak_ptr().lock().get();
        IndexData    = (*m_geometry_descriptor)->get_indices_weak_ptr().lock().get();
        
        if(PositionData->size() == 0) 
        {
            std::string err = m_geometry_descriptor->get_current_primitive_set_name() + " Position Data is empty\n";
            throw std::runtime_error(err);
        }

        calculate_offsets();
        create_vbo();

        if(IndexData->size() != 0)
        {
            create_ibo();
        }
    }

    VertexArrayObject::~VertexArrayObject()
    {
        Renderer::GL_API()->glDeleteVertexArrays(1, &m_vao);
        delete_vbo();
        delete_ibo();
    }

    void VertexArrayObject::set_vertex_attribute(std::vector<float>* position_data = nullptr, std::vector<float>* normal_data = nullptr, std::vector<GLubyte>* color_data = nullptr)
    {
        if(position_data != nullptr)  
           PositionData = position_data;
        
        if(normal_data != nullptr)  
           NormalData   = normal_data;

        if(color_data != nullptr)
           ColorData    = color_data;

        try 
        {
            delete_vbo();
        }

        catch(const std::exception& e)
        {
            DEBUG_PRINT(e.what(), '\n');
        } 

        calculate_offsets();
        create_vbo();
    }   
       
    void VertexArrayObject::set_indices(std::vector<uint32_t>* index_data)
    {
        if(index_data == nullptr) return;
        IndexData = index_data;
        try 
        {
            delete_ibo();
        }

        catch(const std::exception& e)
        {
            DEBUG_PRINT(e.what(), '\n');
        } 

        create_ibo();
    } 


    void VertexArrayObject::bind()
    {
        if(Renderer::GL_API()->glIsVertexArray(m_vao) != GL_TRUE) 
        { 
            DEBUG_PRINT("VAO is not created\n");
            return;
        }   
        else
        {
            Renderer::GL_API()->glBindVertexArray(m_vao);
            DEBUG_PRINT("VAO is bound\n");
        }
        
        if(Renderer::GL_API()->glIsBuffer(m_ibo) == GL_TRUE)
        {
            Renderer::GL_API()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);       
        }
    }

    void VertexArrayObject::unbind()
    {
        Renderer::GL_API()->glBindVertexArray(0);
        Renderer::GL_API()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    }
    
    void VertexArrayObject::calculate_offsets()
    {
        vSize = 0;
        nSize = 0;
        cSize = 0;

        if (PositionData)
            vSize = PositionData->size() * sizeof(float);

        if (NormalData)
            nSize = NormalData->size()   * sizeof(float);

        if (ColorData)
            cSize = ColorData->size()    * sizeof(GLubyte);

        vOffset = 0;
        nOffset = vSize;
        cOffset = vSize + nSize;
    }

    void VertexArrayObject::create_vbo()
    {   
    
        calculate_offsets();
        /// @brief Allocate the vertex buffer object only if the vertex data size has changed
        /// @note  This is to avoid the reallocation of the VBO for every frame
        if(m_vbo_curr_size != vSize + nSize + cSize)
        { 
           delete_vao();
           delete_vbo();
           Renderer::GL_API()->glGenVertexArrays(1, &m_vao);
           Renderer::GL_API()->glGenBuffers(1, &m_vbo);
           Renderer::GL_API()->glBindVertexArray(m_vao);
           Renderer::GL_API()->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
           Renderer::GL_API()->glBufferData(GL_ARRAY_BUFFER, vSize + nSize + cSize, nullptr, GL_STATIC_DRAW);
           m_vbo_curr_size = vSize + nSize + cSize;
        }
        else
        {
           Renderer::GL_API()->glBindVertexArray(m_vao);
           Renderer::GL_API()->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        }

    
        uint32_t it = 0;

        // Copy data to VBO
        if (vSize != 0)
            Renderer::GL_API()->glBufferSubData(GL_ARRAY_BUFFER, vOffset, vSize, PositionData->data());

        if (nSize != 0)
            Renderer::GL_API()->glBufferSubData(GL_ARRAY_BUFFER, nOffset, nSize, NormalData->data());

        if (cSize != 0)
            Renderer::GL_API()->glBufferSubData(GL_ARRAY_BUFFER, cOffset, cSize, ColorData->data());

        // Set vertex attributes pointers
        if (vSize)
        {
            Renderer::GL_API()->glVertexAttribPointer(it, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
            Renderer::GL_API()->glEnableVertexAttribArray(it);
            ++it;
        }
        
        else
        {
            // Throw Some Exception for rehandle the render pass
            // glVertexAttrib3f(0, 0.0f, 0.0f, 0.1f);
        }

        if (nSize)
        {
            Renderer::GL_API()->glVertexAttribPointer(it, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)vOffset);
            Renderer::GL_API()->glEnableVertexAttribArray(it);
            ++it;
        }

        else
        {
            //glVertexAttrib3f(2, 0.0f, 0.0f, 0.1f);
        }

        if (cSize)
        {
            Renderer::GL_API()->glVertexAttribPointer(it, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)nOffset);
            Renderer::GL_API()->glEnableVertexAttribArray(it);
        }
        else
        {
            //Renderer::GL_API()->glVertexAttrib3f(2, 1.0f, 0.0f, 0.0f);
        }

        // Unbind VBO
        Renderer::GL_API()->glBindBuffer(GL_ARRAY_BUFFER, 0);
        unbind();

        }

      
      void VertexArrayObject::create_ibo()
      {   
          if(m_ibo_curr_size != IndexData->size()) 
          {
            delete_ibo();
            Renderer::GL_API()->glGenBuffers(1, &m_ibo); 
          }

          bind();
          Renderer::GL_API()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
          Renderer::GL_API()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexData->size() * sizeof(uint32_t), IndexData->data(), GL_STATIC_DRAW);
          m_ibo_curr_size = IndexData->size();
          Renderer::GL_API()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
          unbind();
      }


      void VertexArrayObject::update_vbo()
      {
            Renderer::GL_API()->glBindVertexArray(m_vao);
            Renderer::GL_API()->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            calculate_offsets();
            Renderer::GL_API()->glBufferData(GL_ARRAY_BUFFER, vSize + nSize + cSize, nullptr, GL_STATIC_DRAW);
    
            if (vSize)
                Renderer::GL_API()->glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, PositionData->data());
    
            if (nSize)
                Renderer::GL_API()->glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, NormalData->data());
    
            if (cSize)
                Renderer::GL_API()->glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize, cSize, ColorData->data());
    
            Renderer::GL_API()->glBindBuffer(GL_ARRAY_BUFFER, 0);
            Renderer::GL_API()->glBindVertexArray(0);
      }

        void VertexArrayObject::update_ibo()
        {
            if(IndexData->size() == 0)
                return;

            Renderer::GL_API()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            Renderer::GL_API()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexData->size() * sizeof(uint32_t), IndexData->data(), GL_STATIC_DRAW);
            Renderer::GL_API()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        void VertexArrayObject::delete_vbo()
        {
            if(Renderer::GL_API()->glIsBuffer(m_vbo) == GL_TRUE)
            {
               Renderer::GL_API()->glDeleteBuffers(1, &m_vbo);
               m_vbo = 0;
            }
            else
               DEBUG_PRINT("VBO is already deleted\n");   
        }

        void VertexArrayObject::delete_ibo()
        {
            if(Renderer::GL_API()->glIsBuffer(m_ibo) == GL_TRUE)
            {
               Renderer::GL_API()->glDeleteBuffers(1, &m_ibo);
               m_ibo = 0;
            }
            else
              DEBUG_PRINT("IBO is already deleted\n");  
        }

        void VertexArrayObject::delete_vao()
        {
            bind(); 
            if(Renderer::GL_API()->glIsVertexArray(m_vao) == GL_TRUE)
            {
               Renderer::GL_API()->glDeleteVertexArrays(1, &m_vao);
               m_vao = 0;
            }
            unbind(); 
        }



} // namespace gridpro_gui