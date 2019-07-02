#ifndef MY_JNI_HELPER_H
#define MY_JNI_HELPER_H

#include "myLogger.h"
#include <pthread.h>
#include <string>
#include <jni.h>
#include <vector>
#include <android/asset_manager.h>

#ifdef __cplusplus
extern "C" {
#endif

class MyJNIHelper {

private:
    mutable pthread_mutex_t threadMutex;
    std::string apkInternalPath;
    AAssetManager *apkAssetManager;

public:
    MyJNIHelper(JNIEnv *env, jobject obj, jobject assetManager, jstring pathToInternalDir);

    ~MyJNIHelper();

    void Init(JNIEnv *env, jobject obj, jobject assetManager, jstring pathToInternalDir);

    bool ExtractAssetReturnFilename(std::string assetName, std::string &filename,
                                    bool checkIfFileIsAvailable = false);

    bool ReadFileFromAssetsToBuffer(const char *filename, std::vector<uint8_t> *bufferRef);
};

extern MyJNIHelper *gHelperObject;

#ifdef __cplusplus
}
#endif

#endif // #define MY_JNI_HELPER_H
