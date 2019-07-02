/**
 * 提供给java层调用的接口类
 */
#include <jni.h>
#include "modelAssimp.h"
//如果定义了__cplusplus宏
#ifdef __cplusplus
//使用C的方式编译内部代码。如果是c++代码，依然用C的处理方式
//https://blog.csdn.net/zzwdkxx/article/details/44244535
//因为C++的编译器会改变方法名，加了这个即使用C++编译器依然保持名字不变
extern "C" {
#endif

//以标示变量或者函数的定义在别的文件中，提示编译器遇到此变量和函数时在其他模块中寻找其定义
extern ModelAssimp *gAssimpObject;

JNIEXPORT void JNICALL
Java_com_nemoli_sprite_MyGLRenderer_DrawFrameNative(JNIEnv *env,
                                                    jobject instance) {

    if (gAssimpObject == NULL) {
        return;
    }
    gAssimpObject->Render();

}

JNIEXPORT void JNICALL
Java_com_nemoli_sprite_MyGLRenderer_SurfaceCreatedNative(JNIEnv *env,
                                                         jobject instance) {

    if (gAssimpObject == NULL) {
        return;
    }
    gAssimpObject->PerformGLInits();

}

JNIEXPORT void JNICALL
Java_com_nemoli_sprite_MyGLRenderer_SurfaceChangedNative(JNIEnv *env,
                                                         jobject instance,
                                                         jint width,
                                                         jint height) {

    if (gAssimpObject == NULL) {
        return;
    }
    gAssimpObject->SetViewport(width, height);

}

#ifdef __cplusplus
}
#endif

