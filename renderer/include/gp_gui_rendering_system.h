#ifndef GP_GUI_RENDERING_SYSTEM_H
#define GP_GUI_RENDERING_SYSTEM_H

#include "gp_gui_renderer_api.h"
#include "gp_gui_scene.h"
#include "gp_gui_entity_handle.h"
#include "ecs.h"

namespace gridpro_gui
{
   class OpenGL_3_3_RenderSystem : public ecs::System
   {
     public :
     void update(float layer) override;
   };
}

#endif // GP_GUI_RENDERING_SYSTEM_H