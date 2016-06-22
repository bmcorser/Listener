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
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    Node* node = scene->CreateChild("Planet");
    node->SetPosition(pos);
    node->SetRotation(Quaternion(Random(360.0f), Random(360.0f), Random(360.0f)));
    node->SetScale(2.0f + Random(15));

    StaticModel* staticModel = node->CreateComponent<StaticModel>();
    Model* model = new Model(context_);
    VertexBuffer* vertexBuffer = new VertexBuffer(context_);
    vertexBuffer->SetSize(12, NULL);
    /*
    1, 1, 1,
    -1, 1,-1,
    1, -1,-1,
    -1, 1,-1,
    -1, -1, 1,
    1, -1,-1,
    1, 1, 1,
    1, -1,-1,
    -1, -1, 1,
    1, 1, 1,
    -1, -1, 1,
    -1, 1,-1,
    */
    std::cout << "count " << vertexBuffer->GetVertexCount() << std::endl;
    std::cout << "size " << vertexBuffer->GetVertexSize() << std::endl;
    exit(0);
    unsigned char* vertexData = (unsigned char*)vertexBuffer->Lock(0, vertexBuffer->GetVertexCount());
    vertexBuffer->Unlock();

    /*
    Model* model = cache->GetResource<Model>("Models/Sphere.mdl");
    mushroomObject->SetModel(mushroomModel);
    VertexBuffer* buffer = mushroomModel->GetGeometry(0, 0)->GetVertexBuffer(0);
    unsigned char* vertexData = (unsigned char*)buffer->Lock(0, buffer->GetVertexCount());
    unsigned numVertices = buffer->GetVertexCount();
    unsigned vertexSize = buffer->GetVertexSize();
    PODVector<Vector3> vertices;
    for (unsigned i = 0; i < numVertices; ++i)
    {
        Vector3& vertex = *reinterpret_cast<Vector3*>(vertexData + i * vertexSize);
        // vertex.x_ += 3;
    }
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
