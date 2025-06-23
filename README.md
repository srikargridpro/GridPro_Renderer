# GridPro GFX Module

## Overview

`AbstractViewerWindow Interface Class & GeometryDescriptor class` are the cornerstone interfaces for rendering in the GridPro GFX module. It abstracts away the complexity of OpenGL function access, scene management, picking, and rendering, offering a standardized and high level interface.

It serves as a middleware base for Graphics developers to build 3D apps using App frameworks like Qt (via `QOpenGLWidget`), GLFW, or OpenSceneGraph.

The abstraction accommodates varying OpenGL function loaders — such as `QOpenGLFunctions_4_3_Compatibility` in Qt or `GLEW` in GLFW — while still exposing a unified public API.

## Why Use `AbstractViewerWindow Interface`?

GridPro GFX is designed for high-performance, interactive scientific visualization. To support this while simplifying the development effort:

* `AbstractViewerWindow` offers a **stable and consistent viewer interface**.
* Think of it as the **paper**, and the `GeometryDescriptor` as the **pen**.
* Geometry is committed via a descriptor, and rendering is managed internally.
* GUI developers can subclass this interface and plug in their preferred rendering backend without needing to reinvent rendering logic.

## Core Capabilities

* **Scene Management**: Abstracts a scene graph and layer system with default SceneManager.
* **Orthographic Camera**: A protected default camera simplifies spatial navigation.
* **Geometry Commit System**: Queue-based commit stack for 2D and 3D descriptors.
* **Blending & Lighting Controls**: Easy toggles to manage OpenGL rendering states.
* **Selection & Picking**: Callback interfaces for entity picking, box selection, and double-click events.
* **Workplane Utilities**: Projection and manipulation aids for geometry interaction.
* **Customizable Event Hooks**: Override event handlers to respond to user input.
* **Text Overlay**: Render 2D text in normalized device space.

## Design Philosophy

* **Interface Not Implementation**: The class provides default fallbacks, but real functionality should be implemented in derived viewer classes.
* **Cross-Backend Compatibility**: Qt, GLFW, or OpenSceneGraph backends can be supported with minimal effort.
* **Scene Abstraction**: Geometry and interactions are managed without direct OpenGL calls.
* **Plugin Flexibility**: ImGui or other overlays can be layered in independently.

## Extending AbstractViewerWindow

To create a viewer, subclass `AbstractViewerWindow` and implement key OpenGL-related virtual methods:

```cpp
class MyViewer : public QWidget, public AbstractViewerWindow {
public:
    void initializeGL() override { /* Setup GL context */ }
    void paintGL() override { /* Draw committed geometry */ }
    void update_display() override { update(); }
    void capture_screen_shot(...) override { /* Save framebuffer */ }
    void render_2d_text(...) override { /* Implement text rendering */ }
};
```

## Essential Public API Functions

| Method                                           | Description                               |
| ------------------------------------------------ | ----------------------------------------- |
| `commit_geometry()`                              | Commit 3D geometry to the scene           |
| `commit_2d_geometry()`                           | Commit 2D geometry to the scene           |
| `upload_commits()`                               | Upload all committed descriptors          |
| `remove_geometry()`                              | Remove geometry from the scene            |
| `hide_geometry()` / `show_geometry()`            | Toggle visibility of entities             |
| `set_bounding_box()`                             | Define spatial bounds for framing         |
| `set_blend_state()` / `is_blending_enabled()`    | Control OpenGL blending                   |
| `set_lighting_state()` / `is_lighting_enabled()` | Control OpenGL lighting                   |
| `clear_screen(r, g, b, a)`                       | Clear the display with RGBA color         |
| `zoom_in()` / `zoom_out()`                       | Camera zoom control                       |
| `snap_to_*_plane()`                              | Snap camera to orthogonal/isometric views |
| `render_2d_text(...)`                            | Overlay screen-space text                 |
| `set_pick_callback(...)`                         | Register picking callback                 |

## Internal Components

* `Scene_Manager`: Manages rendering layers and geometry lifecycle.
* `OrthographicCamera`: Built-in camera with panning, zoom, and snapping.
* `GeometryDescriptor`: Encapsulates geometry data and metadata.
* `WorkPlane`: Projected plane with geometry-aligned interaction tools.
* `MouseState`, `Gizmo`, `LightPosition`, and `ScreenDimensions`: Internal state helpers.

## Example: `Viewer` Implementation

The `Viewer` class is a concrete implementation using Qt’s `QOpenGLWidget`:

### Key Features

* Integrates with Qt’s event system
* Uses `QOpenGLFunctions_4_3_Compatibility`
* Adds ImGui widget support
* Emits signals for entity picking and interaction

### Common API Functions

| Method                                          | Description                             |
| ----------------------------------------------- | --------------------------------------- |
| `commit_geometry()`                             | Commit geometry to the scene            |
| `set_bounding_box()`                            | Define the spatial bounds for the scene |
| `is_blending_enabled()`                         | Check if blending is currently active   |
| `set_blend_state(bool)`                         | Enable or disable blending              |
| `is_lighting_enabled()`                         | Check if lighting is currently active   |
| `set_lighting_state(bool)`                      | Enable or disable lighting              |
| `capture_screen_shot(res_scale_x, res_scale_y)` | Save a high-res screenshot              |
| `render_2d_text(nx, ny, text, size, color)`     | Render screen-aligned text              |
| `add_imgui_widget(name, widget)`                | Add ImGui widget to the render loop     |
| `remove_imgui_widget(name)`                     | Remove ImGui widget                     |

### Event Hooks

* `mousePressEvent`, `mouseMoveEvent`, `mouseReleaseEvent`
* `keyPressEvent`, `keyReleaseEvent`, `wheelEvent`, `paintEvent`

### Example Usage

```cpp
Viewer* viewer = new Viewer(parent);
viewer->commit_geometry(...);
viewer->set_bounding_box(...);
viewer->set_blend_state(true);
viewer->show();
```

### Signals

| Signal                                                            | Description                                     |
| ----------------------------------------------------------------- | ----------------------------------------------- |
| `picked(data)`                                                    | Triggered when an entity is picked in the scene |
| `gl_entity_picked(entity_key, sub_entity, button, key_held, pos)` | Detailed picking information                    |

## Intended Users

* **Scientific Programmers**: Can use the interface to visualize complex mesh and geometry without worrying about OpenGL or GPU management.
* **GUI Developers**: Can extend or replace internals (scene manager, camera, UI integrations) for custom workflows.

## Notes

* Do not instantiate `AbstractViewerWindow` directly.
* Combine it with a GUI widget like `QOpenGLWidget` or your rendering surface of choice.
* The interface offers a powerful middle layer between raw OpenGL and high-level scientific visualization.

---

For full API reference and examples, refer to the [GridPro GFX documentation site](#) or the module source.



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
