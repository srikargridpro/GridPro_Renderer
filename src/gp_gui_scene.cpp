

#include "gp_gui_entity_handle.h" // Warning : This has Circular Dependency with gp_gui_scene.h
#include "gp_gui_scene.h"
#include "gp_gui_geometry_descriptor.h"
#include "gp_gui_rendering_system.h"
#include "gp_gui_opengl_3_3_render_kernel.h"
#include "gp_gui_debug.h"
#include "gp_gui_shader.h"
#include "gp_gui_shader_src.h"

namespace gridpro_gui 
{
 
Gp_gui_scene::Gp_gui_scene() : RenderSystemsManager(RenderableEntitiesManager) , PublisherInstance(Event::Publisher::GetInstance())
{
   // Critical Do not remove this line  !!!
   PublisherInstance->set_scene_ptr(this); 
   RenderSystemsManager.add<OpenGL_3_3_RenderSystem>(); 
   last_color_id = 0;
   
   try
   {
        ShaderLibrary::AddShader("BasicShader", ShaderSrc::BasicVertexShaderSource, ShaderSrc::BasicFragmentShaderSource);
        ShaderLibrary::AddShader("SelectGeometryShader", ShaderSrc::SelectGeometryVertexShaderSource, ShaderSrc::SelectGeometryFragmentShaderSource);
        ShaderLibrary::AddShader("SelectPrimitiveShader", ShaderSrc::SelectPrimitiveVertexShaderSource, ShaderSrc::SelectPrimitiveFragmentShaderSource);
   }

   catch(const std::exception& e)
   {
       std::cerr << e.what() << '\n';
   }
   
}
    
Gp_gui_scene::~Gp_gui_scene() 
{

}

/// @brief Update the scene
/// @param layer
/// @details  Use this function to update the scene
void Gp_gui_scene::update(const float& layer)
{  
   Event::Subscription scene_subscription("scene");
   
   if(!get_scene_state().is_render_systems_enabled()) 
   {  
     scene_subscription.getPickEvent().setColorID(0);
     scene_subscription.getPickEvent().SetEventType(EventType::None);
     DEBUG_PRINT("Returning from Scene Update as Render Systems are Disabled");
     return;
   }

    update_color_reservations();
    RenderSystemsManager.update(layer);

    uint32_t color_id =  scene_subscription.getPickEvent().getColorID();
    if((color_id) != 0 && color_id <=  last_color_id)
    {
      scene_subscription.getPickEvent().setEntityKey(EntityIdxKeyMapRegistry[get_actual_id(color_id)]);
      scene_subscription.getPickEvent().setEntityID(get_actual_id(color_id));
      scene_subscription.getPickEvent().setSubEntityID(color_id - (unique_colr_reservations[get_actual_id(color_id)]._Min_ColorID_));
    }
  
    DEBUG_PRINT("Updated");

}

void Gp_gui_scene::update_mouse_event(const float& x, const float& y)
{
   MouseEvent mouse_event;
   mouse_event.mouseX = x;
   mouse_event.mouseY = y;

   PublisherInstance->GlobalMouseEvent.emplace_back(mouse_event);
   
   /// @brief Keep track of last 6 mouse events
   if(PublisherInstance->GlobalMouseEvent.size() > 6)
   {
        PublisherInstance->GlobalMouseEvent.pop_back();
   }
 
  /// @brief  Get the pick event and update it
  Event::Subscription scene_subscription("scene");
  uint32_t color_id =  PublisherInstance->frame_buffer()->color_id_at(x,y);
  float depth =  PublisherInstance->frame_buffer()->last_hit_depth();

  scene_subscription.getPickEvent().setColorID(0);
  scene_subscription.getPickEvent().SetEventType(EventType::None);
  scene_subscription.getPickEvent().setEntityKey("NULL_ENTITY");
  scene_subscription.getPickEvent().setDepth(PublisherInstance->frame_buffer()->depth_at(x,y));

  if(!get_scene_state().is_render_systems_enabled()) 
  {  
     return;
  }

  if((color_id) != 0 && color_id <=  last_color_id)
  {
    scene_subscription.getPickEvent().setColorID(color_id);
    scene_subscription.getPickEvent().SetEventType(EventType::PickedEntity);
    scene_subscription.getPickEvent().setEntityKey(EntityIdxKeyMapRegistry[get_actual_id(color_id)]);
    scene_subscription.getPickEvent().setEntityID(get_actual_id(color_id));
    scene_subscription.getPickEvent().setSubEntityID(color_id - (unique_colr_reservations[get_actual_id(color_id)]._Min_ColorID_));
    scene_subscription.getPickEvent().setDepth(depth);  
  }
}

/// @brief Check if the entity exists in the scene
/// @param entity_key
/// @return bool
/// @details  Use this function to check if the entity exists in the scene
bool Gp_gui_scene::has_entity(const std::string& entity_key)
{
    std::unordered_map<std::string, uint32_t>::iterator it  = this->SceneEntityRegistry.find(entity_key); 
    if( it != SceneEntityRegistry.end() )
    {
        return true;
    }
    else
    {
        return false;
    }
}


/// @brief Get the entity object from the scene
/// @param entity_key
/// @return Gp_gui_entity_handle
/// @note If the entity with the given key does not exist, it will be created
/// @note If the entity with the given key exists, it will be retrieved
/// @details  Use this function to get the entity object from the scene
Gp_gui_entity_handle Gp_gui_scene::get_entity(const std::string& entity_key)
{
// Check if key exists
std::unordered_map<std::string, uint32_t>::iterator it  = this->SceneEntityRegistry.find(entity_key); 

// If key Exists Retrieve it
if( it != SceneEntityRegistry.end() )
{
    Gp_gui_entity_handle entt_handle;
    entt_handle.entity_ptr = &Entity_DataBase[SceneEntityRegistry[entity_key]];
    entt_handle.scene_ptr = this;
    entt_handle.entity_key = entity_key; 
    
    return entt_handle;
}
// Else Create a new one
else
{   
    DEBUG_PRINT("Creating Entity : ", entity_key);
    DEBUG_PRINT("Total Entity Count : ", (Entity_DataBase.size()));

    uint32_t curr_assign_id = (Entity_DataBase.size());

    // Register the Entity key-idx pairs
    SceneEntityRegistry[entity_key] = curr_assign_id; /// store Entity key index pair in registry 
    EntityIdxKeyMapRegistry[curr_assign_id] = entity_key;

    // Create and store entity in the entity deque
    Entity_DataBase.emplace_back(this->RenderableEntitiesManager.create());
    
    // Add a geometry descriptor Component to the entity
    Entity_DataBase.back().add<OpenGL_3_3_RenderKernel>();
    Entity_DataBase.back().get<OpenGL_3_3_RenderKernel>().set_kernel_id(curr_assign_id);

    // Create a indirect EntityHandle and return it
    Gp_gui_entity_handle entt_handle;
    entt_handle.entity_ptr =  &(Entity_DataBase.back()); 
    entt_handle.scene_ptr = this;
    entt_handle.entity_key = entity_key;
    
    DEBUG_PRINT("Success in Entity Creation : ", entity_key);
    
    return entt_handle;
}
}

/// @details Gp_gui_scene::remove_entity_from_registry()
/// @brief Remove the entity from the scene
/// @param entity_key
/// @return bool
/// @details  Use this function to remove the entity from the scene
bool Gp_gui_scene::remove_entity_from_registry(const std::string& entity_key)
{
// Reassigning IDs 
std::unordered_map<std::string, uint32_t>::iterator idx = SceneEntityRegistry.begin();
while(idx !=  SceneEntityRegistry.end())
{
    if(idx->second > SceneEntityRegistry[entity_key])
    {
        --(idx->second);
    }
    ++idx;
}
 
std::deque<ecs::Entity>::iterator it = Entity_DataBase.begin();

while ((*it != (Entity_DataBase[SceneEntityRegistry[entity_key]])))
{
     if(it == Entity_DataBase.end()) break;
    ++it;
}

if(it != Entity_DataBase.end())
{
    Entity_DataBase.erase(it);
    EntityIdxKeyMapRegistry.erase(SceneEntityRegistry[entity_key]);
    unique_colr_reservations.erase(SceneEntityRegistry[entity_key]);
    SceneEntityRegistry.erase(entity_key);
    return true;
}

return false;
}



/// @brief Update the color reservations
/// @details  Use this function to update the color reservations
/// @note This function is called before the scene is updated so that pick ids are reserved for each entity properly
void Gp_gui_scene::update_color_reservations()
{
  std::deque<ecs::Entity>::iterator end = Entity_DataBase.end();
  
  std::deque<ecs::Entity>::iterator it  = Entity_DataBase.begin();

  uint32_t reserved_color_id_end  = 1000000;

  for(it; it != end; ++it)
     {
       // Temporary Color reservation
       unique_color_reservation  colr_reserv;

       // Set its Minima of color id based on previous entity max color id
       colr_reserv._Min_ColorID_ = reserved_color_id_end;
       
       // get entity's mesh component
       GeometryDescriptor* Mesh =  (it->get<OpenGL_3_3_RenderKernel>().get_descriptor().get());
       
       // set color reservation id so that we can know to whom the reservation belongs to 
       colr_reserv._EntityID_ = it->get<OpenGL_3_3_RenderKernel>().get_kernel_id();     
       
       // Based on previous entitiy mac color id,  let the mesh calculate how many ids it needs
       Mesh->set_color_id_reserve_start(reserved_color_id_end);
       
       // reflect the same in the color reservation component
       colr_reserv._Max_ColorID_ = Mesh->get_color_id_reserve_end();

       // After the mesh calculates the required ids by the current mesh set minima for next entity as max of previous entity  
       reserved_color_id_end = colr_reserv._Max_ColorID_ + 1;
    
       unique_colr_reservations[colr_reserv._EntityID_] = colr_reserv;

       DEBUG_PRINT("RESERVED IDS for Entity" , EntityIdxKeyMapRegistry[colr_reserv._EntityID_] , " = " , colr_reserv._Min_ColorID_ , ", " , colr_reserv._Max_ColorID_ );  
     } 

     last_color_id = reserved_color_id_end - 1; 
}

uint32_t Gp_gui_scene::get_actual_id(const uint32_t& color_id)
{
   std::unordered_map<uint32_t, unique_color_reservation>::iterator it       =  unique_colr_reservations.begin();
   std::unordered_map<uint32_t, unique_color_reservation>::iterator iter_end =  unique_colr_reservations.end();

   for(it; it != iter_end; ++it)
    {
       if(color_id >= it->second._Min_ColorID_ && color_id <= it->second._Max_ColorID_)
       {
          return it->second._EntityID_;
       }
    }

   return 0;
}

void Gp_gui_scene::set_system_state(const bool& state)
{
    m_scene_state_obj.set_render_systems_switch(state);
}

const bool Gp_gui_scene::flip_system_state()
{
    return m_scene_state_obj.flip_render_systems_switch();
}

const bool Gp_gui_scene::get_system_state()
{
    return m_scene_state_obj.get_render_systems_switch();
} 

const SceneState::RenderMode Gp_gui_scene::get_render_mode() const
{
    return m_scene_state_obj.get_render_mode();
}

SceneState& Gp_gui_scene::get_scene_state()
{
    return m_scene_state_obj;
}

void Gp_gui_scene::set_mvp(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
{
    m_scene_state_obj.m_projection = projection;
    m_scene_state_obj.m_view = view;
    m_scene_state_obj.m_model = model;
}

}// namespace gridpro_gui