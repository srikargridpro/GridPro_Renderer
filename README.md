# GridPro Renderer
GridPro WS internal Renderer API Abstraction.

## Table of Contents

- [About](#about)
- [Getting Started](#getting-started)
- [API Reference](#api-reference)
- [Examples](#examples)

## About
Renderer Lib Provides a API independent abstraction like OpenScene Graph and VTU but oriented towards Engineering Geometry Rendering.




## API Reference

For detailed information about the API provided by the HPS Application Framework, consult the [API Reference](API-Reference.md). This reference guide includes comprehensive documentation for all available classes, methods, and modules.


## Examples

To see practical examples of how to use the HPS Application Framework, refer to the [Examples](./examples) directory in this repository. You'll find a variety of sample projects that demonstrate different aspects of the framework.


---

## Getting Started

```cpp 

	//    +-----------------------------------------------------------------------------------+
  //    Newer Scene API
	//    +-----------------------------------------------------------------------------------+
	      //   try 
	      //   {
	      //   	   Gp_gui_scene& Scene = *(view->get_scene()); 
	      //       Instrumentation::Stopwatch timer("Updating Data");    
 
        //       std::shared_ptr<GeometryDescriptor> test1 = std::make_shared<GeometryDescriptor>();
		    // 	     std::string triangle_name1 = "Triangle1";
		    // 	     (test1)->set_current_primitive_set(triangle_name1, GL_QUADS);
			  
        //       // Vertex positions for the cube (each vertex appears only once)
        //       std::vector<float> positions1 = {
        //       -1.0f, -1.0f, -1.0f, // Vertex 0
        //        1.0f, -1.0f, -1.0f, // Vertex 1
        //        1.0f,  1.0f, -1.0f, // Vertex 2
        //       -1.0f,  1.0f, -1.0f, // Vertex 3
        //       -1.0f, -1.0f,  1.0f, // Vertex 4
        //        1.0f, -1.0f,  1.0f, // Vertex 5
        //        1.0f,  1.0f,  1.0f, // Vertex 6
        //       -1.0f,  1.0f,  1.0f  // Vertex 7
        //       };

        //      // Index array defining the order in which vertices are connected to form quads
        //      std::vector<uint32_t> indices1 = {
        //       // Front face
        //       4, 5, 6, 7,
        //       // Back face
        //       0, 3, 2, 1,
        //       // Left face
        //       0, 4, 7, 3,
        //       // Right face
        //       1, 2, 6, 5,
        //       // Top face
        //       3, 7, 6, 2,
        //       // Bottom face
        //       0, 1, 5, 4
        //      };
        //    (test1)->move_pos_array(std::move(positions1));
        //    (test1)->move_index_array(std::move(indices1));
        //    (*test1)->set_wireframe_mode(GL_WIREFRAME_NONE
      
        //    (*test1)->set_pick_scheme(GL_PICK_BY_PRIMITIVE);

        //    Gp_gui_entity_handle triangle_handle1 = Scene.get_entity(triangle_name1);
        //    OpenGL_3_3_RenderKernel* triangle_render_kernel1 = triangle_handle1.GetComponent<OpenGL_3_3_RenderKernel>();

        //    triangle_render_kernel1->set_geometry_descriptor(test1);	

        //    Scene.update(1.0f);
        //   }

        //   catch (const std::exception& e)
        //   {	
        //   	std::cout << "Exception : " << e.what() << std::endl;
        //   }
...

![sample code output (dark, segoeui font, freetype)](https://user-images.githubusercontent.com/8225057/191050833-b7ecf528-bfae-4a9f-ac1b-f3d83437a2f4.png)
![sample code output (light, segoeui font, freetype)](https://user-images.githubusercontent.com/8225057/191050838-8742efd4-504d-4334-a9a2-e756d15bc2ab.png)

...

Feel free to explore the documentation and examples to make the most of the HPS Application Framework.
If you encounter any issues or have questions, please don't hesitate to [open an issue](https://github.com/ksrikar1234/HPS_API_Documentation/issues) on GitHub.

Happy coding!
