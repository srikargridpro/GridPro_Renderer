#include "gp_gui_communications.h"
#include "gp_gui_scene.h"

namespace gridpro_gui
{
  namespace Event
  {
     Publisher::Publisher() : scene_ptr(nullptr)
     {
      PickEvent empty_pick_event;
      KeyEvent empty_key_event;
      MouseEvent empty_mouse_event;

      GlobalMouseEvent.emplace_back(empty_mouse_event); 
      GlobalKeyBoardEvent.emplace_back(empty_key_event);  
      GlobalPickEvent.emplace_back(empty_pick_event); 
     }

    framebuffer* Publisher::frame_buffer()
    {
      return &(this->selection_buffer);
    }
    void Publisher::RegisterSubscriber(const std::string& SubscriberID, Subscription* Subscriber)
    {   
        SubscribersRegistry[SubscriberID] = Subscriber;
    }

    void Publisher::UnregisterSubscriber(const std::string& SubscriberID)
    {
        SubscribersRegistry.erase(SubscriberID);
    } 
   
    MouseEvent& Publisher::RetrieveMouseEvent()
    {
       return GlobalMouseEvent.front();
    }

    KeyEvent& Publisher::RetrieveKeyBoardEvent()
    {
       return GlobalKeyBoardEvent.front();
    }

    PickEvent& Publisher::RetrievePickEvent()
    {
      return GlobalPickEvent.front();
    }
    
    uint32_t Publisher::get_entity_actual_id(const uint32_t color_id)
    {
        return get_scene_ptr()->get_actual_id(color_id);
    }

    /// @brief Get Scene State
    const SceneState::RenderMode Publisher::get_render_mode()
    {
        return get_scene_ptr()->get_render_mode();
    } 

    SceneState& Publisher::get_scene_state()
    {
        return get_scene_ptr()->get_scene_state();
    }

    void Publisher::set_scene_ptr(Gp_gui_scene* scene)
    {
        scene_ptr = scene;
    } 

    Gp_gui_scene* Publisher::get_scene_ptr()
    {  
      if(scene_ptr == nullptr)
        {
            throw std::runtime_error("Scene Pointer is not valid");
        }
        return scene_ptr;
    }    
    }
} // namespace gridpro_gui