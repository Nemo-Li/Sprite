#ifndef ASSIMPLOADER_H
#define ASSIMPLOADER_H

#include <map>
#include <scene.h>
#include <postprocess.h>
#include <Importer.hpp>
#include <vector>

#include "myGLM.h"
#include "myGLFunctions.h"

// info used to render a mesh
struct MeshInfo {
    GLuint  textureIndex;
    int     numberOfFaces;
    GLuint  faceBuffer;
    GLuint  vertexBuffer;
    GLuint  textureCoordBuffer;
};

class AssimpLoader {

public:
    AssimpLoader();
    ~AssimpLoader();

    void Render3DModel(glm::mat4 *MVP);
    bool Load3DModel(std::string modelFilename);
    void Delete3DModel();

private:
    void GenerateGLBuffers();
    bool LoadTexturesToGL(std::string modelFilename);

    std::vector<struct MeshInfo> modelMeshes;       // contains one struct for every mesh in model
    Assimp::Importer *importerPtr;
    const aiScene* scene;                           // assimp's output data structure
    bool isObjectLoaded;

    std::map<std::string, GLuint> textureNameMap;   // (texture filename, texture name in GL)

    GLuint  vertexAttribute, vertexUVAttribute;     // attributes for shader variables
    GLuint  shaderProgramID;
    GLint   mvpLocation, textureSamplerLocation;    // location of MVP in the shader
};

#endif //ASSIMPLOADER_H
