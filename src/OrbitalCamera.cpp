#include "OrbitalCamera.hpp"
#include <Urho3D/DebugNew.h>



OrbitalCamera::OrbitalCamera(Context* context) : LogicComponent(context)
{
    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE | USE_UPDATE);
}

void OrbitalCamera::RegisterObject(Context* context)
{
    context->RegisterFactory<OrbitalCamera>();
}

void OrbitalCamera::Start()
{
    leftAnalogX = 0;
    leftAnalogY = 0;
    rightAnalogY = 0;

    Node* node = GetNode();
    Scene* scene = GetScene();
    debugRenderer = scene->GetComponent<DebugRenderer>();

    minRadius = 10;
    maxRadius = 20;
    currentPitch = 30;
    currentYaw = 40;
    radius = 10;
    target = Vector3::ZERO;

    containerNode = node->CreateChild("OrbitalCameraContainer");
    debugRenderer->AddNode(containerNode);
    targetTransform = containerNode->CreateComponent<SmoothedTransform>();

        yawNode = containerNode->CreateChild("OrbitalCameraYaw");
        yawNode->SetRotation(Quaternion(currentYaw, Vector3::UP));
        yawTransform = yawNode->CreateComponent<SmoothedTransform>();

            pitchNode = yawNode->CreateChild("OrbitalCameraPitch");
            pitchNode->SetRotation(Quaternion(currentPitch, 0, 0));
            pitchTransform = pitchNode->CreateComponent<SmoothedTransform>();

                cameraNode = pitchNode->CreateChild("OrbitalCamera");
                cameraNode->SetPosition(Vector3(0, 0, -radius));
                cameraNode->SetRotation(Quaternion(0, 0, 0));

                    camera = cameraNode->CreateComponent<Camera>();  // public

                balanceNode = pitchNode->CreateChild("OrbitalCameraBalance");
                balanceNode->SetPosition(Vector3(0, 0, radius));

    SubscribeToEvent(E_JOYSTICKAXISMOVE, URHO3D_HANDLER(OrbitalCamera, HandleJoystickAxis));

}

void OrbitalCamera::HandleJoystickAxis(StringHash eventType,VariantMap& eventData)
{
    using namespace JoystickAxisMove;

    float pos = eventData[P_POSITION].GetFloat();

    if (eventData[P_AXIS] == CONTROLLER_AXIS_LEFTX) {
        leftAnalogX = pos;
    }
    if (eventData[P_AXIS] == CONTROLLER_AXIS_LEFTY) {
        leftAnalogY = pos;
    }
    if (eventData[P_AXIS] == CONTROLLER_AXIS_RIGHTY) {
        rightAnalogY = pos;
    }
}


void OrbitalCamera::Update(float timeStep)
{
    currentYaw -= leftAnalogX;
    yawNode->SetRotation(Quaternion(currentYaw, Vector3::UP));

    currentPitch -= leftAnalogY;
    // currentPitch = Clamp(currentPitch, -90.0f, 90.0f);
    pitchNode->SetRotation(Quaternion(currentPitch, Vector3::RIGHT));

    const float MOUSE_SENSITIVITY = 0.1f;
    Input* input = GetSubsystem<Input>();
    containerNode->SetPosition(centre.Lerp(target, timeStep));

    // Update class
    cameraPosition = cameraNode->GetWorldPosition();
    centre = containerNode->GetWorldPosition();
    radius -= rightAnalogY;
    cameraNode->SetPosition(Vector3(0.0f, 0.0f, radius));

    /*
    camera->DrawDebugGeometry(debugRenderer, true);
    debugRenderer->AddLine(cameraPosition, target, Color::GREEN);
    debugRenderer->AddLine(cameraPosition, centre, Color::RED);

    UI* ui = GetSubsystem<UI>();
    if (ui->GetFocusElement())
        return;

    Cursor* cursor = ui->GetCursor();
    cursor->SetVisible(!input->GetQualifierDown(QUAL_CTRL));

    if (!ui->GetCursor()->IsVisible())
    {
        IntVector2 mouseMove = input->GetMouseMove();
        currentYaw += MOUSE_SENSITIVITY * mouseMove.x_;
        currentPitch += MOUSE_SENSITIVITY * mouseMove.y_;
        currentPitch = Clamp(currentPitch, -90.0f, 90.0f);
        yawNode->SetRotation(Quaternion(currentYaw, Vector3::UP));
        pitchNode->SetRotation(Quaternion(currentPitch, Vector3::RIGHT));
    }
    */
}
