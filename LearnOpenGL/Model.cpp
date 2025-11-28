#include "Model.h"




using namespace std;

    Model::Model(string const& path, std::shared_ptr<Cubemap> cubemap) : shared_cubeMap(cubemap)
    {
        std::cout << "Creating Model\n";
        loadModel(path);
    }
    Model::~Model()
    {
        std::cout << "deleting Model \n";
    }

    void Model::DrawHelper(NewShader& shader, const MeshTextureLocations& MeshToTexStruct)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        unsigned int emissiveNR = 1;
        int unit = 1;
        for (unsigned int i = 0; i < MeshToTexStruct.size; i++)
        {
            if (i >= MaxTexturesPerMesh || MeshToTexStruct.Locations[i] >= MeshTextures.size()) {
                std::cerr << "Invalid texture index access in DrawHelper!\n";
                continue;
            }

            std::string number;
            TextureType type = MeshTextures[MeshToTexStruct.Locations[i]].GetType();
            std::string name;
            switch (type)
            {
            case TextureType::Diffuse:
                name = "texture_diffuse";
                number = std::to_string(diffuseNr++);
                break;
            case TextureType::Specular:
                name = "texture_specular";
                number = std::to_string(specularNr++);
                break;
            case TextureType::Normal:
                name = "texture_normal";
                number = std::to_string(normalNr++);
                break;
            case TextureType::Height:
                name = "texture_height";
                number = std::to_string(heightNr++);
                break;
            case TextureType::Emissive:
                name = "texture_emissive";
                number = std::to_string(emissiveNR++);
                break;
            default:
                std::cout << "Unexpected TextureType\n";
                break;
            }

            shader.setUniform1i((name + number).c_str(), unit);
            MeshTextures[MeshToTexStruct.Locations[i]].UseTexture(unit); // first texture unit reserved for cubemap
            unit++;
        }
    }

    void Model::Draw(NewShader& shader)
    {
        if (meshToTexture.size() != meshes.size()) {
            std::cerr << "ERROR: meshToTexture.size() != meshes.size()\n";
            return;
        }

        if (shared_cubeMap)
        {
            shader.setUniform1i("cubemap", 0);
            shared_cubeMap->UseCubemap(0);
        }

        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            const MeshTextureLocations& MeshToTexStruct = meshToTexture[i];
            DrawHelper(shader, MeshToTexStruct);
            meshes[i].Render();
        }
    }

    void Model::loadModel(string const& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        //const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_ValidateDataStructure);


        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

        MeshTextures.reserve(scene->mNumMeshes);
        meshToTexture.reserve(scene->mNumMeshes);
        meshes.reserve(scene->mNumMeshes);

        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene, 0);
    }

    void Model::processNode(aiNode* node, const aiScene* scene, unsigned int depth)
    {
        std::cout << "processNode depth: " << depth << "\n";
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshToTexture.emplace_back();
            processMesh(mesh, scene);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, depth + 1);
        }
    }

    void Model::processMesh(aiMesh * mesh, const aiScene * scene)
    {
        static unsigned int calls = 0;
        std::cout << "processMesh calls: " << calls << "\n";
        calls++;

        vector<Vertex> vertices;
        vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                if (mesh->HasTangentsAndBitangents()) {
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        if (mesh->mMaterialIndex >= scene->mNumMaterials) {
            std::cerr << "Invalid material index in mesh!\n";
            return;
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if (!material) {
            std::cerr << "Material is nullptr!\n";
            return;
        }

        MeshTextureLocations& MeshToTexStruct = meshToTexture.back();

        loadMaterialTextures(material, aiTextureType_DIFFUSE, MeshToTexStruct);
        loadMaterialTextures(material, aiTextureType_SPECULAR, MeshToTexStruct);
        loadMaterialTextures(material, aiTextureType_HEIGHT, MeshToTexStruct);
        loadMaterialTextures(material, aiTextureType_AMBIENT, MeshToTexStruct);
        loadMaterialTextures(material, aiTextureType_EMISSIVE, MeshToTexStruct);

        meshes.emplace_back(std::move(vertices), std::move(indices));
    }

    TextureType Model::aiTexTypeToTexType(aiTextureType type)
    {
        switch (type)
        {
        case aiTextureType_DIFFUSE: return TextureType::Diffuse;
        case aiTextureType_SPECULAR: return TextureType::Specular;
        case aiTextureType_HEIGHT: return TextureType::Normal;
        case aiTextureType_AMBIENT: return TextureType::Height;
        case aiTextureType_EMISSIVE: return TextureType::Emissive;
        default: return TextureType::Diffuse;
        }
    }

    void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, MeshTextureLocations& MeshToTexStruct)
    {
        static unsigned int calls = 0;
        std::cout << "loadMaterialTextures calls: " << calls << "\n";
        calls++;

        if (mat->GetTextureCount(type) == 0 && MeshToTexStruct.size < MaxTexturesPerMesh)
        {
            const std::string defaultDiffuse = "Textures/plain.png";
            for (unsigned int j = 0; j < MeshTextures.size(); j++)
            {
                if (MeshTextures[j].GetFileLocation() == defaultDiffuse)
                {
                    MeshToTexStruct.Locations[MeshToTexStruct.size] = j;
                    MeshToTexStruct.size += 1;
                    return;
                }
            }

            TextureType myType = aiTexTypeToTexType(type);
            MeshTextures.emplace_back("Textures/plain.png", myType);
            MeshTextures.back().LoadTexture();
            MeshToTexStruct.Locations[MeshToTexStruct.size] = MeshTextures.size() - 1;
            MeshToTexStruct.size += 1;
            return;
        }

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            std::string location = str.C_Str();
            location = directory + "/" + location;

            std::transform(location.begin(), location.end(), location.begin(), [](unsigned char c) { return std::toupper(c); });


            std::replace(location.begin(), location.end(), '\\', '/');

            bool skip = false;
            for (unsigned int j = 0; j < MeshTextures.size(); j++)
            {
                if (MeshTextures[j].GetFileLocation() == location)
                {
                    if (MeshToTexStruct.size < MaxTexturesPerMesh)
                    {
                        MeshToTexStruct.Locations[MeshToTexStruct.size] = j;
                        MeshToTexStruct.size += 1;
                    }
                    skip = true;
                    break;
                }
            }
            if (!skip && MeshToTexStruct.size < MaxTexturesPerMesh)
            {
                TextureType texType = aiTexTypeToTexType(type);
                MeshTextures.emplace_back(location, texType);
                MeshTextures.back().LoadTexture();
                MeshToTexStruct.Locations[MeshToTexStruct.size] = MeshTextures.size() - 1;
                MeshToTexStruct.size += 1;
            }
        }
    }
