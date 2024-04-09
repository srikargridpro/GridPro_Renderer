
#ifndef _GP_GUI_API_RENDERER_OPENGL_FUNCTIONS_
#define _GP_GUI_API_RENDERER_OPENGL_FUNCTIONS_

#include "gp_gui_debug.h"
#include <QOpenGLFunctions_2_1>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions_4_3_Compatibility>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QOpenGLWidget>
#include "gp_gui_typedefs.h"


namespace gridpro_gui {

namespace OGL_4_3 
{ static QOpenGLWidget* Renderer_Context = nullptr; }

class Renderer {
public :

uint64_t gl_api_call_count; 

static Renderer& GetInstance() {
    static Renderer instance;
    return instance;
}

/* Default */
static QOpenGLFunctions_4_3_Compatibility* GL_API()
{
    // if(OGL_4_3::Renderer_Context == nullptr) {
    //     throw std::runtime_error("Renderer Context is not set");
    // }
    // return OGL_4_3::Renderer_Context;

    Renderer& OpenGL_Functions_Singleton_Access = GetInstance();
    // ++OpenGL_Functions_Singleton_Access.gl_api_call_count;
    return &OpenGL_Functions_Singleton_Access.OpenGL_4_3_functions;
}

/* Legacy */
class Legacy {
public :
static QOpenGLFunctions_2_1* GL_API()
{
   Renderer& OpenGL_Functions_Singleton_Access = GetInstance();
     ++OpenGL_Functions_Singleton_Access.gl_api_call_count;
    return &OpenGL_Functions_Singleton_Access.OpenGL_2_1_functions;
}
};
/* Compatibility Profile */
class COMPAT_3_3 {
public :
static QOpenGLFunctions_3_3_Compatibility* GL_API()
{
    Renderer& OpenGL_Functions_Singleton_Access = GetInstance();
     ++OpenGL_Functions_Singleton_Access.gl_api_call_count;
    return &OpenGL_Functions_Singleton_Access.OpenGL_3_3_functions;
}
};

class COMPAT_4_3 {
public :	
static QOpenGLFunctions_4_3_Compatibility* GL_API()
{
    Renderer& OpenGL_Functions_Singleton_Access = GetInstance();
    ++OpenGL_Functions_Singleton_Access.gl_api_call_count;
    return &OpenGL_Functions_Singleton_Access.OpenGL_4_3_functions;
}
};

/* Core Profile */
class CORE_3_3
{   
public :	
static QOpenGLFunctions_3_3_Core* GL_API()
{   
    Renderer& OpenGL_Functions_Singleton_Access = GetInstance();
     ++OpenGL_Functions_Singleton_Access.gl_api_call_count;
    return &OpenGL_Functions_Singleton_Access.OpenGL_3_3_Corefunctions;
}

}; // CORE_3_3

class CORE_4_3
{
public :	
static QOpenGLFunctions_4_3_Core* GL_API()
{
    Renderer& OpenGL_Functions_Singleton_Access = GetInstance();
     ++OpenGL_Functions_Singleton_Access.gl_api_call_count;
    return &OpenGL_Functions_Singleton_Access.OpenGL_4_3_Corefunctions;
}

}; // CORE_4_3

QOpenGLFunctions_2_1 OpenGL_2_1_functions;

QOpenGLFunctions_3_3_Compatibility OpenGL_3_3_functions; 
QOpenGLFunctions_3_3_Core OpenGL_3_3_Corefunctions; 

QOpenGLFunctions_4_3_Compatibility OpenGL_4_3_functions; 
QOpenGLFunctions_4_3_Core OpenGL_4_3_Corefunctions; 

//-----------------------------
QOpenGLContext* context;
QOpenGLFunctions* glFunctions;
QOpenGLWidget* gl_widget;

uint64_t API_CALL_COUNT() { return this->gl_api_call_count; }

void initialise()
{
    OpenGL_2_1_functions.initializeOpenGLFunctions();

    OpenGL_3_3_functions.initializeOpenGLFunctions(); 
    OpenGL_3_3_Corefunctions.initializeOpenGLFunctions();

    OpenGL_4_3_Corefunctions.initializeOpenGLFunctions();
    OpenGL_4_3_functions.initializeOpenGLFunctions();
} 

private :
Renderer() { 
    gl_api_call_count = 0;
    //initialise();
    context = QOpenGLContext::currentContext();
    if (context) {
        glFunctions = context->functions();
        // Use glFunctions for OpenGL operations
        // For example:
        // glFunctions->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Print current context
        std::cout << "Current OpenGL context: " << context->format().version().first << "." << context->format().version().second << std::endl;
    }
}

~Renderer() {
printf("Releasing OpenGL Function Objects");
}

// Disable the copy constructor and assignment operator.
Renderer(const Renderer&) = delete;
Renderer& operator=(const Renderer&) = delete;      

}; // Renderer class

} // namespace HELIUM_API

#endif
