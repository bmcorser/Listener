#include "PlanetComponent.hpp"

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
    // DebugRenderer* debug = node->GetScene()->GetComponent<DebugRenderer>();
}

Node* PlanetComponent::place(Vector3 pos)
{
    node = scene->CreateChild("Planet");
    node->SetPosition(pos);
    node->SetRotation(Quaternion(Random(360.0f), Random(360.0f), Random(360.0f)));
    node->SetScale(2.0f + Random(15));

    CustomGeo* cg = new CustomGeo(context_);

    // icosahedron
    float phi = (1.0 + sqrt(5.0)) / 2.0;
    float a = 1.0 / 2.0;
    float b = 1.0 / (2.0 * phi);

    Vector3 vertices[] = {
        Vector3( 0,  b, -a),
        Vector3( b,  a,  0),
        Vector3(-b,  a,  0),
        Vector3( 0,  b,  a),
        Vector3( 0, -b,  a),
        Vector3(-a,  0,  b),
        Vector3( 0, -b, -a),
        Vector3( a,  0, -b),
        Vector3( a,  0,  b),
        Vector3(-a,  0, -b),
        Vector3( b, -a,  0),
        Vector3(-b, -a,  0),
    };

    for (unsigned i = 0; i < 12; i += 1)
    {
        cg->AddPoint(vertices[i]);
    }

    cg->AddTriangle(2, 1, 0, true);
    cg->AddTriangle(3, 1, 2, true);
    cg->AddTriangle(8, 3, 4, true);
    cg->AddTriangle(6, 0, 7, true);
    cg->AddTriangle(11, 6, 10, true);
    cg->AddTriangle(4, 11, 10, true);
    cg->AddTriangle(9, 0, 6, true);
    cg->AddTriangle(10, 7, 8, true);
    cg->AddTriangle(5, 2, 9, true);
    cg->AddTriangle(4, 3, 5, true);
    cg->AddTriangle(3, 2, 5, true);
    cg->AddTriangle(7, 1, 8, true);
    cg->AddTriangle(8, 1, 3, true);
    cg->AddTriangle(2, 0, 9, true);
    cg->AddTriangle(7, 0, 1, true);
    cg->AddTriangle(9, 11, 5, true);
    cg->AddTriangle(9, 6, 11, true);
    cg->AddTriangle(10, 6, 7, true);
    cg->AddTriangle(11, 4, 5, true);
    cg->AddTriangle(8, 4, 10, true);

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
