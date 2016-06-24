#ifndef PDPATCHMIXER_H
#define PDPATCHMIXER_H

#pragma once
#include <vector>
#include <unistd.h>


#include "PdBase.hpp"
#include "RtAudio.h"
#include "PlanetComponent.hpp"

using namespace Urho3D;
using namespace pd;

RtAudio rtAudio;
PdBase libPd;



struct PlanetPatch
{
    int                 patchId;
    PlanetComponent*    planet;

};

class PdPatchMixer
{


public:

    PdPatchMixer();
    PODVector<PlanetPatch> patches_;

    void init (int sampleRate, unsigned int bufferFrames);
    void addPlanet(PlanetComponent* planet);
    void update(Vector3 cameraPosition);

private:

    Vector3 bbmax_; // leave here

};

#endif
