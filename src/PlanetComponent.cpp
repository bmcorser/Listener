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
const int polyhedra_max_t = 3;


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
    colours[0] = Color(0, 31, 63);
    colours[1] = Color(0, 116, 217);
    colours[2] = Color(127, 219, 255);
    colours[3] = Color(57, 219, 255);
    colours[4] = Color(57, 204, 204);
    colours[5] = Color(61, 153, 112);
    colours[6] = Color(46, 204, 64);
    colours[7] = Color(1, 255, 112);
    colours[8] = Color(255, 220, 0);
    colours[9] = Color(255, 133, 27);
    colours[10] = Color(255, 65, 54);
    colours[11] = Color(133, 20, 75);
    colours[12] = Color(240, 18, 190);
    colours[13] = Color(177, 13, 201);
    colours[14] = Color(17, 17, 17);
    colours[15] = Color(170, 170, 170);
    colours[16] = Color(221, 221, 221);
    rootNode = GetNode();
    scene = GetScene();
    debugRenderer = scene->GetComponent<DebugRenderer>();
}

void PlanetComponent::Update(float timeStep)
{
}

void PlanetComponent::HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData)
{
    /*
    DebugRenderer* debug = node->GetScene()->GetComponent<DebugRenderer>();
    for (unsigned i = 0; i < size(polyhedron.vertices); i += 3)
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


Node* PlanetComponent::place(Vector3 pos)
{
    node = scene->CreateChild("Planet");
    node->SetPosition(pos);
    // node->SetRotation(Quaternion(Random(360.0f), Random(360.0f), Random(360.0f)));
    node->SetScale(15);

    static default_random_engine e;
    static uniform_int_distribution<int> d(1, polyhedra_max_t);
    polyhedra_t solid_name = static_cast<polyhedra_t>(d(e));

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

    /*
    Material* material = new Material(context_);
    // ("Materials/DefaultMaterial.xml");
    if (material) {
        material->SetShaderParameter("MatDiffColor", colours[0].ToVector4() / Vector4(255, 255, 255, 1));
        material->SetShaderParameter("MatSpecColor", colours[0].ToVector4() / Vector4(255, 255, 255, 1));
        mushroomObject->SetMaterial(material);
    }
    mushroomObject->SetCastShadows(true);

    */
    return node;
}
