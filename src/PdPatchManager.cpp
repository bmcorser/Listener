#include "PdBase.hpp"
#include "RtAudio.h"

using namespace pd;

RtAudio audio;
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
   libPd.processFloat(ticks, (float *)inputBuffer, (float*)outputBuffer);
   return 0;
}

class AudioManager
{
    public:
    void init (int sampleRate, unsigned int bufferFrames) {
        std::cout << "Hello, computer" << std::endl;
        if(audio.getDeviceCount()==0){
            std::cout << "There are no available sound devices." << std::endl;
            exit(1);
        }
        // don't use urho's audio buffer stream thing
        RtAudio::StreamParameters parameters;
        parameters.deviceId = audio.getDefaultOutputDevice();
        parameters.nChannels = 2;

        RtAudio::StreamOptions options;
        options.streamName = "LibPD Test";
        options.flags = RTAUDIO_SCHEDULE_REALTIME;
        if ( audio.getCurrentApi() != RtAudio::MACOSX_CORE ) {
            options.flags |= RTAUDIO_MINIMIZE_LATENCY; // CoreAudio doesn't seem to like this
        }
        audio.openStream(&parameters, NULL, RTAUDIO_FLOAT32, sampleRate, &bufferFrames, &audioCallback, NULL, &options);
        audio.startStream();
        // ok done

        libPd.init(0, 2, sampleRate, true);
        libPd.computeAudio(true);
        Patch patch = libPd.openPatch("patch.pd", "./");
        std::cout << patch << std::endl;

        Patch patch2 = libPd.openPatch("patch.pd", "./");
        std::cout << patch2 << std::endl;
    }
};
