#include "myShader.h"
#include "myJNIHelper.h"
#include <iostream>
#include <fstream>

/**
 * Read the shader code from assets
 */
bool ReadShaderCode(std::string & shaderCode, std::string & shaderFileName) {

    MyLOGI("Reading shader: %s", shaderFileName.c_str());

    // android shaders are stored in assets
    // read them using MyJNIHelper
    bool isFilePresent = gHelperObject->ExtractAssetReturnFilename(shaderFileName,
                                                                                shaderFileName);
    if( !isFilePresent ) {
        return false;
    }

    std::ifstream shaderStream(shaderFileName.c_str(), std::ios::in);
    if (shaderStream.is_open()) {
        std::string Line = "";
        while (getline(shaderStream, Line)) {
            shaderCode += "\n" + Line;
        }
        shaderStream.close();
    } else {
        MyLOGF("Cannot open %s", shaderFileName.c_str());
        return false;
    }

    MyLOGI("Read successfully");
    return true;
}

/**
 * Compile the shader and log any error messages
 */
bool CompileShader(GLuint & shaderID, const GLenum shaderType, char const * shaderCode) {

    // Create the shader
    shaderID = glCreateShader(shaderType);

    // Compile Shader
    MyLOGI("Compiling shader");
//    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);

    // Check Shader
    GLint result = GL_FALSE;
    int infoLogLength;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (result == 0) {
        MyLOGI("Failed to compile shader");
        std::vector<char> shaderErrorMessage(infoLogLength + 1);
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
        MyLOGI("%s", &shaderErrorMessage[0]);
        return false;
    }
    else{
        MyLOGI("Compiled shader successfully");
    }

    return true;
}

/**
 * Link the vertex and fragment shaders together
 */
bool LinkProgram(GLuint programID, GLuint vertexShaderID,
                 GLuint fragmentShaderID) {
    GLint result = GL_FALSE;
    int infoLogLength;

    MyLOGI("Linking program");

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

    // common deletes
    if (vertexShaderID) {
        glDeleteShader(vertexShaderID);
    }
    if (fragmentShaderID) {
        glDeleteShader(fragmentShaderID);
    }

    if (result == 0) {
        MyLOGI("Failed to link program: %d", programID);
        std::vector<char> programErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength, NULL,
                            &programErrorMessage[0]);
        MyLOGI("%s", &programErrorMessage[0]);
        if (programID) {
            glDeleteProgram(programID);
        }
        return false;
    }
    MyLOGI("Linked successfully");

    return true;
}

/**
 * Read the vertex & fragment shaders, compile and link them, return the program ID
 */
GLuint LoadShaders(std::string vertexShaderFilename,
                   std::string fragmentShaderFilename) {

    GLuint vertexShaderID, fragmentShaderID, programID;
    programID = glCreateProgram();

    // read and compile the vertex shader
//    std::string vertexShaderCode;
//    if (!ReadShaderCode(vertexShaderCode, vertexShaderFilename)) {
//        MyLOGE("Error in reading Vertex shader");
//        return 0;
//    }

    const char *vertexShaderCode = "#version 300 es\n"
                                   "\n"
                                   "layout (location = 0) in vec3 Position;\n"
                                   "layout (location = 1) in vec2 TexCoord;\n"
                                   "layout (location = 2) in vec3 Normal;\n"
                                   "layout (location = 3) in ivec4 BoneIDs;\n"
                                   "layout (location = 4) in vec4 Weights;\n"
                                   "\n"
                                   "out     vec2 textureCoords;\n"
                                   "uniform     mat4 mvpMat;\n"
                                   "\n"
                                   "const int MAX_BONES = 100;\n"
                                   "uniform mat4 gBones[MAX_BONES];\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "    mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];\n"
                                   "    BoneTransform     += gBones[BoneIDs[1]] * Weights[1];\n"
                                   "    BoneTransform     += gBones[BoneIDs[2]] * Weights[2];\n"
                                   "    BoneTransform     += gBones[BoneIDs[3]] * Weights[3];\n"
                                   "\n"
                                   "    vec4 PosL    = BoneTransform * vec4(Position, 1.0);\n"
                                   "    gl_Position     = mvpMat * PosL;\n"
                                   "    textureCoords   = TexCoord;\n"
                                   "}";

    if (!CompileShader(vertexShaderID, GL_VERTEX_SHADER, vertexShaderCode)) {
        MyLOGE("Error in compiling Vertex shader");
        return 0;
    }

    // read and compile the fragment shader
//    std::string fragmentShaderCode;
//    if (!ReadShaderCode(fragmentShaderCode, fragmentShaderFilename)) {
//        MyLOGE("Error in reading Fragment shader");
//        return 0;
//    }


    const char *fragmentShaderCode = "#version 300 es\n"
                                     "\n"
                                     "precision mediump float; // required in GLSL ES 1.00\n"
                                     "\n"
                                     "in vec2      textureCoords;\n"
                                     "uniform sampler2D textureSampler;\n"
                                     "out vec4 fragColor;\n"
                                     "\n"
                                     "void main()\n"
                                     "{\n"
                                     "    fragColor.xyz = texture( textureSampler, textureCoords ).xyz;\n"
                                     "}";

    if (!CompileShader(fragmentShaderID, GL_FRAGMENT_SHADER, fragmentShaderCode)) {
        MyLOGE("Error in compiling fragment shader");
        return 0;
    }

    // Link both the shaders together
    if (!LinkProgram(programID, vertexShaderID, fragmentShaderID)) {
        MyLOGE("Error in linking shaders");
        return 0;
    }

    return programID;
}

/*
 * get the attribute location of an input variable in a shader
 */
GLuint GetAttributeLocation(GLuint programID, std::string variableName) {

    GLint loc = glGetAttribLocation(programID, variableName.c_str());
    if (loc == -1) {
        MyLOGF("Error in getting attribute: %s", variableName.c_str());
        return (0);
    } else {
        return ((GLuint) loc);
    }
}

/*
 * get the location of a uniform variable in a shader
 */
GLint GetUniformLocation(GLuint programID, std::string uniformName) {

    GLint loc = glGetUniformLocation(programID, uniformName.c_str());
    if (loc == -1) {
        MyLOGF("error in uniform: %s", uniformName.c_str());
    } else {
        return loc;
    }
}
