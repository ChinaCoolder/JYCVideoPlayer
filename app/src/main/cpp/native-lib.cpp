#include <jni.h>
#include <string>
#include <unistd.h>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavfilter/avfilter.h>
    #include <libavcodec/jni.h>

    JNIEXPORT jstring JNICALL
    Java_god_jyc_videoplayer_repo_VideoRepo_ffmpegInfo(JNIEnv *env, jobject  /* this */) {

        char info[40000] = {0};
        void* cOpaque = nullptr;
        const AVCodec *c_temp = nullptr;

        while ((c_temp = av_codec_iterate(&cOpaque))) {
            if (c_temp->decode != nullptr) {
                sprintf(info, "%sdecode:", info);
            } else {
                sprintf(info, "%sencode:", info);
            }
            switch (c_temp->type) {
                case AVMEDIA_TYPE_VIDEO:
                    sprintf(info, "%s(video):", info);
                    break;
                case AVMEDIA_TYPE_AUDIO:
                    sprintf(info, "%s(audio):", info);
                    break;
                default:
                    sprintf(info, "%s(other):", info);
                    break;
            }
            sprintf(info, "%s[%s]\n", info, c_temp->name);
        }
        
        return env->NewStringUTF(info);
    }
}