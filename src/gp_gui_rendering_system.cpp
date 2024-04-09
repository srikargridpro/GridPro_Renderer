#include "gp_gui_renderer_api.h"
#include "gp_gui_scene.h"
#include "gp_gui_entity_handle.h"
#include "gp_gui_rendering_system.h"
#include "gp_gui_geometry_descriptor.h"
#include "gp_gui_opengl_3_3_render_kernel.h"
#include "gp_gui_communications.h"
#include "gp_gui_instrumentation.h"
#include <iostream>


namespace gridpro_gui {

void OpenGL_3_3_RenderSystem::update(float layer)
{
    Instrumentation::Stopwatch watch("OpenGL_3_3_RenderSystem::update");
    DEBUG_PRINT("Entities count = ",  entities().count() , "\n");
    
    for(auto Entity : entities().with<OpenGL_3_3_RenderKernel>())
    { 
        auto& render_kernel = Entity.get<OpenGL_3_3_RenderKernel>();
        render_kernel.render_selection_mode();
    }

    Event::Publisher::GetInstance()->frame_buffer()->update_current_frame_buffer();   
    
    // for(auto Entity : entities().with<OpenGL_3_3_RenderKernel>())
    // { 
    //     auto& render_kernel = Entity.get<OpenGL_3_3_RenderKernel>();
    //     render_kernel.render_display_mode();
    // }
}

} // namespace gridpro_gui    
