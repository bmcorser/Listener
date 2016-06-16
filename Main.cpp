#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

#include <Urho3D/Audio/BufferedSoundStream.h>
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

// #include "PDInstance.cpp"
#include "PdBase.hpp"
#include "PdObject.h"

using namespace Urho3D;
using namespace pd;

class ListenerApp : public Application
{
public:

    SharedPtr<ResourceCache> resourceCache_;
    SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;
    SharedPtr<BufferedSoundStream> soundStream_;
    PdBase pd;

    ListenerApp(Context * context) : Application(context) { }

    virtual void Setup()
    {
        Node* node = new Node(context_);
        SoundSource* source = node->CreateComponent<SoundSource>();
        soundStream_ = new BufferedSoundStream();
        soundStream_->SetFormat(44100, true, false); // TODO: Add stereo here
        source->Play(soundStream_);

        pd.init(0, 2, 44100, true);
        pd.computeAudio(true);
        // pd.queued(true);
        Patch patch = pd.openPatch("patch.pd", "./");
        // std::cout << patch << std::endl;

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
        }
        if (eventData[P_AXIS] == CONTROLLER_AXIS_LEFTY) {
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

    void HandleAudio()
    {
        // Try to keep 1/10 seconds of sound in the buffer, to avoid both dropouts and unnecessary latency
        float targetLength = 1.0f / 10.0f;
        float requiredLength = targetLength - soundStream_->GetBufferLength();
        printf("requiredLength %u\n", requiredLength);
        if (requiredLength < 0.0f)
            return;

        unsigned numSamples = (unsigned)(soundStream_->GetFrequency() * requiredLength);
        printf("numSamples %u\n", numSamples);
        if (!numSamples)
            return;

        SharedArrayPtr<short> newData(new short[numSamples]);
        pd.processShort(1, NULL, newData);
        /*
        short *output = (short*)malloc(1024*2*sizeof(short));
        short *output = (short*)malloc(1024*2*sizeof(short));
        for (int i = 0; i < numSamples/2; i++) {
        }
        for (int i = 0; i < numSamples; i++) {
            std::cout << newData[i] << ",";
            // printf("%hu,", newData[i]);
        }
        printf("\n");
        soundStream_->AddData(newData, numSamples * sizeof(short));
        Queue buffer to the stream for playback
        soundStream_->AddData(newData, numSamples * sizeof(signed short));
        */
        std::cout << pd.blockSize() << std::endl;

    }

    void HandleUpdate(StringHash eventType,VariantMap& eventData)
    {
        HandleAudio();
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
