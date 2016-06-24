#pragma once

#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Scene/LogicComponent.h>
#include "CustomGeo.cpp"
#include "polyhedra/container.cpp"

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
    void HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData);

    Node* place(Vector3 pos, int colour_id, int polyhedron_id, int scale);
    Node* node;
    CustomGeo* cg;
    PlatonicSolid polyhedron;

private:

    Color colours[17] = {
        Color(0, 31, 63),
        Color(0, 116, 217),
        Color(127, 219, 255),
        Color(57, 219, 255),
        Color(57, 204, 204),
        Color(61, 153, 112),
        Color(46, 204, 64),
        Color(1, 255, 112),
        Color(255, 220, 0),
        Color(255, 133, 27),
        Color(255, 65, 54),
        Color(133, 20, 75),
        Color(240, 18, 190),
        Color(177, 13, 201),
        Color(17, 17, 17),
        Color(170, 170, 170),
        Color(221, 221, 221),
    };

};
