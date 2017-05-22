#ifndef OBJ_MESH_H
#define OBJ_MESH_H

#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyOBJLoader/tiny_obj_loader.h"

class OBJMesh :public Mesh
{
public:
    /* Constructor */
    OBJMesh(const GLchar* objPath, const struct Material myMaterial)
    {
        material = myMaterial;

        /* TinyOBJ setup*/
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials; //Not used

        std::string err;
        bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objPath);

        //Print any errors raised by the OBJ loader
        if (!err.empty())
        {
          std::cerr << err << std::endl;
        }

        //Exit application if could not load OBJ
        if (!success)
        {
          exit(1);
        }

        std::vector<Vertex> OBJVertices;

        //Shapes
        for(size_t shape = 0; shape < shapes.size(); shape++)
        {
            //Polygon faces within a given shape
            size_t index_offset = 0;
            for(size_t face = 0; face < shapes[shape].mesh.num_face_vertices.size(); face++)
            {
                //Imported as triangles, so I -think- this should always be 3...
                //Documentation could be better
                int faceVertCount = shapes[shape].mesh.num_face_vertices[face];

                //Vertices in given face
                for(size_t vert = 0; vert < faceVertCount; vert++)
                {
                    tinyobj::index_t i = shapes[shape].mesh.indices[index_offset + vert];
                    float vx = attrib.vertices[3*i.vertex_index+0];
                    float vy = attrib.vertices[3*i.vertex_index+1];
                    float vz = attrib.vertices[3*i.vertex_index+2];
                    float nx = attrib.normals[3*i.normal_index+0];
                    float ny = attrib.normals[3*i.normal_index+1];
                    float nz = attrib.normals[3*i.normal_index+2];
                    float tx = attrib.texcoords[2*i.texcoord_index+0];
                    float ty = attrib.texcoords[2*i.texcoord_index+1];
                    OBJVertices.push_back({{vx, vy, vz}, {nx, ny, nz}, {tx, ty}});
                }
                index_offset += faceVertCount;
            }
        }

        vertexCount = OBJVertices.size();

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        //Set up the vertex buffers
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(struct Vertex), &OBJVertices[0], GL_STATIC_DRAW);

        //Set the vertex attrib pointers
        //Vertex positions
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(struct Vertex), (const GLvoid*) offsetof (struct Vertex, position));
        glEnableVertexAttribArray(0);
        //Vertex texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*) offsetof (struct Vertex, textureCoords));
        glEnableVertexAttribArray(1);
        //Normal positions
        glVertexAttribPointer(2, 3, GL_DOUBLE, GL_FALSE, sizeof(struct Vertex), (const GLvoid*) offsetof (struct Vertex, normal));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        //Generate the texture
        glGenTextures(1, &diffuse);
        glGenTextures(1, &specularMap);
        int width, height, n;

        //Diffuse
        glBindTexture(GL_TEXTURE_2D, diffuse);
        unsigned char* image = stbi_load(material.diffuse.c_str(), &width, &height, &n, 3);
        if(image != NULL)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
            std::cout << "Loaded texture at: " << material.diffuse << std::endl;
            //std::cout << "Image stats: " << width << ", " << height << ", " << n << std::endl;
            //std::cout << "First four bytes: " << (int)image[0] << ", " << (int)image[1] << ", " << (int)image[2] << ", " << (int)image[3] << std::endl;
        }
        else
        {
            std::cout << "Failed to load texture at: " << material.diffuse << std::endl;
        }
        stbi_image_free(image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Specular
        glBindTexture(GL_TEXTURE_2D, specularMap);
        image = stbi_load(material.specular.c_str(), &width, &height, &n, 3);
        if(image != NULL)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
            std::cout << "Loaded texture at: " << material.specular << std::endl;
        }
        else
        {
            std::cout << "Failed to load texture at: " << material.specular << std::endl;
        }
        stbi_image_free(image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Clean-up
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Draw(Shader shader)
    {
        shader.Use();

        GLint viewPosLocation = glGetUniformLocation(shader.getShaderProgram(), "viewPos");
        glUniform3f(viewPosLocation, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

        GLint matShineLocation    = glGetUniformLocation(shader.getShaderProgram(), "material.shininess");
        glUniform1f(matShineLocation, material.shininess);

        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
		glUniform1i(glGetUniformLocation(shader.getShaderProgram(), "material.diffuse"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glUniform1i(glGetUniformLocation(shader.getShaderProgram(), "material.specular"), 1);

		glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray(0);
    }

private:
    Material material;
    GLuint VAO, VBO, diffuse, specularMap;
    int vertexCount;
    glm::vec3 fragmentColour;
};

#endif // OBJ_MESH_H
