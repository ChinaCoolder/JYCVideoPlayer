package god.jyc.videoplayer.component.player

import android.content.Context
import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import android.view.Surface
import android.view.SurfaceView
import god.jyc.videoplayer.R
import java.io.BufferedInputStream
import java.io.FileInputStream
import java.lang.ref.WeakReference

class JYCPlayer private constructor(
    val param: PlayerParam
){
    companion object {
        const val NATIVE_LIB_NAME = "jyc-player"

        fun loadVideoFromAssets(path: String, ctx: Context) =
            JYCPlayer(
                PlayerParam(
                    ctx,
                    PlayerParam.Companion.PLAYER_TYPE.VIDEO,
                    PlayerParam.Companion.SOURCE.ASSETS,
                    path,
                    "",
                    -1
                )
            )
    }

    init {
        System.loadLibrary(NATIVE_LIB_NAME)
    }

    private var nativeHandle: Long = 0L
    private var surface: WeakReference<Surface>?  = null

    fun into(surfaceView: SurfaceView) {
        surface = WeakReference(surfaceView.holder.surface)
    }

    fun init() {
        BufferedInputStream(param.ctx.assets.open("test.mp4"))
    }

    fun drawFirstFrame(assetManager: AssetManager, name: String, surface: Surface) {
        drawSingleFrame(assetManager, name, surface)
    }

    private external fun drawSingleFrame(assetManager: AssetManager, name: String, surface: Surface)
}

data class PlayerParam(
    val ctx: Context,
    val playerType: PLAYER_TYPE,
    val comeFrom: SOURCE,
    val assetsPath: String = "",
    val filePath: String = "",
    val rawId: Int = -1,
){
    companion object {
        enum class PLAYER_TYPE {
            VIDEO,
            AUDIO
        }
        enum class SOURCE {
            ASSETS,
            RAW,
            FILE,
            HTTP,
        }
    }
}