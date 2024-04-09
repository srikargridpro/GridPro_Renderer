#ifndef _GP_GUI_COMMUNICATIONS_H_
#define _GP_GUI_COMMUNICATIONS_H_

#include <unordered_map>
#include <deque>
#include <string>


#include "gp_gui_forward_structs.h"
#include "gp_gui_framebuffer.h"
#include "gp_gui_events.h"

namespace gridpro_gui
{
/// @brief Publisher and Subscriber classes
/// Publisher is a singleton class that is used to publish events
/// It has a registry of subscribers that are interested in the events
/// It is the only class that can publish events
/// It is used as primary communication between the scene and the user
/// It has a pointer to the scene
/// It has a pointer to the selection buffer
/// It has a deque of mouse events, keyboard events and pick events.
/// It has a notifier that is used to check if the event has been updated

class Gp_gui_scene;

namespace Event {

class Subscription;

class Publisher {
public:
   
    static Publisher* GetInstance()
    {
        static Publisher SingletonPublisher;
        return &SingletonPublisher;
    }

    framebuffer* frame_buffer();
    
    void set_scene_ptr(Gp_gui_scene* scene_ptr); 

    Gp_gui_scene* get_scene_ptr();
    
    void RegisterSubscriber(const std::string& SubscriberID, Subscription* Subscriber);

    void UnregisterSubscriber(const std::string& SubscriberID);
   
    MouseEvent& RetrieveMouseEvent();

    KeyEvent& RetrieveKeyBoardEvent();

    PickEvent& RetrievePickEvent();
    
    uint32_t get_entity_actual_id(const uint32_t color_id);

    public:

    std::unordered_map<std::string, Subscription*> SubscribersRegistry;
    std::unordered_map<uint32_t, std::string>*     EntityIdxKeyMapRegistry;

    private :
    friend class Gp_gui_scene;
    Gp_gui_scene* scene_ptr; 
    Publisher();

    framebuffer selection_buffer;
    public :
    std::deque<MouseEvent> GlobalMouseEvent;
    std::deque<KeyEvent>   GlobalKeyBoardEvent; 
    std::deque<PickEvent>  GlobalPickEvent;

    public :
    bool notifier;
    /// @brief Updates Notifiers of subscribptions
    inline void updateNotifiers(); 
    /// @brief Get Scene State
    const SceneState::RenderMode get_render_mode(); 
    SceneState& get_scene_state();
    
};


/// @brief Subscription class
/// Subscription is a class that is used to subscribe to events
/// It is used to get the events from the publisher
/// It has a notifier that is used to check if the event has been updated
/// It has a unique subscriber name that is used to identify the subscriber
/// It has a pointer to the publisher instance
/// It has a pointer to the last mouse event, keyboard event and pick event

class Subscription 
{
public:
Subscription(const std::string SubcriberName) : SubcriberUserID(SubcriberName) 
{ 
    PublisherInstance = Publisher::GetInstance();
    PublisherInstance->RegisterSubscriber(SubcriberName, this); 
    notifier = true; 
}

~Subscription() 
{
  PublisherInstance->UnregisterSubscriber(SubcriberUserID);
}

MouseEvent& getMouseEvent()
{ 
    if(isNotifierUpdated()) 
      {
        last_mouse_event = PublisherInstance->RetrieveMouseEvent();
        return PublisherInstance->RetrieveMouseEvent();
      }
    last_mouse_event.SetEventType(EventType::None);  
    return last_mouse_event; 
}

KeyEvent& getKeyBoardEvent()
{
    if(isNotifierUpdated()) 
      {
        last_keyboard_event = PublisherInstance->RetrieveKeyBoardEvent();
        return PublisherInstance->RetrieveKeyBoardEvent();
      }

    last_keyboard_event.SetEventType(EventType::None);
    return last_keyboard_event; 

}

PickEvent& getPickEvent()
{
    if(isNotifierUpdated()) 
      {
        last_pick_event = PublisherInstance->RetrievePickEvent();
        return PublisherInstance->RetrievePickEvent();
      }
    last_pick_event.SetEventType(EventType::None);
    return last_pick_event; 

}

bool isNotifierUpdated()
{
    return notifier;
}

private : 
friend class Publisher;
Publisher* PublisherInstance;
std::string SubcriberUserID;
bool notifier;
MouseEvent   last_mouse_event;
KeyEvent     last_keyboard_event;
PickEvent    last_pick_event;
std::string  notification_message;

};

inline void Publisher::updateNotifiers() 
{
    for(std::unordered_map<std::string, Subscription*>::iterator it = SubscribersRegistry.begin(); it != SubscribersRegistry.end(); ++it)
           it->second->notifier = true;
}
 

} // namespace Event

} // namespace gridpro_gui
#endif // GP_GUI_COMMUNICATIONS_H    