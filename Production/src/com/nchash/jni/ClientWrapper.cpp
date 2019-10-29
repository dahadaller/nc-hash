#include "com_nchash_view_CppHook.h"
#include <jni.h>
#include <stdio.h>

// using namespace std;

JNIEXPORT void JNICALL Java_com_nchash_view_CppHook_printMsg
(JNIEnv *env, jobject obj, jstring msg) {
    const char *utfMsg;
    jboolean *iscopy = NULL;

    // Get the UTF string 
    utfMsg = env->GetStringUTFChars(msg, iscopy);
    if (utfMsg == NULL) {
        printf("Could not convert Java string to UTF-8 string.\n");
        return;
    }

    // Print the message on the standard output 
    printf("%s\n", utfMsg);

    // Release the memory 
    env->ReleaseStringUTFChars(msg, utfMsg);
}

JNIEXPORT jstring JNICALL Java_com_nchash_view_CppHook_client_1caller
  (JNIEnv *env, jobject obj, jstring msg){
    const char *utfMsg;
    const char *hash;
    jboolean *iscopy = NULL;

    // Get the UTF string
    utfMsg = env->GetStringUTFChars(msg, iscopy);
    if (utfMsg == NULL) {
        printf("Could not convert Java string to UTF-8 string.\n");
        return 0;
    }
    //hash = client(utfMsg);
    // let's pretend this text is being processed here. rather than just reassigned.

    jstring javaString = env->NewStringUTF(utfMsg);

    // Release the memory
    //env->ReleaseStringUTFChars(msg, utfMsg);

    return javaString;
  }
