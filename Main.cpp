#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <random>

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

#include "src/PdPatchMixer.cpp"
#include "src/OrbitalCamera.cpp"
#include "src/PlanetComponent.cpp"
#include "src/CustomGeo.h"

using namespace Urho3D;

class ListenerApp : public Application
{
public:

    SharedPtr<ResourceCache> resourceCache_;
    SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;
    PdPatchMixer pdPatchMixer;

    ListenerApp(Context * context) : Application(context)
    {
        OrbitalCamera::RegisterObject(context_);
        PlanetComponent::RegisterObject(context_);
    }

    virtual void Setup()
    {
        pdPatchMixer.init(44100, 128);

        engineParameters_["FullScreen"] = false;

        engineParameters_["WindowWidth"] = 1280;
        engineParameters_["WindowHeight"] = 1024;
        /*
        engineParameters_["WindowWidth"] = 640;
        engineParameters_["WindowHeight"] = 480;
        */
        // engineParameters_["WindowResizable"] = true;
        // engineParameters_["HighDPI"] = true;
    }

    virtual void Start()
    {
        GetSubsystem<Input>()->SetMouseGrabbed(true);
        GetSubsystem<Input>()->SetMouseVisible(false);
        ResourceCache* resourceCache_ = GetSubsystem<ResourceCache>();

        scene_ = new Scene(context_);
        scene_->CreateComponent<Octree>();
        scene_->CreateComponent<DebugRenderer>();

        Node* skyNode = scene_->CreateChild("Sky");
        Skybox* skybox = skyNode->CreateComponent<Skybox>();
        skybox->SetModel(resourceCache_->GetResource<Model>("Models/Box.mdl"));
        skybox->SetMaterial(resourceCache_->GetResource<Material>("Materials/Skybox.xml"));

        {
            Node* lightNode = scene_->CreateChild();
            lightNode->SetDirection(Vector3::FORWARD);
            lightNode->Yaw(50);     // horizontal
            lightNode->Pitch(10);   // vertical
            Light* light=lightNode->CreateComponent<Light>();
            light->SetLightType(LIGHT_DIRECTIONAL);
            light->SetBrightness(0.8);
            light->SetColor(Color(1.0,.6,0.3,1));
        }
        {
            Node* lightNode = scene_->CreateChild();
            lightNode->SetDirection(Vector3::FORWARD);
            lightNode->Yaw(120);     // horizontal
            lightNode->Pitch(170);   // vertical
            Light* light=lightNode->CreateComponent<Light>();
            light->SetLightType(LIGHT_DIRECTIONAL);
            light->SetBrightness(0.8);
            light->SetColor(Color(0.1,.6,0.3,1));
        }

        Node* orbitalCameraNode = scene_->CreateChild("CameraRoot");
        OrbitalCamera* orbitalCamera = orbitalCameraNode->CreateComponent<OrbitalCamera>();
        cameraNode_ = orbitalCamera->cameraNode;
        orbitalCamera->SetTargetPos(Vector3(0, 0, 0));

        Renderer* renderer = GetSubsystem<Renderer>();
        renderer->DrawDebugGeometry(true);
        cameraNode_ = scene_->CreateChild("Camera");
        Camera* debugCamera = cameraNode_->CreateComponent<Camera>();
        debugCamera->SetFarClip(2000);
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, debugCamera));
        /*
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, orbitalCamera->camera));
        */
        renderer->SetViewport(0,viewport);

        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(ListenerApp, HandleKeyDown));
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(ListenerApp, HandleUpdate));
        SubscribeToEvent(E_RENDERUPDATE, URHO3D_HANDLER(ListenerApp, HandleRenderUpdate));
        SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(ListenerApp, HandlePostRenderUpdate));

        std::random_device rd;
        std::mt19937 rng(rd());

        std::uniform_real_distribution<float> scale_dist(7, 15);
        std::uniform_int_distribution<int> uni(0, 4);
        std::uniform_int_distribution<int> colour_dist(0, 16);
        std::uniform_int_distribution<> position_dist(-100, 100);


        // random parts


        for (unsigned i = 0; i < 5; i += 1)
        {
            int polyhedron_id = uni(rng);
            int scale = scale_dist(rng);
            int colour_id = colour_dist(rng);
            PlanetComponent* planetComponent = scene_->CreateComponent<PlanetComponent>();
            Vector3 position = Vector3(
                position_dist(rng),
                position_dist(rng),
                position_dist(rng)
            );
            planetComponent->place(position, colour_id, polyhedron_id, scale);
            pdPatchMixer.addPlanet(planetComponent);
        }

    }

    virtual void Stop()
    {
    }

    void HandleKeyDown(StringHash eventType,VariantMap& eventData)
    {
        using namespace KeyDown;

        int key=eventData[P_KEY].GetInt();

        if(key == KEY_ESCAPE)
            engine_->Exit();

    }

    void HandleUpdate(StringHash eventType,VariantMap& eventData)
    {
        pdPatchMixer.update(cameraNode_->GetWorldPosition());
        DebugHandleUpdate(eventType, eventData);
    }

    void DebugHandleUpdate(StringHash eventType,VariantMap& eventData)
    {
        float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
        float MOVE_SPEED = 40.0f;
        const float MOUSE_SENSITIVITY = 0.1f;
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

        IntVector2 mouseMove=input->GetMouseMove();
        static float yaw_=0;
        static float pitch_=0;
        yaw_+=MOUSE_SENSITIVITY*mouseMove.x_;
        pitch_+=MOUSE_SENSITIVITY*mouseMove.y_;
        pitch_=Clamp(pitch_,-90.0f,90.0f);
        // Reset rotation and set yaw and pitch again
        cameraNode_->SetDirection(Vector3::FORWARD);
        cameraNode_->Yaw(yaw_);
        cameraNode_->Pitch(pitch_);
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
