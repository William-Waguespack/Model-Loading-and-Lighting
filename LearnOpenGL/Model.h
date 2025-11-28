#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "NewMesh.h"
#include "NewShader.h"
#include "Texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <map>
#include <vector>
#include <array>
#include <memory>
#include "Cubemap.h"

#define MaxTexturesPerMesh 8

using namespace std;

struct MeshTextureLocations
{
    size_t size = 0;
    std::array<unsigned int, MaxTexturesPerMesh> Locations;

    MeshTextureLocations()
    {
        Locations.fill(0);
    }
};

class Model
{
public:
    vector<Texture> MeshTextures;
    vector<MeshTextureLocations> meshToTexture;
    vector<NewMesh> meshes;
    string directory;
    std::shared_ptr<Cubemap> shared_cubeMap;
    

    Model(string const& path, std::shared_ptr<Cubemap> cubemap = nullptr);
    ~Model();

    void DrawHelper(NewShader& shader, const MeshTextureLocations& MeshToTexStruct);

    void Draw(NewShader& shader);

private:
    void loadModel(string const& path);

    void processNode(aiNode* node, const aiScene* scene, unsigned int depth);

    void processMesh(aiMesh* mesh, const aiScene* scene);

    TextureType aiTexTypeToTexType(aiTextureType type);

    void loadMaterialTextures(aiMaterial* mat, aiTextureType type, MeshTextureLocations& MeshToTexStruct);
};
