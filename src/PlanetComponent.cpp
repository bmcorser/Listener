#include "PlanetComponent.hpp"
#include "polyhedra/tetrahedron.cpp"
#include "polyhedra/octahedron.cpp"
#include "polyhedra/icosahedron.cpp"


template<typename T, int sz>
int size(T(&)[sz])
{
    return sz;
}

enum class polyhedra_t : int { tetrahedron, octahedron, icosahedron};
const int polyhedra_max_t = 2;


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

}


Node* PlanetComponent::place(Vector3 pos)
{
    node = GetScene()->CreateChild("Planet");
    node->SetPosition(pos);
    node->SetRotation(Quaternion(Random(360.0f), Random(360.0f), Random(360.0f)));
    node->SetScale(15);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0, polyhedra_max_t);

    polyhedra_t solid_name = static_cast<polyhedra_t>(uni(rng));

    switch(solid_name)
    {
    case polyhedra_t::tetrahedron:
        polyhedron = Tetrahedron();
        break;

    case polyhedra_t::octahedron:
        polyhedron = Octahedron();
        break;

    case polyhedra_t::icosahedron:
        polyhedron = Icosahedron();
        break;
    default:
        std::cout << "polyhedra_t " << to_string(static_cast<int>(solid_name)) << std::endl;
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


    std::uniform_int_distribution<int> colour_dist(0, 16);
    Color colour = colours[colour_dist(rng)];
    Material* material = new Material(context_);
    material->SetShaderParameter("MatDiffColor", colour.ToVector4() / Vector4(255, 255, 255, 1));
    material->SetShaderParameter("MatSpecColor", colour.ToVector4() / Vector4(255, 255, 255, 1));
    node->GetComponent<StaticModel>()->SetMaterial(material);

    return node;
}
