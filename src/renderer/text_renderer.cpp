#include "renderer/text_renderer.h"
#include <glm/gtc/matrix_transform.hpp>

namespace renderer {
    TextRenderer::TextRenderer(float width, float height) : m_textShader{"resources/shaders/text_vs.glsl", "resources/shaders/text_fs.glsl"} {
        if (FT_Init_FreeType(&ft)) {
            std::cout << "ERROR::FREETYPRE: Could not initFreeType Library" << std::endl;
        }
        if (FT_New_Face(ft, "resources/fonts/Arial.ttf", 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }
        m_textShader.Use();
        glm::mat4 mat = glm::ortho(0.0f, width, 0.0f, height);
        m_textShader.SetMat4("projection", mat);

        FT_Set_Pixel_Sizes(face, 0, 48);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Load first 128 characters of ASCII set
        for (GLubyte c = 0; c < 128; c++) {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (GLuint)face->glyph->advance.x
            };
            m_characters.insert(std::pair<GLchar, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        // Destroy FreeType once we're done
        //FT_Done_Face(face);
        //FT_Done_FreeType(ft);

        // Configure VAO/VBO for texture quads
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    TextRenderer::~TextRenderer() {}

    void TextRenderer::Render(std::string text, float x, float y, glm::vec3 colour) {
        float scale = 0.5f;

        m_textShader.Use();
        m_textShader.SetVec3("textColour", colour);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) { // looks like cancer
            Character ch = m_characters[*c];

            float x_pos = x + ch.bearing.x * scale;
            float y_pos = y - (ch.size.y - ch.bearing.y) * scale;
            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            // UpdateVBO for each character
            GLfloat vertices[6][4] = {
                {x_pos,     y_pos + h, 0.0f, 0.0f},
                {x_pos,     y_pos,     0.0f, 1.0f},
                {x_pos + w, y_pos,     1.0f, 1.0f},

                {x_pos,     y_pos + h, 0.0f, 0.0f},
                {x_pos + w, y_pos,     1.0f, 1.0f},
                {x_pos + w, y_pos + h, 1.0f, 0.0f}
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0 ,6);
            // Now advance cursors for next glyph (noth that advance is number of 1/64 pixels)
            x += (ch.advance >> 6) * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
