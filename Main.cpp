#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>

#include "PdBase.hpp"
#include "RtAudio.h"

using namespace Urho3D;
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

class ListenerApp : public Application
{
public:

    SharedPtr<ResourceCache> resourceCache_;
    SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;
    uint32_t sampleRate_;
    uint16_t channels_;
    unsigned int bufferFrames_;

    ListenerApp(Context * context) :
        Application(context),
        sampleRate_(44100),
        channels_(2),
        bufferFrames_(128)
    {
        // constructor
    }

    virtual void Setup()
    {
        if(audio.getDeviceCount()==0){
            std::cout << "There are no available sound devices." << std::endl;
            exit(1);
        }
        // don't use urho's audio buffer stream thing
        RtAudio::StreamParameters parameters;
        parameters.deviceId = audio.getDefaultOutputDevice();
        parameters.nChannels = channels_;

        RtAudio::StreamOptions options;
        options.streamName = "LibPD Test";
        options.flags = RTAUDIO_SCHEDULE_REALTIME;
        if ( audio.getCurrentApi() != RtAudio::MACOSX_CORE ) {
            options.flags |= RTAUDIO_MINIMIZE_LATENCY; // CoreAudio doesn't seem to like this
        }
        audio.openStream(&parameters, NULL, RTAUDIO_FLOAT32, sampleRate_, &bufferFrames_, &audioCallback, NULL, &options);
        audio.startStream();
        // ok done

        Node* node = new Node(context_);

        libPd.init(0, channels_, sampleRate_, true);
        libPd.computeAudio(true);
        Patch patch = libPd.openPatch("patch.pd", "./");
        std::cout << patch << std::endl;

        Patch patch2 = libPd.openPatch("patch.pd", "./");
        std::cout << patch2 << std::endl;

        engineParameters_["FullScreen"] = false;

        engineParameters_["WindowWidth"] = 1280;
        engineParameters_["WindowHeight"] = 720;
        engineParameters_["WindowResizable"] = true;
        // engineParameters_["HighDPI"] = true;
    }

    virtual void Start()
    {
        ResourceCache* resourceCache_ = GetSubsystem<ResourceCache>();

        scene_ = new Scene(context_);
        scene_->CreateComponent<Octree>();
        scene_->CreateComponent<DebugRenderer>();

        Node* skyNode = scene_->CreateChild("Sky");
        Skybox* skybox = skyNode->CreateComponent<Skybox>();
        skybox->SetModel(resourceCache_->GetResource<Model>("Models/Box.mdl"));
        skybox->SetMaterial(resourceCache_->GetResource<Material>("Materials/Skybox.xml"));

        Node* sphereNode = scene_->CreateChild("Sphere");
        sphereNode->SetPosition(Vector3(0,2,15));
        sphereNode->SetScale(Vector3(3,3,3));
        StaticModel* sphereModel = sphereNode->CreateComponent<StaticModel>();
        sphereModel->SetModel(resourceCache_->GetResource<Model>("Models/Sphere.mdl"));
        // boxObject->SetMaterial(resourceCache_->GetResource<Material>("Materials/DefaultMaterial.xml"));
        sphereModel->SetCastShadows(true);

        Node* lightNode=scene_->CreateChild();
        lightNode->SetDirection(Vector3::FORWARD);
        lightNode->Yaw(50);     // horizontal
        lightNode->Pitch(10);   // vertical
        Light* light=lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetBrightness(1.6);
        light->SetColor(Color(1.0,.6,0.3,1));
        light->SetCastShadows(true);

        cameraNode_ = scene_->CreateChild("Camera");
        Camera* camera = cameraNode_->CreateComponent<Camera>();
        camera->SetFarClip(2000);

        Renderer* renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>())); // don't call
        renderer->SetViewport(0,viewport);

        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(ListenerApp, HandleKeyDown));
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(ListenerApp, HandleUpdate));
        SubscribeToEvent(E_RENDERUPDATE, URHO3D_HANDLER(ListenerApp, HandleRenderUpdate));
        SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(ListenerApp, HandlePostRenderUpdate));
        SubscribeToEvent(E_JOYSTICKAXISMOVE, URHO3D_HANDLER(ListenerApp, HandleJoystickAxis));
    }

    virtual void Stop()
    {
    }

    void HandleJoystickAxis(StringHash eventType,VariantMap& eventData)
    {
        using namespace JoystickAxisMove;

        float pos = eventData[P_POSITION].GetFloat();
        if (eventData[P_AXIS] == CONTROLLER_AXIS_LEFTX) {
            libPd.sendFloat("1003-x-axis", -pos);
        }
        if (eventData[P_AXIS] == CONTROLLER_AXIS_LEFTY) {
            libPd.sendFloat("1003-y-axis", pos);
        }
    }

    void HandleKeyDown(StringHash eventType,VariantMap& eventData)
    {
        using namespace KeyDown;
        int key=eventData[P_KEY].GetInt();
        if(key==KEY_ESCAPE)
            engine_->Exit();

        if(key==KEY_TAB)    // toggle mouse cursor when pressing tab
        {
            GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
            GetSubsystem<Input>()->SetMouseGrabbed(!GetSubsystem<Input>()->IsMouseGrabbed());
        }
    }

    void HandleUpdate(StringHash eventType,VariantMap& eventData)
    {
        // HandleAudio();
        float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

        // Movement speed as world units per second
        float MOVE_SPEED=10.0f;
        // Mouse sensitivity as degrees per pixel
        const float MOUSE_SENSITIVITY=0.1f;

        Input* input=GetSubsystem<Input>();
        if(input->GetQualifierDown(1))  // 1 is shift, 2 is ctrl, 4 is alt
            MOVE_SPEED*=10;
        if(input->GetKeyDown('W'))
            cameraNode_->Translate(Vector3(0,0, 1)*MOVE_SPEED*timeStep);
        if(input->GetKeyDown('S'))
            cameraNode_->Translate(Vector3(0,0,-1)*MOVE_SPEED*timeStep);
        if(input->GetKeyDown('A'))
            cameraNode_->Translate(Vector3(-1,0,0)*MOVE_SPEED*timeStep);
        if(input->GetKeyDown('D'))
            cameraNode_->Translate(Vector3( 1,0,0)*MOVE_SPEED*timeStep);

        if(!GetSubsystem<Input>()->IsMouseVisible())
        {
            // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
            IntVector2 mouseMove = input->GetMouseMove();
            static float yaw_=0;
            static float pitch_=0;
            yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
            pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
            pitch_ = Clamp(pitch_, -90.0f, 90.0f);
            // Reset rotation and set yaw and pitch again
            cameraNode_->SetDirection(Vector3::FORWARD);
            cameraNode_->Yaw(yaw_);
            cameraNode_->Pitch(pitch_);
        }
    }

    void HandleRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
        // http://urho3d.github.io/documentation/1.32/_rendering.html
    }

    void HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
        // We could draw some debuggy looking thing for the octree.
        // scene_->GetComponent<Octree>()->DrawDebugGeometry(true);
    }
};

// GO!
URHO3D_DEFINE_APPLICATION_MAIN(ListenerApp);
