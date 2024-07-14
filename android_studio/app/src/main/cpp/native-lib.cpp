#include <jni.h>
#include <string>
#include <android/log.h>

#define TAG "MY_TAG"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)

extern "C" {
    #include <unistd.h>
    #include <gnunet_util_lib.h>
}

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
}


extern "C" JNIEXPORT jstring JNICALL
Java_org_gnu_gnunet_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    char *const argvx[] = {
            "server",
            "8081",
            NULL
    };
    std::string tmp_file = GNUNET_DISK_mktemp ("test");
    LOGD ("Temp file is here: %s", tmp_file.c_str());
    GNUNET_PROGRAM_run (1,
                        argvx,
                        "native-lib",
                        "native-lib",
                        NULL,
                        &run,
                        NULL);
    std::string hello = "Hello from C++. Temp file is here: " + tmp_file;
    return env->NewStringUTF(hello.c_str());
}