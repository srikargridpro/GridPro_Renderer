#include "gp_gui_shader.h" 

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// public implementations: ////////////////////////////////////////


namespace gridpro_gui {



__INLINE__ 
Shader::Shader()
{ 
    m_program = 0;
    vs = 0 ; fs = 0;
}

__INLINE__ 
Shader::Shader(const std::string& filePathVertexShader, const std::string& filePathFragmentShader)
{
	// read compile link and load a GLSL shader as a program
    try
    {
	   std::string vertexShader   = readFile(filePathVertexShader);
	   std::string fragmentShader = readFile(filePathFragmentShader); 
       
	   if(!createShader(vertexShader, fragmentShader))
	      throw std::runtime_error("Falied to read the files from the path " + filePathVertexShader + ", " + filePathFragmentShader);
       
	   m_vertexShader = vertexShader;
	   m_fragmentShader = fragmentShader;
	}
    catch(const std::exception& e)
    {   
        // Handle the exception (print an error message, log, etc.)
        DEBUG_PRINT("Exception in GetUniformLocation: ", e.what(), "\n");
        // We might want to rethrow the exception here if we want to propagate it further.
		throw e;
	}
	
	 Renderer::GL_API()->glUseProgram(m_program);
}

__INLINE__
Shader::Shader(const char* VertexShaderSource , const char* FragmentShaderSource)
{
	// read compile link and load a GLSL shader as a program
    try
    {
	   std::string vertexShader   = std::string(VertexShaderSource);
	   std::string fragmentShader = std::string(FragmentShaderSource);
       
	   if(!createShader(vertexShader, fragmentShader))
	      throw std::runtime_error("Failed to Create the Shaders from the strings " + std::string(VertexShaderSource) + ", " + std::string(FragmentShaderSource));
       
	   m_vertexShader = vertexShader;
	   m_fragmentShader = fragmentShader;
	}

    catch(const std::exception& e)
    {   
        // Handle the exception (print an error message, log, etc.)
        DEBUG_PRINT("Exception in Shader(const char* VertexShaderSource , const char* FragmentShaderSource): ", e.what() , "\n");
        // We might want to rethrow the exception here if we want to propagate it further.
		throw e;
	}
	
	 Renderer::GL_API()->glUseProgram(m_program);
}

__INLINE__ 
Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other) {
        // Move the resources
        m_program = other.m_program;
        vs = other.vs;
        fs = other.fs;
        uniformLocations = std::move(other.uniformLocations);
        vertexAttribLocations = std::move(other.vertexAttribLocations);
        m_vertexShader = std::move(other.m_vertexShader);
        m_fragmentShader = std::move(other.m_fragmentShader);

        // Reset the resources in the other object
        other.m_program = 0;
        other.vs = 0;
        other.fs = 0;
        }
    return *this;
}

__INLINE__ 
Shader::Shader(const Shader& other)
{
    m_program = other.m_program;
    vs = other.vs;
    fs = other.fs;
    uniformLocations = (other.uniformLocations);
    vertexAttribLocations = (other.vertexAttribLocations);
    m_vertexShader = (other.m_vertexShader);
    m_fragmentShader = (other.m_fragmentShader);
}

__INLINE__ 
void Shader::bind()
{
	try
	{ 
     if(m_program)
        Renderer::GL_API()->glUseProgram(m_program);
	 else
	   throw std::runtime_error("Unable to bind Shader");
	}
	catch(const std::exception& e)
	{
		throw e;
	}
}

__INLINE__ 
void Shader::unbind()
{  
	try
	{ 
     if(m_program != 0)
        Renderer::GL_API()->glUseProgram(0);
	else
	  throw std::runtime_error("Unable to unbind Shader");
	}
	catch(const std::exception& e)
	{
		throw e;
	}	 
}

__INLINE__
GLint Shader::GetUniformLocation(const std::string& uniform_name) const
{
try {
	 if(uniformLocations.find(uniform_name) == uniformLocations.end())
	    uniformLocations[uniform_name] = Renderer::GL_API()->glGetUniformLocation(this->m_program, uniform_name.c_str());
	    
      if(uniformLocations[uniform_name] == -1)  
	     throw std::runtime_error("Failed to get uniform location for: " + uniform_name);
	  
	  return uniformLocations[uniform_name];	 
    }
catch(const std::exception& e) {
        // Handle the exception (print an error message, log, etc.)
         DEBUG_PRINT("Exception in GetUniformLocation: ", e.what(), "\n");
		 std::cout << "Exception in GetUniformLocation: " << e.what() << "\n";
        // We might want to rethrow the exception here if we want to propagate it further.
		throw e;
    }
	return -1;
}

__INLINE__
GLint Shader::GetVertexAttribLocation(const std::string& attrib_name) const
{
try {
	 if(vertexAttribLocations.find(attrib_name) == vertexAttribLocations.end())
        vertexAttribLocations[attrib_name] =  Renderer::GL_API()->glGetAttribLocation(this->m_program, attrib_name.c_str());
     
      if( vertexAttribLocations[attrib_name] == -1)  
	     throw std::runtime_error("Failed to get attrib location for: " + attrib_name);
	  
	  return  vertexAttribLocations[attrib_name];
    }

catch(const std::exception& e) {
        // Handle the exception (print an error message, log, etc.)
         DEBUG_PRINT( "Exception in GetVertexAttribLocation: ", e.what() , "\n");
        // We might want to rethrow the exception here if we want to propagate it further.
		throw e;
    }
	return -1;
}


__INLINE__ 
void Shader::Set1i(const std::string& uniform_name, const float& value)
{
       Renderer::GL_API()->glUniform1i(GetUniformLocation(uniform_name) , value);
}

__INLINE__ 
void Shader::Set1f(const std::string& uniform_name, const float& value)
{
       Renderer::GL_API()->glUniform1f(GetUniformLocation(uniform_name) , value);
}

__INLINE__ 
void Shader::SetVec2fv(const std::string& uniform_name, const glm::vec2& value)
{
       Renderer::GL_API()->glUniform2fv(GetUniformLocation(uniform_name), 1,  glm::value_ptr(value));
}

__INLINE__ 
void Shader::SetVec3fv(const std::string& uniform_name, const glm::vec3& value)
{
       Renderer::GL_API()->glUniform3fv(GetUniformLocation(uniform_name), 1,  glm::value_ptr(value));
}

__INLINE__ 
void Shader::SetVec4fv(const std::string& uniform_name, const glm::vec4& value)
{	 
      Renderer::GL_API()->glUniform4fv(GetUniformLocation(uniform_name), 1,  glm::value_ptr(value));
}

__INLINE__ 
void Shader::SetMat3fv(const std::string& uniform_name, const glm::mat3& value)
{	
      Renderer::GL_API()->glUniformMatrix3fv(GetUniformLocation(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
}

__INLINE__ 
void Shader::SetMat4fv(const std::string& uniform_name, const glm::mat4& value)
{
      Renderer::GL_API()->glUniformMatrix4fv(GetUniformLocation(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
}

__INLINE__ 
int Shader::program()
{
   return this->m_program;
}

__INLINE__
const bool Shader::is_valid()
{
	return (m_program != 0 && vs != 0 && fs != 0 && m_vertexShader.size() > 0 && m_fragmentShader.size() > 0);
}

__INLINE__ 
void Shader::delete_shader()
{
	try
	{
	 if (m_program != 0)
	 {
 		 Renderer::GL_API()->glDeleteProgram(m_program);
	 }
	 else
	 {
        throw std::runtime_error("Falied to delete the shader program");
	 }
	}
    catch(const std::exception& e)
    {   
        // Handle the exception (print an error message, log, etc.)
         DEBUG_PRINT("Exception in ~Shader(): ", e.what(), "\n");
        // We might want to rethrow the exception here if we want to propagate it further.
		throw e;
	}

	m_vertexShader.clear();
	m_fragmentShader.clear();
}

__INLINE__ 
Shader::~Shader()
{  
   DEBUG_PRINT("Shader ID :",  m_program , " Destructor Called");	
   delete_shader();
}

__INLINE__ Shader::operator GLint() const
{
   return m_program;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// private implementations: ///////////////////////////////////////

__INLINE__ 
std::string Shader::readFile(const std::string& filePath) {
	std::ifstream fs(filePath.c_str(), std::ios::in);

	if (!fs.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::stringstream buffer;
	std::string line;
	while (std::getline(fs, line)) {
		// debug:
		std::cout << line << "\n"; 
		buffer << line << "\n";
	}

	fs.close();
	return buffer.str();
}

__INLINE__ 
GLint Shader::compileShader(GLint type, const std::string& source)
{
	// creates an empty shader obj, ready to accept source-code and be compiled
	GLint hShader =  Renderer::GL_API()->glCreateShader(type);

	// hands the shader source code to the shader object so that it can keep a copy of it
	const char* src = source.c_str();
	Renderer::GL_API()->glShaderSource(hShader, 1, &src, nullptr);

	// compiles whatever source code is contained in the shader object
	Renderer::GL_API()->glCompileShader(hShader);

	// Error Handling: Check whether the shader has been compiled
	GLint result;
	Renderer::GL_API()->glGetShaderiv(hShader, GL_COMPILE_STATUS, &result);	// assigns result with compile operation status
	if (result == GL_FALSE)
	{
		int length;
		 Renderer::GL_API()->glGetShaderiv(hShader, GL_INFO_LOG_LENGTH, &length); // assigns length with length of information log
		char* infoLog = (char*)alloca(length * sizeof(char));	// allocate on stack frame of caller
		 Renderer::GL_API()->glGetShaderInfoLog(hShader, length, &length, infoLog);	// returns the information log for a shader object
		 std::cout << "Failed to compile shader!"
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "\n";
		std::cout << infoLog << std::endl;
		Renderer::GL_API()->glDeleteShader(hShader);
		return 0;
	}

	DEBUG_PRINT((type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), "Shader Compiled Successfully");

	return hShader;
}

__INLINE__ 
bool Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	// compile the two shaders given as string reference
	vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// create a container for the program-object to which you can attach shader objects
	m_program =  Renderer::GL_API()->glCreateProgram();

	// attaches the shader objects to the program object
	 Renderer::GL_API()->glAttachShader(m_program, vs);
	 Renderer::GL_API()->glAttachShader(m_program, fs);

	// links all the shader objects, that are attached to a program object, together
	 Renderer::GL_API()->glLinkProgram(m_program);

	// Error Handling: Check whether program has been linked successfully
	GLint result;
	 Renderer::GL_API()->glGetProgramiv(m_program, GL_LINK_STATUS, &result);	// assigns result with compile operation status
	if (result == GL_FALSE)
	{
		int length;
		 Renderer::GL_API()->glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length); // assigns length with length of information log
		char* infoLog = (char*)alloca(length * sizeof(char));	// allocate on stack frame of caller
		 Renderer::GL_API()->glGetProgramInfoLog(m_program, length, &length, infoLog);	// returns the information log for a shader object
		std::cout << "Failed to link vertex and fragment shader!"
			<< "\n";
		std::cout << infoLog << std::endl;
		 Renderer::GL_API()->glDeleteProgram(m_program);
		return false;
	}
	 Renderer::GL_API()->glValidateProgram(m_program);
	
	 DEBUG_PRINT("Shader Program Linked Successfully");

	// deletes intermediate objects
	 Renderer::GL_API()->glDeleteShader(vs);
	 Renderer::GL_API()->glDeleteShader(fs);

	// activate the program into the state machine of opengl
	// glUseProgram(m_program);

	return true;
}

} // namespace gridpro_gui



