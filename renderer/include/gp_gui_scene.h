#ifndef GP_GUI_SCENE_H
#define GP_GUI_SCENE_H

#include "ecs.h"
#include <unordered_map>
#include <deque>
#include "gp_gui_forward_structs.h"
#include "gp_gui_communications.h"

namespace gridpro_gui
{

    /// @brief Scene class
    /// The scene class is the main
    /// class that holds the entities
    /// and the systems that are used to render the entities
    /// It also holds the color reservations for assigning pick ids to entities 
    /// and the scene state like light, camera, render mode etc.
    namespace Event
    {
        class Publisher;
    }
    
    class Gp_gui_scene
    {
    public:
         Gp_gui_scene();
        ~Gp_gui_scene();

         // The Only Functions you'll ever need ------------------------+
         /// Adds an entity to the scene
         Gp_gui_entity_handle get_entity(const std::string& entity_key);
         /// Updates all Systems
         void update(const float& layer);
         ///------------------------------------------------------------+

         void update_mouse_event(const float& x, const float& y);

         void update_color_reservations();
         uint32_t get_actual_id(const uint32_t& color_id);

         bool has_entity(const std::string& entity_key);
         bool remove_entity_from_registry(const std::string& entity_key);
         
         const SceneState::RenderMode get_render_mode() const;
         SceneState& get_scene_state();
         
         void set_system_state(const bool& state);
         const bool get_system_state();
         const bool flip_system_state();

         void set_mvp(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
         
     public :
     std::deque<ecs::Entity> Entity_DataBase;
     std::unordered_map<std::string, uint32_t> SceneEntityRegistry;
     std::unordered_map<uint32_t, std::string> EntityIdxKeyMapRegistry;
     std::unordered_map<uint32_t, unique_color_reservation> unique_colr_reservations;
     ecs::EntityManager RenderableEntitiesManager;
     ecs::SystemManager RenderSystemsManager;

     uint32_t last_color_id;    
     
     Event::Publisher*  PublisherInstance;

     private:
     mutable SceneState m_scene_state_obj;
     
    };

} // namespace gridpro_gui


#endif // GP_GUI_SCENE_H