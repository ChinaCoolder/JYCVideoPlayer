//
// Created by jiayi on 2021/5/31.
//

#ifndef JYCVIDEOPLAYER_BASEDECODER_H
#define JYCVIDEOPLAYER_BASEDECODER_H

class BaseDecoder {
public:
    virtual void Init() = 0;
    virtual void Release() = 0;

    virtual void Start() = 0;
    virtual void Stop() = 0;

    virtual void Pause() = 0;

    virtual void SeekTargetPosition(float position) = 0;
    virtual float GetCurrentPosition() = 0;
};

#endif //JYCVIDEOPLAYER_BASEDECODER_H
