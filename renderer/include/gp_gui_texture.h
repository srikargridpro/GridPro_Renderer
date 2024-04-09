#ifndef GP_GUI_TEXTURE_H
#define GP_GUI_TEXTURE_H

#include <string>
#include "gp_gui_renderer_api.h"
#include "stb_image.h"

namespace gridpro_gui
{
class OpenGLTexture {
public:
    OpenGLTexture();
    OpenGLTexture(const std::string& imagePath);
   ~OpenGLTexture();

    void loadTex(const std::string& imagePath);
    void bind(const GLuint slot) const ;
    void unbind();
    operator const GLuint() const;

private:
    void loadTexture(const std::string& imagePath);

    GLuint textureId;
    mutable GLuint m_slot;
};
}
#endif // GP_GUI_TEXTURE_H
