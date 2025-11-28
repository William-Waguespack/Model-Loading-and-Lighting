#pragma once

#include <GL\glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4



struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

class NewMesh
{
public:
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    // constructor
    NewMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    NewMesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices);

    NewMesh(const NewMesh&) = delete;
    NewMesh& operator=(const NewMesh&) = delete;

    NewMesh(NewMesh&& other) noexcept;
    NewMesh& operator=(NewMesh&& other) noexcept;

    void clearMesh();

    ~NewMesh();

    // render the mesh
    void Render();

private:
    // render data 
    unsigned int VAO, VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();
};
