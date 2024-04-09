#ifndef GP_GUI_SHADER_H
#define GP_GUI_SHADER_H

#include "gp_gui_renderer_api.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>

namespace gridpro_gui
{

class Shader
{
public:
	/// @brief	creates, loads and activates a shader program to run on GPU
        __INLINE__ Shader();
	/// @brief Read Shader from file 
	__INLINE__ Shader(const std::string& filePathVertexShader, const std::string& filePathFragmentShader);
	/// @brief Read Shader from c strings 
        __INLINE__ Shader(const char* VertexShaderSource, const char* FragmentShaderSource);
	/// @brief	destroys the shader program
	__INLINE__ ~Shader();

	/// @brief auto cast to program value 
        __INLINE__ operator GLint() const;
	/// @brief	binds the shader program
        __INLINE__ void bind();
	/// @brief	destroys the shader program
        __INLINE__ void unbind();
	/// @brief	returns the shader program id
	__INLINE__ GLint program();

	/// @brief	checks validity of shader program 
	__INLINE__ const bool is_valid();

	/// @brief	sets the shader program to be used for rendering
	__INLINE__ void release();

    /// @brief	Set uniforms of the shader program
	__INLINE__ void Set1i(const std::string& uniform_name, const float& value);
        __INLINE__ void Set1f(const std::string& uniform_name, const float& value);
	__INLINE__ void SetVec2fv(const std::string& uniform_name, const glm::vec2& value);
	__INLINE__ void SetVec3fv(const std::string& uniform_name, const glm::vec3& value);
	__INLINE__ void SetVec4fv(const std::string& uniform_name, const glm::vec4& value);
        __INLINE__ void SetMat3fv(const std::string& uniform_name, const glm::mat3& value);
	__INLINE__ void SetMat4fv(const std::string& uniform_name, const glm::mat4& value);

	/// @brief	deletes and unlinks a GLSL-Shader-Program
	/// @note	equivalent to glUseProgram(m_program);            
        __INLINE__ void delete_shader();

	/// @brief	compiles and links a GLSL-Shader-Pair
	/// @note	to activate the shader created by this use glUseProgram(m_program);
	__INLINE__ bool createShader(const std::string& vertexShader, const std::string& fragmentShader);

	/// @brief	Gets the give uniform location
	/// @param	type	std::string
	/// @param	source	a reference to the GLSL source code as std::string
	/// @return	location if succeeded, otherwhise 0

        __INLINE__ GLint GetUniformLocation(const std::string& uniform_name) const;

	/// @brief	Gets the give uniform location
	/// @param	type	std::string
	/// @param	source	a reference to the GLSL source code as std::string
	/// @return	location if succeeded, otherwhise 0

        __INLINE__ GLint GetVertexAttribLocation(const std::string& attrib_name) const;	

       // Move assignment operator
       __INLINE__ Shader& operator=(Shader&& other) noexcept;
      // Copy constructor
       __INLINE__ Shader(const Shader& other);
private:
	/// @brief	reads the file give by its path filePath
	/// @return	a copy of the content of the file
	__INLINE__ std::string readFile(const std::string& filePath);

	/// @brief	compiles a shader of give type with give GLSL source code as string
	/// @param	type	GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
	/// @param	source	a reference to the GLSL source code as std::string
	/// @return	hShader if succeeded, otherwhise 0
	__INLINE__ GLint compileShader(GLint type, const std::string& source);

private:
	GLint m_program;
        GLint vs, fs;
	mutable std::unordered_map<std::string, int> uniformLocations;
	mutable std::unordered_map<std::string, int> vertexAttribLocations;
	std::string m_vertexShader , m_fragmentShader;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @class	ShaderLibrary is Singleton for  Adding, Caching & retrieval of pre-compiled shaders 
//////////  Only Static Functions are avaliable for Simplicity and ease of use ////////////////////

class ShaderLibrary
{
    public :
    std::unordered_map<std::string, std::shared_ptr<Shader>> Avaliable_Shaders;


    static ShaderLibrary* GetLibrary() {
	static ShaderLibrary SingletonShaderLibrary;
	return &SingletonShaderLibrary;
    }   
    
    static const bool HasShader(const std::string& shader_name)
    {
	   if(ShaderLibrary::GetLibrary()->Avaliable_Shaders.find(shader_name) != ShaderLibrary::GetLibrary()->Avaliable_Shaders.end())
	      return true;
	   else 
	      return false;           
    }

    static void AddShader(const std::string& shader_name, const char* VertexShaderSource , const char* FragmentShaderSource)
    {
	try {

	     if(ShaderLibrary::HasShader(shader_name))
	     {
		DEBUG_PRINT("Shader with the name " + shader_name + " already exists in the library");
	     }
	     else
	     {
		ShaderLibrary::GetLibrary()->Avaliable_Shaders[shader_name] = std::make_shared<Shader>(VertexShaderSource, FragmentShaderSource);
		DEBUG_PRINT("Shader with the name ",  shader_name,  " added to the library");
	     }
	}

        catch(const std::exception& e)
        {   
         // Handle the exception (print an error message, log, etc.)
         std::cerr << "Exception First throwed from ShaderLibrary::AddShader() : " << e.what() << "\n";
         // We might want to rethrow the exception here if we want to propagate it further.
		 throw e;
	}
    } 

    static std::shared_ptr<Shader> GetShader(const std::string& shader_name)
    {
	try {
	  std::unordered_map<std::string, std::shared_ptr<Shader>>::iterator it = ShaderLibrary::GetLibrary()->Avaliable_Shaders.find(shader_name);
	  if(it != ShaderLibrary::GetLibrary()->Avaliable_Shaders.end())
	     return (it->second);
	  else
	     throw std::runtime_error("Failed to Retrieve a shader with name " + shader_name + " from the shader library"); 
	} 
        catch(const std::exception& e)
        {   
         // Handle the exception (print an error message, log, etc.)
         std::cerr << "Exception First throwed from ShaderLibrary::GetShader() : " << e.what() << "\n";
         // We might want to rethrow the exception here if we want to propagate it further.
	 throw e;
	}

	return nullptr; 
    }   	
	  
	private:
	ShaderLibrary() {}
       ~ShaderLibrary() {}

};

} // namespace gridpro_gui

#endif // GP_GUI_SHADER_H
