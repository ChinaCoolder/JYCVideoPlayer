package god.jyc.videoplayer.view

import androidx.hilt.lifecycle.ViewModelInject
import androidx.lifecycle.ViewModel
import god.jyc.videoplayer.repo.VideoRepo

class MainViewModel @ViewModelInject constructor(
    private val videoRepo: VideoRepo
): ViewModel() {

    fun getInfo(): String = videoRepo.info
}