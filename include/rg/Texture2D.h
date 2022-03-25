//
// Created by matf-rg on 30.10.20..
//

#ifndef PROJECT_BASE_TEXTURE2D_H
#define PROJECT_BASE_TEXTURE2D_H
#include <glad/glad.h>
#include <stb_image.h>
#include <rg/Error.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <stb_image.h>

class Texture2D {
public:

    Texture2D(const char *pathToTexture, GLint textureNum)
    {
        m_textureNumber = textureNum;

        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(pathToTexture, &width, &height, &nrChannels, 0);
        if (data) {

            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "Failed to load texture: " << pathToTexture << std::endl;
        }
        stbi_image_free(data);

    }

    ~Texture2D()
    {
        glDeleteTextures(1, &m_textureId);
    }

    inline GLuint getTextureID() const { return m_textureId; }

    inline GLint getTextureNumber() const { return m_textureNumber; }

    void bind() const
    {
        glActiveTexture(GL_TEXTURE0 + m_textureNumber);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
    }

private:
    GLuint m_textureId;
    GLint  m_textureNumber;

};

#endif //PROJECT_BASE_TEXTURE2D_H
