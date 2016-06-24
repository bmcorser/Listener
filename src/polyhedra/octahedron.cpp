struct Octahedron : public PlatonicSolid
{
    Octahedron()
    {
        const float a = 1.0 / (2.0 * sqrt(2.0));
        const float b = 1.0 / 2.0;
        vertices  = {
            -a,  0,  a,
            -a,  0, -a,
             0,  b,  0,
             0, -b,  0,
             a,  0,  a,
             a,  0, -a,
        };
        triangles = {
            1, 0, 2,
            5, 1, 2,
            5, 2, 4,
            4, 2, 0,
            5, 3, 1,
            1, 3, 0,
            4, 3, 5,
            4, 0, 3,
        };
    }
};
