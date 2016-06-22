#include "PdBase.hpp"
#include "RtAudio.h"

using namespace Urho3D;
using namespace pd;

RtAudio rtAudio;
PdBase libPd;

int audioCallback(
    void *outputBuffer,
    void *inputBuffer,
    unsigned int nBufferFrames,
    double streamTime,
    RtAudioStreamStatus status,
    void *userData
){
   int ticks = nBufferFrames / 64;
   // libPd.processFloat(ticks, (float *)inputBuffer, (float*)outputBuffer);
   return 0;
}

class PdPatchManager
{

    public:
    void init (int sampleRate, unsigned int bufferFrames) {
        if(rtAudio.getDeviceCount() == 0){
            std::cout << "There are no available sound devices." << std::endl;
            exit(1);
        }
        RtAudio::StreamParameters parameters;
        parameters.deviceId = rtAudio.getDefaultOutputDevice();
        parameters.nChannels = 2;

        RtAudio::StreamOptions options;
        options.streamName = "planets";
        options.flags = RTAUDIO_SCHEDULE_REALTIME;
        if (rtAudio.getCurrentApi() != RtAudio::MACOSX_CORE) {
            options.flags |= RTAUDIO_MINIMIZE_LATENCY; // CoreAudio doesn't seem to like this
        }
        rtAudio.openStream(
            &parameters,
            NULL,
            RTAUDIO_FLOAT32,
            sampleRate,
            &bufferFrames,
            &audioCallback,
            NULL,
            &options
        );
        rtAudio.startStream();

        libPd.init(0, 2, sampleRate, true);
        libPd.computeAudio(true);
        Patch patch = libPd.openPatch("patch.pd", "./");
    }
    void updateX(float pos) {
        libPd.sendFloat("x-axis", -pos);
    }
    void updateY(float pos) {
        libPd.sendFloat("y-axis", pos);
    }
};
