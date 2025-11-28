#include "NewMesh.h"
#include <stdexcept>
#include <iostream>

NewMesh::NewMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    this->vertices = vertices;
    this->indices = indices;
    
    std::cout << "Creating NewMesh by value constructor\n";
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

NewMesh::NewMesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices) :
    vertices(std::move(vertices)), indices(std::move(indices))
{
    std::cout << "Creating NewMesh by Rvalues constructor\n";
    setupMesh();
}

NewMesh::NewMesh(NewMesh&& other) noexcept
{
    std::cout << "Created NewMesh by Move Constructor\n";
    this->vertices = std::move(other.vertices);
    this->indices = std::move(other.indices);
    this->VAO = other.VAO;
    this->VBO = other.VBO;
    this->EBO = other.EBO;
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
}

NewMesh& NewMesh::operator=(NewMesh&& other) noexcept
{
    std::cout << "Altered NewMesh by Move assignment\n";
    this->vertices = std::move(other.vertices);
    this->indices = std::move(other.indices);
    this->VAO = other.VAO;
    this->VBO = other.VBO;
    this->EBO = other.EBO;
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;

    return *this;
}

void NewMesh::clearMesh()
{
    if (EBO != 0)
    {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
}

NewMesh::~NewMesh()
{
    std::cout << "deleting Mesh \n";
    clearMesh();
}

// render the mesh
void NewMesh::Render()
{

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    //glActiveTexture(GL_TEXTURE0);
}



void NewMesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
}