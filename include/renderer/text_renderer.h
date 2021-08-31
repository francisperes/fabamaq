#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <string>
#include <map>
#include "shader.h"

namespace renderer {
    class TextRenderer {
    public:
        TextRenderer(float width, float height);
        ~TextRenderer();

        void Render(std::string text, float x, float y, glm::vec3 colour);
    private:
        struct Character {
            GLuint textureID;
            glm::ivec2 size;
            glm::ivec2 bearing;
            GLuint advance;
        };

        std::map<GLchar, Character> m_characters;
        GLuint VAO, VBO;
        Shader m_textShader;
        FT_Library ft;
        FT_Face face;
    };
}
#endif
