package god.jyc.videoplayer.repo

import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class VideoRepo @Inject constructor() {
    init {
        System.loadLibrary("native-lib")
    }


    val info: String
        get() = ffmpegInfo()

    external fun ffmpegInfo(): String
}