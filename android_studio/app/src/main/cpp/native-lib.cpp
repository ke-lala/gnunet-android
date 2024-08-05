#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "gnunet_arm_service.h"

#define TAG "MY_TAG"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)

extern "C" {
    #include <unistd.h>
    #include <gnunet_util_lib.h>
}

/** Milestone 6-7 code.
static void
run (void *cls,
     char *const *args,
     const char *cfgfile,
     const struct GNUNET_CONFIGURATION_Handle *c)
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        LOGE ("ERROR opening socket");
        perror("ERROR opening socket");
        goto shutdown;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8081;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        goto shutdown;
    }
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        goto shutdown;
    }
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        goto shutdown;
    }
    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        goto shutdown;
    }
    close(newsockfd);
    close(sockfd);

shutdown:
    GNUNET_SCHEDULER_shutdown();
}*/

static jobject android_java_asset_manager = NULL;
static struct GNUNET_ARM_Handle *h;
static struct GNUNET_CONFIGURATION_Handle *cfg;
static struct GNUNET_ARM_Operation *op;


static void
start_callback (void *cls,
                enum GNUNET_ARM_RequestStatus rs,
                enum GNUNET_ARM_Result result)
{
    (void) cls;
    op = NULL;
    if (GNUNET_ARM_REQUEST_SENT_OK != rs)
    {
        LOGE("Failed to start the ARM service.");
        GNUNET_SCHEDULER_shutdown ();
        return;
    }
    if ((GNUNET_ARM_RESULT_STARTING != result) &&
        (GNUNET_ARM_RESULT_IS_STARTED_ALREADY != result))
    {
        LOGE("Failed to start the ARM service.");
        GNUNET_SCHEDULER_shutdown ();
        return;
    }
    LOGD ("ARM service [re]start successful");
}


static void
shutdown_task (void *cls)
{
    (void) cls;
    if (NULL != op)
    {
        GNUNET_ARM_operation_cancel (op);
        op = NULL;
    }
    if (NULL != h)
    {
        GNUNET_ARM_disconnect (h);
        h = NULL;
    }
    GNUNET_CONFIGURATION_destroy (cfg);
    cfg = NULL;
}


/**
 * Function called whenever we connect to or disconnect from ARM.
 * Termiantes the process if we fail to connect to the service on
 * our first attempt.
 *
 * @param cls closure
 * @param connected #GNUNET_YES if connected, #GNUNET_NO if disconnected,
 *                  #GNUNET_SYSERR on error.
 */
static void
conn_status (void *cls,
             enum GNUNET_GenericReturnValue connected)
{
    static int once;

    (void) cls;
    if ( (GNUNET_SYSERR == connected) &&
         (0 == once) )
    {
        LOGE("Fatal error initializing ARM API.");
        GNUNET_SCHEDULER_shutdown ();
        return;
    }
    once = 1;
}


static void
run (void *cls,
     char *const *args,
     const char *cfgfile,
     const struct GNUNET_CONFIGURATION_Handle *c)
{
    cfg =  GNUNET_CONFIGURATION_dup (c);
    AAssetManager *mgr = static_cast<AAssetManager *>(cls);
    AAsset *asset = AAssetManager_open(mgr, "gnunet.conf", AASSET_MODE_BUFFER);
    char buf[AAsset_getLength(asset)];

    AAsset_read(asset, buf, AAsset_getLength(asset));

    if (GNUNET_OK != GNUNET_CONFIGURATION_deserialize (cfg, buf, strlen(buf), NULL))
    {
        LOGE ("Deserialization of configuration failed!");
    }

    AAsset_close(asset);

    if (NULL == (h = GNUNET_ARM_connect (cfg,
                                         &conn_status,
                                         NULL)))
        return;

    op = GNUNET_ARM_request_service_start (h,
                                           "arm",
                                           GNUNET_OS_INHERIT_STD_NONE,
                                           &start_callback,
                                           NULL);

}


extern "C" JNIEXPORT jstring JNICALL
Java_org_gnu_gnunet_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */,
        jobject assets) {


    android_java_asset_manager = (*env).NewGlobalRef(assets);
    AAssetManager *mgr = AAssetManager_fromJava(env, android_java_asset_manager);

    char *const argvx[] = {
            "server",
            "8081",
            NULL
    };


    static const struct GNUNET_GETOPT_CommandLineOption options[] = {
            GNUNET_GETOPT_OPTION_END
    };

    std::string tmp_file = GNUNET_DISK_mktemp ("test");
    LOGD ("Temp file is here: %s", tmp_file.c_str());
    GNUNET_PROGRAM_run (1,
                        argvx,
                        "native-lib",
                        "native-lib",
                        options,
                        &run,
                        mgr);
    std::string hello = "Hello from C++. Temp file is here: " + tmp_file;

    return env->NewStringUTF(hello.c_str());
}