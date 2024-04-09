#include "gp_gui_renderer_api.h"
#include "gp_gui_opengl_3_3_render_kernel.h"
#include "gp_gui_geometry_descriptor.h"
#include "gp_gui_vertex_array_object.h"
#include "gp_gui_shader.h"
#include "gp_gui_communications.h"
#include "gp_gui_events.h"
#include "gp_gui_texture.h"
#include "gp_gui_instrumentation.h"
#include "gp_gui_pixel_utils.h"
#include <exception>
//#include <glm/gtx/string_cast.hpp>
// Define a macro for OpenMP pragmas

#ifndef USE_OPENMP
    #include <omp.h>
    #define PARALLEL_FOR _Pragma("omp parallel for")
    #define PARALLEL_FOR_NUM_THREADS(num_threads) _Pragma(omp parallel for num_threads(num_threads))
#else
    #define PARALLEL_FOR
    #define PARALLEL_FOR_NUM_THREADS(num_threads)
#endif

namespace gridpro_gpu_metrics
{
  static uint32_t gpu_current_vertex_array_size = 0;
}

namespace gridpro_gui
{
    OpenGL_3_3_RenderKernel::OpenGL_3_3_RenderKernel(std::shared_ptr<GeometryDescriptor>& geometry_descriptor)
    : m_geometry_descriptor(geometry_descriptor) , init_flag(false)
    {
       init();
    }

    OpenGL_3_3_RenderKernel::OpenGL_3_3_RenderKernel()  : init_flag(false)
    {

    }

    void OpenGL_3_3_RenderKernel::init()
    {   
        if(init_flag) return;
        if(m_geometry_descriptor == nullptr) throw std::runtime_error("Geometry Descriptor is not set");
        m_vao = std::make_shared<VertexArrayObject>(m_geometry_descriptor.get());
        gridpro_gpu_metrics::gpu_current_vertex_array_size +=  m_vao->get_vbo_size();
        std::cout << "Current Vertex Array Size = " << gridpro_gpu_metrics::gpu_current_vertex_array_size << std::endl;
        m_geometry_descriptor->clearDirtyFlags();

        init_flag = true;
    }
    
    OpenGL_3_3_RenderKernel::~OpenGL_3_3_RenderKernel()
    {
       
    }
    
    /// @brief Set the geometry descriptor (For setting the geometry descriptor)
    void OpenGL_3_3_RenderKernel::set_geometry_descriptor(const std::shared_ptr<GeometryDescriptor>& geometry_descriptor)
    {
       reset();
       m_geometry_descriptor = geometry_descriptor;
       init();
    }
    
    /// @brief Render the geometry in display mode (For rendering the geometry)
    bool OpenGL_3_3_RenderKernel::render_display_mode()
    {
      
        try 
        { 
          if((*m_geometry_descriptor)->positions_vector().size() == 0) return false;
          
            // Bind the texture
            // m_texture->bind(0);
            m_shader = ShaderLibrary::GetShader("BasicShader");
            m_shader->bind();
            SceneState& scene_state = Event::Publisher::GetInstance()->get_scene_state();
          
            /// Set the shader uniforms
            m_shader->SetMat4fv("projection", scene_state.m_projection);
            m_shader->SetMat4fv("model", scene_state.m_model);
            m_shader->SetMat4fv("view", scene_state.m_view);        
                 
            // Enable if you want to use the texture  
            // m_shader->Set1i("textureSampler", *m_texture);
            m_vao->bind();

            //// Draw the geometry in fill mode if wireframe mode is overlay 
            if((*m_geometry_descriptor)->get_wireframe_mode_enum() == GL_WIREFRAME_OVERLAY)
            {
              glm::vec4 object_color = glm::make_vec4((*m_geometry_descriptor)->color.get_color().data());
              m_shader->SetVec4fv("object_color", object_color);   
              // Draw Call
              execute_draw_command();
              //// Draw the in wireframe only or fill mode only based on the rasteriser state
              glm::vec4 wireframe_color = glm::make_vec4((*m_geometry_descriptor)->wireframecolor.get_color().data());
              m_shader->SetVec4fv("object_color", wireframe_color);          
            
              set_rasteriser_state();
            
              // Draw Call
              execute_draw_command();
            
              reset_rasteriser_state();              
            }
            else if((*m_geometry_descriptor)->get_wireframe_mode_enum() == GL_WIREFRAME_ONLY)
            {
              glm::vec4 wireframe_color = glm::make_vec4((*m_geometry_descriptor)->wireframecolor.get_color().data());
              m_shader->SetVec4fv("object_color", wireframe_color);          
            
              set_rasteriser_state();
            
              // Draw Call
              execute_draw_command();
            
              reset_rasteriser_state();
            }
            else
            {
              //// Draw the in wireframe only or fill mode only based on the rasteriser state
              glm::vec4 wireframe_color = glm::make_vec4((*m_geometry_descriptor)->wireframecolor.get_color().data());
              m_shader->SetVec4fv("object_color", wireframe_color);          
            
              set_rasteriser_state();
            
              // Draw Call
              execute_draw_command();
            
              reset_rasteriser_state();
            }


            // Unbind the all the objects
            // m_texture->unbind();
            m_vao->unbind();
            m_shader->unbind();
            DEBUG_PRINT("Rendered in Display Mode Sucessfully");
        }

        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }

        return true;
    }

    /// @brief Render the geometry in selection mode (For picking the geometry)
    bool OpenGL_3_3_RenderKernel::render_selection_mode()
    {
        try 
        {
            if((*m_geometry_descriptor)->positions_vector().size() == 0) return false;
            
            /// Get the pick information
            GLenum pick_scheme = (*m_geometry_descriptor)->get_pick_scheme_enum();
            if(pick_scheme == GL_PICK_NONE) return false;

            GLenum primitive_type = (*m_geometry_descriptor)->get_primitive_type_enum();

            if(pick_scheme == GL_PICK_BY_VERTEX) primitive_type = GL_POINTS;

            if(pick_scheme == GL_PICK_BY_PRIMITIVE || pick_scheme == GL_PICK_BY_VERTEX)
               m_shader = ShaderLibrary::GetShader("SelectPrimitiveShader");

            else if(pick_scheme == GL_PICK_GEOMETRY)
               m_shader = ShaderLibrary::GetShader("SelectGeometryShader");
            
            m_shader->bind();
            SceneState& scene_state = Event::Publisher::GetInstance()->get_scene_state();
          
            /// Set the shader uniforms
            m_shader->SetMat4fv("projection", scene_state.m_projection);
            m_shader->SetMat4fv("model", scene_state.m_model);
            m_shader->SetMat4fv("view", scene_state.m_view);

            if(pick_scheme == GL_PICK_BY_PRIMITIVE || pick_scheme == GL_PICK_BY_VERTEX)
                m_shader->Set1i("selection_init_id", m_geometry_descriptor->get_color_id_reserve_start());  

            else if(pick_scheme == GL_PICK_GEOMETRY)
            {
                uint32_t unique_color = m_geometry_descriptor->get_color_id_reserve_start();
                PixelData color = PixelData(unique_color);
                glm::vec3 unique_color_vec = glm::vec3(color.r_float(), color.g_float(), color.b_float());
                m_shader->SetVec3fv("selection_init_id", unique_color_vec);  
            }
            //// Draw the geometry   
            m_vao->bind();
            
            set_rasteriser_state();

            execute_draw_command(primitive_type);
            
            reset_rasteriser_state();

            // Unbind the all the objects
            m_vao->unbind();
            m_shader->unbind();
            DEBUG_PRINT("Rendered in Select Mode Sucessfully");
        }

        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }

        return true;
    }

    /// @brief Reset the render kernel (For reinitialization of the kernel with new geometry descriptor)
    void OpenGL_3_3_RenderKernel::reset()
    {
        m_geometry_descriptor.reset();
        m_vao.reset();
        m_shader.reset();
        m_texture.reset();
        init_flag = false;
    } 

    void OpenGL_3_3_RenderKernel::set_rasteriser_state()
    {
        if((*m_geometry_descriptor)->get_wireframe_mode_enum() != GL_WIREFRAME_NONE)
        { 
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glEnable(GL_POLYGON_OFFSET_FILL);
         glPolygonOffset(1.0, 1.0);
         glLineWidth(4.0f);
        }
    }

    void OpenGL_3_3_RenderKernel::reset_rasteriser_state()
    {
        if((*m_geometry_descriptor)->get_wireframe_mode_enum() != GL_WIREFRAME_NONE)
        {         
         glDisable(GL_POLYGON_OFFSET_FILL);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glLineWidth(4.0f);
        }
    }
    
    /// @brief Execute the draw command (Just a wrapper for the OpenGL draw commands)
    void OpenGL_3_3_RenderKernel::execute_draw_command(const GLenum& primitive_type)
    {
      GLenum my_primitive_type = primitive_type;  
      
      if(my_primitive_type == GL_NONE_NULL) 
      { my_primitive_type =  (*m_geometry_descriptor)->get_primitive_type_enum(); }

      if(my_primitive_type == GL_NONE_NULL) throw std::runtime_error("Primitive type is not set");
      
      if((*m_geometry_descriptor)->indices_vector().size() != 0)
      {
        Renderer::GL_API()->glDrawElements(my_primitive_type, (*m_geometry_descriptor)->get_num_vertices(), GL_UNSIGNED_INT, nullptr);
      }

      else
      {
        Renderer::GL_API()->glDrawArrays(my_primitive_type, 0, (*m_geometry_descriptor)->get_num_vertices());  
      }
    }

}