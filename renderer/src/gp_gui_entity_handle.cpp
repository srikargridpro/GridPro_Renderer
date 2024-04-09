#include "gp_gui_entity_handle.h"
#include "gp_gui_scene.h"
#include "gp_gui_typedefs.h"
#include "gp_gui_forward_structs.h"

namespace gridpro_gui
{

    Gp_gui_entity_handle::Gp_gui_entity_handle() : entity_ptr(nullptr), scene_ptr(nullptr)
    {
    
    }

    Gp_gui_entity_handle::~Gp_gui_entity_handle()
    {
     
    }

    bool Gp_gui_entity_handle::is_valid() const
    {
        return entity_ptr->is_valid() && scene_ptr != nullptr;
    }

    void Gp_gui_entity_handle::destroy()
    {
        if(entity_ptr == nullptr || scene_ptr == nullptr)
        {
            std::cout << "Entity or Scene is not valid" << std::endl;
            return;
        } 
        scene_ptr->remove_entity_from_registry(entity_key);  
        entity_ptr->destroy();
    }

    const std::string& Gp_gui_entity_handle::get_key() const
    {
        return entity_key;
    }


} // namespace gridpro_gui