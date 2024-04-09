#include "gp_gui_texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace gridpro_gui 
{
    OpenGLTexture::OpenGLTexture() : textureId(0) {}

    OpenGLTexture::OpenGLTexture(const std::string& imagePath) : textureId(0) {
        loadTexture(imagePath);
    }

    OpenGLTexture::~OpenGLTexture() {
        if (textureId != 0) {
            printf("OpenGLTexture::~OpenGLTexture() : Deleting texture %d\n", textureId);
            Renderer::GL_API()->glDeleteTextures(1, &textureId);
        }
        else
        {
            printf("OpenGLTexture::~OpenGLTexture() : Texture id is zero\n");
        }
    }

    void OpenGLTexture::loadTex(const std::string& imagePath)
    {
        loadTexture(imagePath);
    }

    void OpenGLTexture::bind(const GLuint slot = 0) const {
        m_slot += slot;
        Renderer::GL_API()->glActiveTexture(GL_TEXTURE0 + slot);
        Renderer::GL_API()->glBindTexture(GL_TEXTURE_2D, textureId);
    }

    void OpenGLTexture::unbind() {
        Renderer::GL_API()->glActiveTexture(GL_TEXTURE0);
        Renderer::GL_API()->glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGLTexture::operator const GLuint() const
    {
      return m_slot;
    }

    void OpenGLTexture::loadTexture(const std::string& imagePath) {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true); // Flip image vertically because OpenGL has the origin at the bottom-left
        unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
        std::cout << "channels = " << channels << std::endl;
        
        GLenum format;
        if (channels == 1) {
            format = GL_RED;
        } else if (channels == 3) {
            format = GL_RGB;
        } else if (channels == 4) {
            format = GL_RGBA;
        } else {
            format = GL_RGB;
        }

        if (data != nullptr) {
            // Generate and set formats

            Renderer::GL_API()->glGenTextures(1, &textureId);
            //Renderer::GL_API()->glActiveTexture(GL_TEXTURE0);
            Renderer::GL_API()->glBindTexture(GL_TEXTURE_2D, textureId);
            Renderer::GL_API()->glTexStorage2D(textureId, 1, GL_RGBA, width, height);

            // Set texture parameters (optional)
            Renderer::GL_API()->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            Renderer::GL_API()->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            Renderer::GL_API()->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            Renderer::GL_API()->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // Generate the texture
            Renderer::GL_API()->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);            
            Renderer::GL_API()->glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);
            
            GLenum error = Renderer::GL_API()->glGetError();
            
            if (error != GL_NO_ERROR) {
                std::cerr << "OpenGL error in loadTexture: " << error << std::endl;
                
               switch (error) {
                    case GL_INVALID_ENUM:
                        std::cerr << "GL_INVALID_ENUM" << std::endl;
                        break;
                    case GL_INVALID_VALUE:
                        std::cerr << "GL_INVALID_VALUE" << std::endl;
                        break;
                    case GL_INVALID_OPERATION:
                        std::cerr << "GL_INVALID_OPERATION" << std::endl;
                        break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
                        break;
                    case GL_OUT_OF_MEMORY:
                        std::cerr << "GL_OUT_OF_MEMORY" << std::endl;
                        break;
                    default:
                        std::cerr << "Unknown error" << std::endl;
                        break;
                }
            
            }

        } else {
            throw std::runtime_error("Failed to load texture: " + imagePath);
            std::cerr << "Failed to load texture: " << imagePath << std::endl;
        }
    }
}