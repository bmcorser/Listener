#ifndef CUSTOMGEO_H
#define CUSTOMGEO_H

#pragma once

#include <Urho3D/Core/Object.h>

using namespace Urho3D;

class CustomGeo : public Object
{

    URHO3D_OBJECT(CustomGeo,Object);

public:

    CustomGeo(Context* context);
    ~CustomGeo();

    void AddPoint(const Vector3 p);
    void AddPointUV(const Vector2 uv);
    void AddPointColor(const Vector3 c);
    //PODVector<Vector3>* GetPoints(){return &points_;};
    //void SetPoint(const unsigned short i, const Vector3 p);
    void HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData);

    void AddTriangle(const unsigned p1, const unsigned p2, const unsigned p3, const bool t=false);//t is to calc tangents
    void Surface(const unsigned slices, const unsigned stacks, Vector3 (CustomGeo::*fptr)(void*, float, float), void* context);
    void Subdivide(const unsigned short depth=1);
    void GetSphere(const unsigned short u, const unsigned short v);
    void GetKlein(const unsigned short u, const unsigned short v);
    void Build(Node* node, const bool smooth = true, const bool rigid = false, const unsigned layer = 0, const unsigned mask = 0);
    Node* GetNode(){return node_;};
    PODVector<Vector3> points_;

private:

    Vector3 Normal(const Vector3& p1, const Vector3& p2, const Vector3& p3);
    Vector3 Tangent(Vector3 n);
    Vector3 Tangent(const unsigned p1, const unsigned p2, const unsigned p3);//diferent method
    Vector3 GetSmoothNormal(const unsigned i);
    void FitBB(const Vector3 p);
    void Debug(const String label, const String value);
    void DoSubdivide();//this does the actual subdivision, called from Subdivide

    Vector3 Sphere(void* context, const float u, const float v);
    Vector3 Klein(void* context, const float u, const float v);

    SharedPtr<Node> node_;

    PODVector<Vector2> uvs_;
    PODVector<Vector3> colors_;
    PODVector<Vector3> tangents_;
    PODVector<unsigned> ids_;
    Vector< PODVector<unsigned> > shared_normal_ids_;
    PODVector<Vector3> normals_;

    //bool smooth_;

    Vector3 bbmin_;
    Vector3 bbmax_;
};

#endif
