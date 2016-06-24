struct Tetrahedron : public PlatonicSolid
{
    Tetrahedron()
    {
        vertices  = {
            -1,  1, -1,
            -1, -1,  1,
             1,  1,  1,
             1, -1, -1,
        };
        triangles = {
            2, 3, 0,
            1, 0, 3,
            2, 1, 3,
            2, 0, 1,
        };
    }
};
