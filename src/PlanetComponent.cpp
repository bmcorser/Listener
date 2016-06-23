#include "PlanetComponent.hpp"

PlanetComponent::PlanetComponent(Context* context) : LogicComponent(context)
{
    SetUpdateEventMask(USE_FIXEDUPDATE | USE_UPDATE);
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

Node* PlanetComponent::place(Vector3 pos)
{
    Node* node = scene->CreateChild("Planet");
    node->SetPosition(pos);
    /*
    node->SetRotation(Quaternion(Random(360.0f), Random(360.0f), Random(360.0f)));
    node->SetScale(2.0f + Random(15));
    */
    node->SetScale(15);

    CustomGeo* cg = new CustomGeo(context_);

    float phi = (1 + sqrt(5)) / 2;
    float a = 1 / 2;
    float b = 1 / (2 * phi);

    // icosahedron
    Vector3 v1  = Vector3( 0,  b, -a);
    Vector3 v2  = Vector3( b,  a,  0);
    Vector3 v3  = Vector3(-b,  a,  0);
    Vector3 v4  = Vector3( 0,  b,  a);
    Vector3 v5  = Vector3( 0, -b,  a);
    Vector3 v6  = Vector3(-a,  0,  b);
    Vector3 v7  = Vector3( 0, -b, -a);
    Vector3 v8  = Vector3( a,  0, -b);
    Vector3 v9  = Vector3( a,  0,  b);
    Vector3 v10 = Vector3(-a,  0, -b);
    Vector3 v11 = Vector3( b, -a,  0);
    Vector3 v12 = Vector3(-b, -a,  0);

    cg->AddPoint(v1 );
    cg->AddPoint(v2 );
    cg->AddPoint(v3 );
    cg->AddPoint(v4 );
    cg->AddPoint(v5 );
    cg->AddPoint(v6 );
    cg->AddPoint(v7 );
    cg->AddPoint(v8 );
    cg->AddPoint(v9 );
    cg->AddPoint(v10);
    cg->AddPoint(v11);
    cg->AddPoint(v12);

    // draw the icosahedron's 20 triangular faces:
    cg->AddTriangle(0, 1, 2, true);
    cg->AddTriangle(3, 2, 1, true);
    cg->AddTriangle(3, 4, 5, true);
    cg->AddTriangle(3, 8, 4, true);
    cg->AddTriangle(0, 6, 7, true);
    cg->AddTriangle(0, 9, 6, true);
    cg->AddTriangle(4, 10, 11, true);
    cg->AddTriangle(6, 11, 10, true);
    cg->AddTriangle(2, 5, 9, true);
    cg->AddTriangle(11, 9, 5, true);
    cg->AddTriangle(1, 7, 8, true);
    cg->AddTriangle(10, 8, 7, true);
    cg->AddTriangle(3, 5, 2, true);
    cg->AddTriangle(3, 1, 8, true);
    cg->AddTriangle(0, 2, 9, true);
    cg->AddTriangle(0, 7, 1, true);
    cg->AddTriangle(6, 9, 11, true);
    cg->AddTriangle(6, 10, 7, true);
    cg->AddTriangle(4, 11, 5, true);
    cg->AddTriangle(4, 8, 10, true);

    cg->Build(node, false, true, 32, 63);

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
