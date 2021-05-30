package god.jyc.videoplayer.view

import android.opengl.GLSurfaceView
import android.os.Bundle
import android.os.Environment
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.viewModels
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.MaterialTheme
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import dagger.hilt.android.AndroidEntryPoint
import god.jyc.videoplayer.component.player.JYCPlayer

@AndroidEntryPoint
class MainActivity : ComponentActivity() {

    init {
        System.loadLibrary("native-lib")
    }

    companion object {
        val PATH: String = Environment.getExternalStorageDirectory().getAbsolutePath() + "/test.mp4"
    }

    private val viewModel by viewModels<MainViewModel>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            NewsStory(PATH)
        }
    }

    @Composable
    fun NewsStory(path: String) {
        MaterialTheme {
            val typography = MaterialTheme.typography
            Column(
                modifier = Modifier.padding(16.dp)
            ) {
                AndroidView(
                    {
                        GLSurfaceView(it).apply {
                            JYCPlayer().drawFirstFrame(context.assets, "test.mp4", this.holder.surface)
                        }
                    },
                    Modifier
                        .height(180.dp)
                        .fillMaxWidth()
                        .clip(shape = RoundedCornerShape(4.dp))
                )
                Spacer(Modifier.height(16.dp))
                Text(
                    "A day wandering through the sandhills " +
                            "in Shark Fin Cove, and a few of the " +
                            "sights I saw",
                    style = typography.h6,
                    maxLines = 2,
                    overflow = TextOverflow.Ellipsis)
                Text("Davenport, California",
                    style = typography.body2)
                Text("Desc",
                    style = typography.body2)
            }
        }
    }

    @Preview
    @Composable
    fun DefaultPreview() {
        NewsStory("text")
    }
}