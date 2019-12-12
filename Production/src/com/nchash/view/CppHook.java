package com.nchash.view;
import java.nio.file.Paths;

public class CppHook {

    // TODO: Figure out how to automatically add the .so file directory to Java Path
    //  so we can use use loadLibrary() instead of load()
    static {
        // TODO: place a hook to the C++ backend here, instead of test function in libTest.so
        System.load(Paths.get("./src/com/nchash/jni/ClientWrapper.so").toAbsolutePath().normalize().toString());
   }
    public native void printMsg(String msg);
    public native String client_caller(String image_file_path);
}
