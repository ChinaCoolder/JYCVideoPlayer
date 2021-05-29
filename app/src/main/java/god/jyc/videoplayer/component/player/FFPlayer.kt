package god.jyc.videoplayer.component.player

import android.content.res.AssetManager
import android.view.Surface

class FFPlayer {
    init {
        System.loadLibrary("native-lib")
    }

    fun drawFirstFrame(assetManager: AssetManager, name: String, surface: Surface) {
        drawSingleFrame(assetManager, name, surface)
    }

    private external fun drawSingleFrame(assetManager: AssetManager, name: String, surface: Surface)
}