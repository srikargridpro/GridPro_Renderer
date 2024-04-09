#ifndef GP_GUI_ENTITY_HANDLE_H
#define GP_GUI_ENTITY_HANDLE_H

#include <string>
#include "ecs.h"
#include "gp_gui_typedefs.h"
#include "gp_gui_forward_structs.h"


namespace gridpro_gui
{
    class Gp_gui_scene;
    
    class Gp_gui_entity_handle
    {
    public:
         Gp_gui_entity_handle();
        ~Gp_gui_entity_handle();
         
         ///@brief Get the component of the entity
         template<typename C>
         C* GetComponent()
         {
           if(entity_ptr == nullptr || (entity_ptr->is_valid() == false))
           {
               throw std::runtime_error("Entity is not valid");
           }

           else if(entity_ptr->has<C>())
               return &entity_ptr->get<C>();
           else
              throw std::runtime_error("Entity does not have the component" + std::string(typeid(C).name()));
         
         }

         ///@brief Set the component of the entity
         template<typename C>
         C* SetComponent()
         {
           if (entity_ptr == nullptr || (entity_ptr->is_valid() == false))
           {
             throw std::runtime_error("Entity is not valid");
           }

           try
           {
             if (entity_ptr->has<C>())
             {
                 C& return_component = entity_ptr->get<C>();
                 return &return_component;
             }

             else
             {
                 entity_ptr->set<C>();
                 C& return_component = entity_ptr->get<C>();
                 return &return_component;
             }
           }
           
           catch (const std::exception& e)
           {
             std::cerr << e.what() << '\n';
             throw e;
           }
         }

         bool is_valid() const;
         void destroy();
         const std::string& get_key() const;
         
    private:
        friend class  Gp_gui_scene;
        std::string   entity_key;
        ecs::Entity*  entity_ptr;
        Gp_gui_scene* scene_ptr;
    };

} // namespace gridpro_gui


#endif // GP_GUI_ENTITY_HANDLE_H