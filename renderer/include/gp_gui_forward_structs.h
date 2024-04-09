#ifndef GP_GUI_FORWARD_STRUCTS_H
#define GP_GUI_FORWARD_STRUCTS_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdint>

namespace gridpro_gui
{
    class Gp_gui_entity_handle;
    class Gp_gui_scene;
    // unique color reservation
    class unique_color_reservation
    { 
      public :
      uint32_t _EntityID_;
      uint32_t _Min_ColorID_, _Max_ColorID_;

      unique_color_reservation() {}

      unique_color_reservation(uint32_t EntityID, uint32_t Min_ColorID, uint32_t Max_ColorID)
      : _EntityID_(EntityID), _Min_ColorID_(Min_ColorID), _Max_ColorID_(Max_ColorID) {}

    };

    struct SceneState 
    {
      SceneState() : m_render_mode(HLM_NONE), m_projection(glm::mat4(1.0f)), m_view(glm::mat4(1.0f)), m_model(glm::mat4(1.0f)), render_systems_enabled(true) {}
     ~SceneState() {}
      enum RenderMode { HLM_NONE = 0 , HLM_RENDER = 1, HLM_SELECT = 2, HLM_RENDER_AND_SELECT = 3 }; 
      /// Scene Render Mode
      RenderMode m_render_mode;
      /// Scene MVP
      glm::mat4 m_projection, m_view , m_model;

      /// Scene Light Properties
      glm::vec3 Position;
      glm::vec3 Ambient, Diffuse, Specular;

      bool render_systems_enabled;
      bool is_render_systems_enabled()   { return render_systems_enabled; }
      bool flip_render_systems_switch()  { render_systems_enabled = !render_systems_enabled; return render_systems_enabled; }
   
      bool set_render_systems_switch(const bool& input_state) { render_systems_enabled = input_state; return render_systems_enabled; }
      bool get_render_systems_switch() { return render_systems_enabled; }
      bool enable_render_systems()  { render_systems_enabled = true;  return render_systems_enabled;   }
      bool disable_render_systems() { render_systems_enabled = false; return render_systems_enabled;   }
      const RenderMode get_render_mode() {  return m_render_mode; }

     };

} // namespace gridpro_gui
#endif // GP_GUI_FORWARD_STRUCTS_H    
