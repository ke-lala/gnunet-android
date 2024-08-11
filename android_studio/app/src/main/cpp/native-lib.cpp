#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "gnunet_util_lib.h"

#define TAG "MY_TAG"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)

extern "C" {
    #include <unistd.h>
    #include <gnunet_util_lib.h>
}

static int pfd[2];
static pthread_t thr;
static const char *tag = "GNUNET";

void *thread_func(void*)
{
    ssize_t rdsz;
    char buf[128];
    while((rdsz = read(pfd[0], buf, sizeof buf - 1)) > 0) {
        if(buf[rdsz - 1] == '\n') --rdsz;
        buf[rdsz] = 0;  /* add null-terminator */
        __android_log_write(ANDROID_LOG_DEBUG, tag, buf);
    }
    return 0;
}

int start_logger(const char *app_name)
{
    tag = app_name;

    /* make stdout line-buffered and stderr unbuffered */
    setvbuf(stdout, 0, _IOLBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);

    /* create the pipe and redirect stdout and stderr */
    pipe(pfd);
    dup2(pfd[1], 1);
    dup2(pfd[1], 2);

    /* spawn the logging thread */
    if(pthread_create(&thr, 0, thread_func, 0) == -1)
        return -1;
    pthread_detach(thr);
    return 0;
}

/** ------------------ Milestone 6-7 code. -------------------------------
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

// END ------------------------------ Milestone 6 - 7 Code ---------------------

static jobject android_java_asset_manager = NULL;
static struct GNUNET_ARM_Handle *h;
static struct GNUNET_CONFIGURATION_Handle *cfg;
static struct GNUNET_ARM_Operation *op;

/* -------------- Multi library code -----------------------------
/*static void
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
}*/


/**
 * Function called whenever we connect to or disconnect from ARM.
 * Termiantes the process if we fail to connect to the service on
 * our first attempt.
 *
 * @param cls closure
 * @param connected #GNUNET_YES if connected, #GNUNET_NO if disconnected,
 *                  #GNUNET_SYSERR on error.
 */
/*static void
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
    start_logger(tag);
    printf("Some message.\n");
    GNUNET_log (GNUNET_ERROR_TYPE_ERROR,
                "Simulate error message. We do not no what is the actual log level.\n");
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
 END -------------- Multi library code ----------------------------- */

extern "C" JNIEXPORT jstring JNICALL
Java_org_gnu_gnunet_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */,
        jobject assets) {


    android_java_asset_manager = (*env).NewGlobalRef(assets);
    AAssetManager *mgr = AAssetManager_fromJava(env, android_java_asset_manager);

    char *const argvx[] = {
            "-L",
            "DEBUG",
            "-l",
            "test.out",
            NULL
    };


    static const struct GNUNET_GETOPT_CommandLineOption options[] = {
            GNUNET_GETOPT_OPTION_END
    };

    std::string tmp_file = GNUNET_DISK_mktemp ("test");
    LOGD ("Temp file is here: %s", tmp_file.c_str());

    start_logger(tag);
    printf("Some message.\n");
    GNUNET_log (GNUNET_ERROR_TYPE_ERROR,
                "Simulate error message. We do not no what is the actual log level.\n");
    cfg = GNUNET_CONFIGURATION_create ();
    AAsset *asset = AAssetManager_open(mgr, "gnunet.conf", AASSET_MODE_BUFFER);
    char buf[AAsset_getLength(asset)];

    AAsset_read(asset, buf, AAsset_getLength(asset));

    if (GNUNET_OK != GNUNET_CONFIGURATION_deserialize (cfg, buf, strlen(buf), NULL))
    {
        LOGE ("Deserialization of configuration failed!");
    }

    AAsset_close(asset);

    GNUNET_SERVICE_main (4,
                        argvx,
                        cfg);

    /*GNUNET_PROGRAM_run (4,
                        argvx,
                        "native-lib",
                        "native-lib",
                        options,
                        &run,
                        mgr);*/
    std::string hello = "Hello from C++. Temp file is here: " + tmp_file;

    return env->NewStringUTF(hello.c_str());
}