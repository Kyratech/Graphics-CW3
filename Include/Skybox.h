#ifndef SKYBOX_H
#define SKYBOX_H

#include "CubeGeometry.h"

class Skybox
{
private:
GLuint textureID;
GLuint VAO, VBO;
int vertexCount;
public:
    Skybox(std::string imagesPath)
    {
        const std::vector<GLdouble> vertices = GetSkyboxGeometry();
        vertexCount = vertices.size();

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        //Set up the vertex buffers
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * vertexCount, &vertices[0], GL_STATIC_DRAW);

        //We only want the vertex positions
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(GLdouble) * 3, (GLvoid*) 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);

        std::vector<std::string> suffixes;
        suffixes.push_back("_RIGHT.png");
        suffixes.push_back("_LEFT.png");
        suffixes.push_back("_TOP.png");
        suffixes.push_back("_BOTTOM.png");
        suffixes.push_back("_BACK.png");
        suffixes.push_back("_FRONT.png");

        int width, height, n;
        //Load in the 6 face images
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        for(GLuint i = 0; i < 6; i++)
        {
            unsigned char* image = stbi_load((imagesPath + suffixes[i]).c_str(), &width, &height, &n, 3);
            if(image != NULL)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(image);
            }
            else
            {
                std::cout << "Failed to load skybox texture at: " << (imagesPath + suffixes[i]) << std::endl;
            }

        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void Draw(Shader shader, glm::mat4 view, glm::mat4 projection)
    {
        glDepthFunc(GL_LEQUAL);

        glm::mat4 projectionView = projection * glm::mat4(glm::mat3(view));
        GLint projectionViewLocation = glGetUniformLocation(shader.getShaderProgram(), "pv");
        glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, glm::value_ptr(projectionView));

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader.getShaderProgram(), "skybox"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


		glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);
    }
};

#endif // SKYBOX_H
