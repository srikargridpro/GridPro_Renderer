#ifndef GP_GUI_SHADER_SRC_H
#define GP_GUI_SHADER_SRC_H


namespace gridpro_gui {

namespace ShaderSrc {

// Shader Name : Default Flat Shader
static const char* BasicVertexShaderSource = R"(

    #version 430 core

    layout(location = 0) in vec3 VertexPos;

    uniform mat4 projection;
    uniform mat4 model; 
    uniform mat4 view; 

    void main()
    {    
       gl_Position = projection * view * model * vec4(VertexPos, 1.0); 
    }
)";

static const char* BasicFragmentShaderSource = R"(

    #version 430 core

    out vec4 FragColor;

 // uniform sampler2D textureSampler;

    uniform vec4 object_color;
 
    void main()
    {  
       uint TriangleID = gl_PrimitiveID;
       
       vec3 mycolor = object_color.rgb;

       vec3 fragPos = gl_FragCoord.xyz;
       float depth  = gl_FragCoord.z;

       vec3 dimming_factor = vec3(depth/2, depth/2, depth/2);
       mycolor = mycolor - dimming_factor;
      
       FragColor = vec4(mycolor, object_color.a);

     //FragColor = texture(textureSampler, uv);
    }
)";


static const char* SelectPrimitiveVertexShaderSource = R"(

    #version 430 core

    layout(location = 0) in vec3 VertexPos;

    uniform mat4 projection;
    uniform mat4 model; 
    uniform mat4 view; 
    
    void main()
    {            
      gl_Position = projection * view * model * vec4(VertexPos, 1.0);
    }
)";

static const char* SelectPrimitiveFragmentShaderSource = R"(

    #version 430 core
    
    out vec4 FragColor;

    uniform int selection_init_id;
    
    void main()
    {  
      uint id = uint(selection_init_id);

      uint PrimID = gl_PrimitiveID + id;

      vec3 unique_color = vec3(1.0, 1.0, 1.0);

      unique_color.b = float((PrimID >> 16) & 0xFF) / 255.0;
      unique_color.g = float((PrimID >> 8)  & 0xFF) / 255.0;
      unique_color.r = float(PrimID & 0xFF) / 255.0;
 
      FragColor = vec4(unique_color, 1.0f);
    }
)";

static const char* SelectGeometryVertexShaderSource = R"(

    #version 430 core

    layout(location = 0) in vec3 VertexPos;

    uniform mat4 projection;
    uniform mat4 model; 
    uniform mat4 view; 
    
    void main()
    {            
      gl_Position = projection * view * model * vec4(VertexPos, 1.0);
    }
)";

static const char* SelectGeometryFragmentShaderSource = R"(

    #version 430 core
    
    out vec4 FragColor;

    uniform vec3 selection_init_id;
    
    void main()
    {  
      FragColor = vec4(selection_init_id, 1.0f);
    }
)";

}

} // namespace gridpro_gui
#endif // GP_GUI_SHADER_SRC_H