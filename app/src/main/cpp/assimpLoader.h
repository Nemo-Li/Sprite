#ifndef ASSIMPLOADER_H
#define ASSIMPLOADER_H

#include "ogldev_math_3d.h"
#include <map>
#include <scene.h>
#include <postprocess.h>
#include <Importer.hpp>
#include <vector>

#include "myGLM.h"
#include "myGLFunctions.h"

// info used to render a mesh
struct MeshInfo {
    GLuint textureIndex;
    int numberOfFaces;
    GLuint faceBuffer;
    GLuint vertexBuffer;
    GLuint textureCoordBuffer;
};

class AssimpLoader {

public:
    AssimpLoader();

    ~AssimpLoader();

    void Render3DModel(glm::mat4 *MVP);

    bool Load3DModel(std::string modelFilename);

    bool LoadMesh(const std::string &Filename);

    void Render();

    void BoneTransform(float TimeInSeconds, std::vector<Matrix4f> &Transforms);

    void Delete3DModel();

private:
#define NUM_BONES_PER_VEREX 4

    struct BoneInfo {
        Matrix4f BoneOffset;
        Matrix4f FinalTransformation;

        BoneInfo() {
            BoneOffset.SetZero();
            FinalTransformation.SetZero();
        }
    };

    struct VertexBoneData {
        uint IDs[NUM_BONES_PER_VEREX];
        float Weights[NUM_BONES_PER_VEREX];

        VertexBoneData() {
            Reset();
        };

        void Reset() {
            ZERO_MEM(IDs);
            ZERO_MEM(Weights);
        }

        void AddBoneData(uint BoneID, float Weight);
    };

    void CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);

    void CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);

    void CalcInterpolatedPosition(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);

    uint FindScaling(float AnimationTime, const aiNodeAnim *pNodeAnim);

    uint FindRotation(float AnimationTime, const aiNodeAnim *pNodeAnim);

    uint FindPosition(float AnimationTime, const aiNodeAnim *pNodeAnim);

    const aiNodeAnim *FindNodeAnim(const aiAnimation *pAnimation, const std::string NodeName);

    void ReadNodeHierarchy(float AnimationTime, const aiNode *pNode, const Matrix4f &ParentTransform);

    void GenerateGLBuffers();

    bool LoadTexturesToGL(std::string modelFilename);

    std::vector<struct MeshInfo> modelMeshes;       // contains one struct for every mesh in model
    Assimp::Importer *importerPtr;
    const aiScene *scene;                           // assimp's output data structure
    bool isObjectLoaded;

#define INVALID_MATERIAL 0xFFFFFFFF

    enum VB_TYPES {
        INDEX_BUFFER,
        POS_VB,
        NORMAL_VB,
        TEXCOORD_VB,
        BONE_VB,
        NUM_VBs
    };

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

    struct MeshEntry {
        MeshEntry() {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
            MaterialIndex = INVALID_MATERIAL;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;

    std::map<std::string, uint> m_BoneMapping; // maps a bone name to its index
    uint m_NumBones;
    std::vector<BoneInfo> m_BoneInfo;
    Matrix4f m_GlobalInverseTransform;

    std::map<std::string, GLuint> textureNameMap;   // (texture filename, texture name in GL)

    GLuint vertexAttribute, vertexUVAttribute;     // attributes for shader variables
    GLuint shaderProgramID;
    GLint mvpLocation, textureSamplerLocation;    // location of MVP in the shader

    bool InitFromScene(const aiScene *pScene, const std::string &Filename);

    void InitMesh(uint MeshIndex,
                  const aiMesh *paiMesh,
                  std::vector<Vector3f> &Positions,
                  std::vector<Vector3f> &Normals,
                  std::vector<Vector2f> &TexCoords,
                  std::vector<VertexBoneData> &Bones,
                  std::vector<unsigned int> &Indices);

    void LoadBones(uint MeshIndex, const aiMesh *paiMesh, std::vector<VertexBoneData> &Bones);
};

#endif //ASSIMPLOADER_H
