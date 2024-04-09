#ifndef GP_GUI_OPENGL_3_3_RENDER_KERNEL_H
#define GP_GUI_OPENGL_3_3_RENDER_KERNEL_H

#include <memory>

namespace gridpro_gui
{
     class GeometryDescriptor;
     class VertexArrayObject;
     class Shader;
     class OpenGLTexture;
     
     namespace Event
     {
         class Publisher;
     }
   
    class OpenGL_3_3_RenderKernel
    {
      public :
      OpenGL_3_3_RenderKernel();
      OpenGL_3_3_RenderKernel(std::shared_ptr<GeometryDescriptor>& geometry_descriptor);
     ~OpenGL_3_3_RenderKernel();
    
      void set_geometry_descriptor(const std::shared_ptr<GeometryDescriptor>& geometry_descriptor);
      std::shared_ptr<GeometryDescriptor>& get_descriptor() { return m_geometry_descriptor; }

      bool render_display_mode();
      bool render_selection_mode();

      void set_kernel_id(uint32_t kernel_id) { m_kernel_id = kernel_id; }
      uint32_t get_kernel_id() { return m_kernel_id; }
      
      private :
      
      void init();
      void reset();
      void execute_draw_command(const GLenum& primitive_type = GL_NONE_NULL);
      void set_rasteriser_state();
      void reset_rasteriser_state();

      // Member Variables
      std::shared_ptr<GeometryDescriptor> m_geometry_descriptor;
      std::shared_ptr<VertexArrayObject>  m_vao;
      std::shared_ptr<Shader>             m_shader;
      std::shared_ptr<OpenGLTexture>      m_texture;
      bool init_flag;
      uint32_t m_kernel_id;
    };
}


#endif // GP_GUI_OPENGL_3_3_RENDER_KERNEL_H