# GridPro Renderer
GridPro WS internal Renderer API Abstraction.

## Table of Contents

- [About](#about)
- [Getting Started](#getting-started)
- [API Reference](#api-reference)
- [Examples](#examples)

## About
Renderer Lib Provides a API independent abstraction like OpenScene Graph and VTU but oriented towards Engineering Geometry Rendering.

## Getting Started
```cpp 

	//    +-----------------------------------------------------------------------------------+
        //    Newer Scene API
	//    +-----------------------------------------------------------------------------------+
	  
               // Vertex positions for the cube (each vertex appears only once)
               std::vector<float> positions =
               {
                 -1.0f, -1.0f, -1.0f, // Vertex 0
                  1.0f, -1.0f, -1.0f, // Vertex 1
                  1.0f,  1.0f, -1.0f, // Vertex 2
                 -1.0f,  1.0f, -1.0f, // Vertex 3
                 -1.0f, -1.0f,  1.0f, // Vertex 4
                  1.0f, -1.0f,  1.0f, // Vertex 5
                  1.0f,  1.0f,  1.0f, // Vertex 6
                 -1.0f,  1.0f,  1.0f  // Vertex 7
               };

               // Index array defining the order in which vertices are connected to form quads
               std::vector<uint32_t> indices =
               {
                 // Front face
                 4, 5, 6, 7,
                 // Back face
                 0, 3, 2, 1,
                 // Left face
                 0, 4, 7, 3,
                 // Right face
                 1, 2, 6, 5,
                 // Top face
                 3, 7, 6, 2,
                 // Bottom face
                 0, 1, 5, 4
                };
              
            try 
            {                 
               // Create a Geometry Descriptor 
               std::shared_ptr<GeometryDescriptor> cube_descriptor = std::make_shared<GeometryDescriptor>();
               std::string cube_name = "Cube1";
               (cube_descriptor)->set_current_primitive_set(cube_name, GL_QUADS);
		
               // Set the Basic Requirements like VertexData
               (cube_descriptor)->move_pos_array(std::move(positions));
               (cube_descriptor)->move_index_array(std::move(indices));
               (cube_descriptor)->set_wireframe_mode(GL_WIREFRAME_NONE);
               (cube_descriptor)->set_pick_scheme(GL_PICK_BY_PRIMITIVE);

               // Commit Geometry to scene
               viewer()->commit_geometry(cube_name, cube_descriptor);

               // Update the Scene
               Scene->update(1.0f);
           }

           catch(const std::exception& e)
           {	
           	std::cout << "Exception : " << e.what() << std::endl;
           }
```
If you encounter any issues or have questions, please don't hesitate to [open an issue](https://github.com/ksrikar1234/HPS_API_Documentation/issues) on GitHub.

Happy coding!
