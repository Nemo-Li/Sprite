#include "myShader.h"
#include "modelAssimp.h"


#include "Importer.hpp"
#include <opencv2/opencv.hpp>
#include "myJNIHelper.h"
#include "spriteUtil.h"

/**
 * Class constructor
 */
ModelAssimp::ModelAssimp() {

    MyLOGD("ModelAssimp::ModelAssimp");
    initsDone = false;

    // create MyGLCamera object and set default position for the object
    myGLCamera = new MyGLCamera();
    float pos[] = {0.0, -8.0f, 0.0, 0.0, 0.0, 0.0};
    std::copy(&pos[0], &pos[5], std::back_inserter(modelDefaultPosition));
    myGLCamera->SetModelPosition(modelDefaultPosition);
    ScaleAction(0.5f);

    m_startTime = GetCurrentTimeMillis();
    modelObject = NULL;
}

ModelAssimp::~ModelAssimp() {

    MyLOGD("ModelAssimp::ModelAssimpssimp");
    if (myGLCamera) {
        delete myGLCamera;
    }
    if (modelObject) {
        delete modelObject;
    }
}

/**
 * Perform inits and load the triangle's vertices/colors to GLES
 */
void ModelAssimp::PerformGLInits() {

    MyLOGD("ModelAssimp::PerformGLInits");

    MyGLInits();

    modelObject = new AssimpLoader();

    // extract the OBJ and companion files from assets
    std::string objFilename, mtlFilename, texFilename;
//    bool isFilesPresent  =
//            gHelperObject->ExtractAssetReturnFilename("amenemhat/amenemhat.obj", objFilename) &&
//            gHelperObject->ExtractAssetReturnFilename("amenemhat/amenemhat.mtl", mtlFilename) &&
//            gHelperObject->ExtractAssetReturnFilename("amenemhat/amenemhat.jpg", texFilename);
//    if( !isFilesPresent ) {
//        MyLOGE("Model %s does not exist!", objFilename.c_str());
//        return;
//    }

//    gHelperObject->ExtractAssetReturnFilename("nanosuit/nanosuit.obj", objFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/nanosuit.mtl", mtlFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/leg_showroom_spec.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/leg_showroom_refl.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/leg_showroom_ddn.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/leg_dif.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/helmet_showroom_spec.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/helmet_showroom_refl.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/helmet_showroom_ddn.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/helmet_diff.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/hand_showroom_spec.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/hand_showroom_refl.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/hand_showroom_ddn.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/hand_dif.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/glass_refl.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/glass_dif.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/glass_ddn.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/front.jpg", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/cell_leg_alpha.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/cell_helmet_alpha.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/cell_hand_alpha.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/cell_ddn.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/cell_body_alpha.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/cell_arm_alpha.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/body_showroom_spec.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/body_showroom_refl.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/body_showroom_ddn.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/body_dif.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/back.jpg", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/arm_showroom_spec.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/arm_showroom_refl.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/arm_showroom_ddn.png", texFilename);
//    gHelperObject->ExtractAssetReturnFilename("nanosuit/arm_dif.png", texFilename);

    gHelperObject->ExtractAssetReturnFilename("Attack.FBX", objFilename);

//    modelObject->Load3DModel(objFilename);
    modelObject->LoadMesh(objFilename);

    CheckGLError("ModelAssimp::PerformGLInits");
    initsDone = true;
}

/**
 * Render to the display
 */
void ModelAssimp::Render() {

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 mvpMat = myGLCamera->GetMVP();
//    modelObject->Render3DModel(&mvpMat);
    std::vector<Matrix4f> Transforms;

    float RunningTime = GetRunningTime();

    modelObject->BoneTransform(RunningTime, Transforms);

    for (uint i = 0; i < Transforms.size(); i++) {
        modelObject->SetBoneTransform(i, Transforms[i]);
    }

    modelObject->Render(&mvpMat);

    CheckGLError("ModelAssimp::Render");

}

/**
 * set the viewport, function is also called when user changes device orientation
 */
void ModelAssimp::SetViewport(int width, int height) {

    screenHeight = height;
    screenWidth = width;
    glViewport(0, 0, width, height);
    CheckGLError("Cube::SetViewport");

    myGLCamera->SetAspectRatio((float) width / height);
}

/**
 * reset model's position in double-tap
 */
void ModelAssimp::DoubleTapAction() {

    myGLCamera->SetModelPosition(modelDefaultPosition);
}

/**
 * rotate the model if user scrolls with one finger
 */
void ModelAssimp::ScrollAction(float distanceX, float distanceY, float positionX, float positionY) {

    myGLCamera->RotateModel(distanceX, distanceY, positionX, positionY);
}

/**
 * pinch-zoom: move the model closer or farther away
 */
void ModelAssimp::ScaleAction(float scaleFactor) {

    myGLCamera->ScaleModel(scaleFactor);
}

/**
 * two-finger drag: displace the model by changing its x-y coordinates
 */
void ModelAssimp::MoveAction(float distanceX, float distanceY) {

    myGLCamera->TranslateModel(distanceX, distanceY);
}

float ModelAssimp::GetRunningTime() {
    float RunningTime = (float) ((double) GetCurrentTimeMillis() - (double) m_startTime) / 1000.0f;
    return RunningTime;
}