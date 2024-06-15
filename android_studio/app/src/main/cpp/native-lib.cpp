#include <jni.h>
#include <string>
#include <gnunet_util_lib.h>


extern "C" JNIEXPORT jstring JNICALL
Java_org_gnu_gnunet_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    GNUNET_DISK_mktemp ("test");
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}