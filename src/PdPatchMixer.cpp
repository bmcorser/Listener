#include "PdPatchMixer.h"


using namespace Urho3D;
using namespace pd;

const float ZERO_VOLUME_DISTANCE = 100.0;

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

PdPatchMixer::PdPatchMixer()
{
    // SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(PdPatchMixer, HandlePostRenderUpdate));
}

void PdPatchMixer::init (int sampleRate, unsigned int bufferFrames) {

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
}

void PdPatchMixer::addPlanet(PlanetComponent* planet) {
    Patch patch = libPd.openPatch("patch.pd", "./");
    int patchId = patch.dollarZero();
    libPd.sendFloat(std::to_string(patchId) + "-x-axis", 0);
    libPd.sendFloat(std::to_string(patchId) + "-y-axis", -1);
    PlanetPatch planetPatch;
    planetPatch.planet = planet;
    planetPatch.patchId = patchId;
    patches_.Push(planetPatch);
}

/*
void PdPatchMixer::remove(Patch patch) {
    // how to remove?
}
*/

void PdPatchMixer::update(Vector3 cameraPosition) {
    for(const PlanetPatch& planetPatch: patches_) {
        Vector3 planetPosition = planetPatch.planet->node->GetWorldPosition();
        float distance = std::abs((cameraPosition - planetPosition).Length());
        float volume = 1 - (distance / (ZERO_VOLUME_DISTANCE / 2));
        if (volume < -1) {
            volume = -1;
        }
        libPd.sendFloat(
            std::to_string(planetPatch.patchId) + "-x-axis",
            0
        );
        libPd.sendFloat(
            std::to_string(planetPatch.patchId) + "-y-axis",
            volume
        );
    }
}
