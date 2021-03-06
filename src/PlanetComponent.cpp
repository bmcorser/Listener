#include "PlanetComponent.hpp"
#include "polyhedra/tetrahedron.cpp"
#include "polyhedra/octahedron.cpp"
#include "polyhedra/hexahedron.cpp"
#include "polyhedra/icosahedron.cpp"
#include "polyhedra/dodecahedron.cpp"
#include "stdlib.h"


enum class polyhedra_t: int {
    tetrahedron, octahedron, hexahedron, icosahedron, dodecahedron
};
const int polyhedra_max_t = 4;


PlanetComponent::PlanetComponent(Context* context) : LogicComponent(context)
{
    SetUpdateEventMask(USE_FIXEDUPDATE | USE_UPDATE);
    SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(PlanetComponent, HandlePostRenderUpdate));
}

void PlanetComponent::RegisterObject(Context* context)
{
    context->RegisterFactory<PlanetComponent>();
}

void PlanetComponent::Start()
{
}

void PlanetComponent::Update(float timeStep)
{
}

void PlanetComponent::HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData)
{
    /* doesn't work with random rotation
    DebugRenderer* debug = node->GetScene()->GetComponent<DebugRenderer>();
    for (unsigned i = 0; i < polyhedron.vertices.size(); i += 3)
    {
        Vector3 pos = Vector3(
            polyhedron.vertices[i] * 15,
            polyhedron.vertices[i + 1] * 15,
            polyhedron.vertices[i + 2] * 15
        );
        debug->AddSphere(Sphere(pos, 0.2), Color::RED);
    }
    */

}
Node* PlanetComponent::place(Vector3 pos, int colour_id, int polyhedron_id, int scale_)
{
    colour = colours[colour_id];
    scale = scale_;

    node = GetScene()->CreateChild("Planet");
    node->SetPosition(pos);
    node->SetRotation(Quaternion(Random(360.0f), Random(360.0f), Random(360.0f)));
    node->SetScale(scale);

    polyhedra_t polyhedron_type = static_cast<polyhedra_t>(polyhedron_id);
    switch(polyhedron_type)
    {

        case polyhedra_t::tetrahedron:
            polyhedron = Tetrahedron();
            break;

        case polyhedra_t::octahedron:
            polyhedron = Octahedron();
            break;

        case polyhedra_t::hexahedron:
            polyhedron = Hexahedron();
            break;

        case polyhedra_t::icosahedron:
            polyhedron = Icosahedron();
            break;

        case polyhedra_t::dodecahedron:
            polyhedron = Dodecahedron();
            break;

    }

    CustomGeo* cg = new CustomGeo(context_);

    for (unsigned i = 0; i < polyhedron.vertices.size(); i += 3)
    {
        cg->AddPoint(
            Vector3(
                polyhedron.vertices[i],
                polyhedron.vertices[i + 1],
                polyhedron.vertices[i + 2]
            )
        );
    }

    for (unsigned i = 0; i < polyhedron.triangles.size(); i += 3)
    {
        cg->AddTriangle(
            polyhedron.triangles[i],
            polyhedron.triangles[i + 1],
            polyhedron.triangles[i + 2],
            false
        );
    }

    cg->Build(node, false, false, 32, 63);


    Material* material = new Material(context_);
    material->SetShaderParameter("MatDiffColor", colour.ToVector4() / Vector4(255, 255, 255, 1));
    material->SetShaderParameter("MatSpecColor", colour.ToVector4() / Vector4(255, 255, 255, 1));
    node->GetComponent<StaticModel>()->SetMaterial(material);

    return node;
}
