#pragma once

#include "CustomGeo.cpp"

using namespace Urho3D;

class PlanetComponent: public LogicComponent
{

URHO3D_OBJECT(PlanetComponent, LogicComponent);

public:
    /// Construct.
    PlanetComponent(Context* context);

    /// Register object factory and attributes.
    static void RegisterObject(Context* context);
    virtual void Start();
    void Update(float timeStep);
    virtual void FixedUpdate(float timeStep) {};

    Node* place(Vector3 pos);

private:

    SharedPtr<DebugRenderer> debugRenderer;
    SharedPtr<Node> rootNode;
    SharedPtr<Scene> scene;
    Color colours[17];

};
