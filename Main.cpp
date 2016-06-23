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

#include "src/PdPatchManager.cpp"
#include "src/OrbitalCamera.cpp"
#include "src/PlanetComponent.cpp"

using namespace Urho3D;

class ListenerApp : public Application
{
public:

    SharedPtr<ResourceCache> resourceCache_;
    SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;
    SharedPtr<PlanetComponent> planetComponent;
    PdPatchManager pdPatchManager_;

    ListenerApp(Context * context) : Application(context)
    {
        OrbitalCamera::RegisterObject(context_);
        PlanetComponent::RegisterObject(context_);
    }

    virtual void Setup()
    {
        pdPatchManager_.init(44100, 128);

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

        /* PlanetComponent* */ planetComponent = scene_->CreateComponent<PlanetComponent>();
        planetComponent->place(Vector3(0, 2, 15));

        Node* lightNode=scene_->CreateChild();
        lightNode->SetDirection(Vector3::FORWARD);
        lightNode->Yaw(50);     // horizontal
        lightNode->Pitch(10);   // vertical
        Light* light=lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetBrightness(1.6);
        light->SetColor(Color(1.0,.6,0.3,1));
        light->SetCastShadows(true);

        Node* orbitalCameraNode = scene_->CreateChild("CameraRoot");
        OrbitalCamera* orbitalCamera = orbitalCameraNode->CreateComponent<OrbitalCamera>();
        orbitalCamera->SetTargetPos(Vector3(0, 0, 0));
        cameraNode_ = orbitalCamera->cameraNode;

        Renderer* renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, orbitalCamera->camera));
        renderer->SetViewport(0,viewport);

        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(ListenerApp, HandleKeyDown));
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(ListenerApp, HandleUpdate));
        SubscribeToEvent(E_RENDERUPDATE, URHO3D_HANDLER(ListenerApp, HandleRenderUpdate));
        SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(ListenerApp, HandlePostRenderUpdate));
    }

    virtual void Stop()
    {
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
        Vector3 cameraPosition = cameraNode_->GetWorldPosition();
        Vector3 planetPosition = planetComponent->node->GetWorldPosition();
        /*
        Vector3 posDiff = cameraPosition - planetPosition;
        std::cout << cameraPosition.x_ << " " << cameraPosition.y_ << " " << cameraPosition.z_ << std::endl;
        std::cout << planetPosition.x_ << " " << planetPosition.y_ << " " << planetPosition.z_ << std::endl;
        std::cout << posDiff.x_ << " " << posDiff.y_ << " " << posDiff.z_ << std::endl;
        */
        float ZERO_VOLUME_DISTANCE = 60.0;
        pdPatchManager_.updateX(1.0 - ((cameraPosition - planetPosition).Length() / ZERO_VOLUME_DISTANCE));
        /*
        pdPatchManager_.updateY(pos);
        */
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
