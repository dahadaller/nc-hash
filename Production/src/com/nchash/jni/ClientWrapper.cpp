#include "com_nchash_view_CppHook.h"
#include <jni.h>
#include <stdio.h>
#include "libClient.h"
// using namespace std;

JNIEXPORT void JNICALL Java_com_nchash_view_CppHook_printMsg
(JNIEnv *env, jobject obj, jstring image_file_path) {
    const char *utf_image_file_path;
    jboolean *iscopy = NULL;

    // Get the UTF string 
    utf_image_file_path = env->GetStringUTFChars(image_file_path, iscopy);
    if (utf_image_file_path == NULL) {
        printf("Could not convert Java string to UTF-8 string.\n");
        return;
    }

    // Print the message on the standard output 
    printf("%s\n", utf_image_file_path);

    // Release the memory 
    env->ReleaseStringUTFChars(image_file_path, utf_image_file_path);
}

JNIEXPORT jstring JNICALL Java_com_nchash_view_CppHook_client_1caller
  (JNIEnv *env, jobject obj, jstring image_file_path){
    const char *utf_image_file_path;
    const bool hash_received_zkp_passed;
    jboolean *iscopy = NULL;

    // Get the UTF string
    utf_image_file_path = env->GetStringUTFChars(image_file_path, iscopy);
    if (utf_image_file_path == NULL) {
        printf("Could not Java string filepath to UTF-8 string filepath.\n");
        return 0;
    }
    hash_received_zkp_passed = client(utf_image_file_path);
    jstring javaString = env->NewStringUTF(hash_received_zkp_passed);

    //TODO: Memory Leak
    // Release the memory
    //env->ReleaseStringUTFChars(image_file_path, utf_image_file_path);

    return javaString;
  }
