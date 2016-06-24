#pragma once

#include <string>
#include <iostream>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/SmoothedTransform.h>

class OrbitalCamera: public LogicComponent
{

URHO3D_OBJECT(OrbitalCamera, LogicComponent);

public:
    /// Construct.
    OrbitalCamera(Context* context);

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);

    virtual void Start();
    void Update(float timeStep);
    virtual void FixedUpdate(float timeStep) {};
    void HandleJoystickAxis(StringHash eventType,VariantMap& eventData);

    void SetTargetNode(Node* node) { target = node->GetPosition(); }
    void SetTargetPos(Vector3 pos) { target = pos; }
    void SetRadiusLimits(float minDistance, float maxDistance);

    SharedPtr<Camera> camera;
    Vector3 target;
    float leftAnalogX;
    float leftAnalogY;
    float rightAnalogY;

    SharedPtr<Node> containerNode;
    SharedPtr<Node> yawNode;
    SharedPtr<Node> pitchNode;
    SharedPtr<Node> cameraNode;
    SharedPtr<Node> balanceNode;


private:

    SharedPtr<SmoothedTransform> targetTransform;
    SharedPtr<SmoothedTransform> pitchTransform;
    SharedPtr<SmoothedTransform> yawTransform;

    SharedPtr<DebugRenderer> debugRenderer;

    float minRadius;
    float maxRadius;
    float radius;

    float currentPitch;
    float currentYaw;
    float currentWorldYaw;
    float targetYaw;

    Vector3 centre;
    Vector3 cameraPosition;

};
