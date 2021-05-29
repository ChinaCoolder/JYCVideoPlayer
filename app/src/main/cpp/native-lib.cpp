#include <jni.h>
#include <string>
#include <unistd.h>


extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavfilter/avfilter.h>
    #include <libavcodec/jni.h>
    #include <android/log.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
    #include <android/native_window.h>
    #include <android/native_window_jni.h>
    #include <android/asset_manager_jni.h>

    JNIEXPORT void JNICALL
    Java_god_jyc_videoplayer_component_player_FFPlayer_drawSingleFrame(
            JNIEnv *env,
            jobject jThis,
            jobject jAssetManager,
            jstring assetName,
            jobject surface
    ) {
        AAssetManager* assetManager = AAssetManager_fromJava(env, jAssetManager);
        const char *szAssetName = env->GetStringUTFChars(assetName, nullptr);
        AAsset* asset = AAssetManager_open(assetManager, szAssetName, AASSET_MODE_RANDOM);
        env->ReleaseStringUTFChars(assetName, szAssetName);
        off_t offset, length;
        int fd = AAsset_openFileDescriptor(asset, &offset, &length);
        AAsset_close(asset);

        AVFormatContext *formatContext = avformat_alloc_context();
        auto openInputResult = avformat_open_input(&formatContext, env->GetStringUTFChars(url, nullptr), nullptr, nullptr);
        if (openInputResult == 0) {
            if (avformat_find_stream_info(formatContext, nullptr) == 0) {
                int streamIndex = -1;
                for(int i=0; i < formatContext->nb_streams; i++) {
                    if(formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                        streamIndex = i;
                        break;
                    }
                }
                if (streamIndex != -1) {
                    AVCodecParameters *codecParameters = formatContext->streams[streamIndex]->codecpar;
                    const AVCodec *avCodec = avcodec_find_decoder(codecParameters->codec_id);
                    if (avCodec == nullptr) {
                        AVCodecContext * avCodecContext = avcodec_alloc_context3(avCodec);
                        if (avcodec_parameters_to_context(avCodecContext, codecParameters) == 0) {
                            if (avcodec_open2(avCodecContext, avCodec, nullptr) == 0) {
                                AVPacket *packet = av_packet_alloc();
                                AVFrame *frame = av_frame_alloc();
                                int readFrameResult = av_read_frame(formatContext, packet);
                                if (readFrameResult == 0) {
                                    if (packet->stream_index == streamIndex) {
                                        avcodec_send_packet(avCodecContext, packet);
                                        avcodec_receive_frame(avCodecContext, frame);
                                        int videoWidth = avCodecContext->width;
                                        int videoHeight = avCodecContext->height;
                                        AVFrame *rgbFrame = av_frame_alloc();
                                        int bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGBA, videoWidth, videoHeight, 1);
                                        auto *frameBuffer = (uint8_t *) av_malloc(bufferSize * sizeof(uint8_t));
                                        av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, frameBuffer, AV_PIX_FMT_RGBA,
                                                             videoWidth, videoHeight, 1);
                                        auto swsContext = sws_getContext(videoWidth, videoHeight, avCodecContext->pix_fmt,
                                                       videoWidth, videoHeight, AV_PIX_FMT_RGBA,
                                                       SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
                                        sws_scale(swsContext, frame->data, frame->linesize, 0, videoHeight, rgbFrame->data, rgbFrame->linesize);

                                        auto nativeWindow = ANativeWindow_fromSurface(env, surface);

                                        ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth,
                                                                         videoHeight, WINDOW_FORMAT_RGBA_8888);

                                        ANativeWindow_Buffer nativeWindowBuffer;

                                        ANativeWindow_lock(nativeWindow, &nativeWindowBuffer, nullptr);
                                        auto *dstBuffer = static_cast<uint8_t *>(nativeWindowBuffer.bits);

                                        int srcLineSize = rgbFrame->linesize[0];
                                        int dstLineSize = nativeWindowBuffer.stride * 4;

                                        for (int i = 0; i < videoHeight; ++i) {
                                            memcpy(dstBuffer + i * dstLineSize, frameBuffer + i * srcLineSize, srcLineSize);
                                        }

                                        ANativeWindow_unlockAndPost(nativeWindow);

                                        if(nativeWindow)
                                            ANativeWindow_release(nativeWindow);


                                        av_frame_free(&rgbFrame);
                                        rgbFrame = nullptr;


                                        if(frameBuffer != nullptr) {
                                            free(frameBuffer);
                                            frameBuffer = nullptr;
                                        }

                                        if(swsContext != nullptr) {
                                            sws_freeContext(swsContext);
                                            swsContext = nullptr;
                                        }
                                    }
                                    av_packet_unref(packet);
                                } else {
                                    __android_log_write(ANDROID_LOG_ERROR, "Native Error", "Read Frame Error");
                                }
                                if(frame != nullptr) {
                                    av_frame_free(&frame);
                                    frame = nullptr;
                                }

                                if(packet != nullptr) {
                                    av_packet_free(&packet);
                                    packet = nullptr;
                                }
                            } else {
                                __android_log_write(ANDROID_LOG_ERROR, "Native Error", "Open Decoder Error");
                            }
                        } else {
                            __android_log_write(ANDROID_LOG_ERROR, "Native Error", "Get Decoder Context Error");
                        }
                        if(avCodecContext != nullptr) {
                            avcodec_close(avCodecContext);
                            avcodec_free_context(&avCodecContext);
                            avCodecContext = nullptr;
                            avCodec = nullptr;
                        }
                    } else {
                        __android_log_write(ANDROID_LOG_ERROR, "Native Error", "Get Decoder Error");
                    }
                } else {
                    __android_log_write(ANDROID_LOG_ERROR, "Native Error", "Get Stream Index Error");
                }
            } else {
                __android_log_write(ANDROID_LOG_ERROR, "Native Error", "Get AVInfo Failed");
            }
        } else {
            __android_log_print(ANDROID_LOG_ERROR, "Native Error", "Open File Failed, error code: %d", openInputResult);
        }
        if(formatContext != nullptr) {
            avformat_close_input(&formatContext);
            avformat_free_context(formatContext);
            formatContext = nullptr;
        }
    }
}