struct Icosahedron : public PlatonicSolid
{
    Icosahedron()
    {
        const float phi = (1.0 + sqrt(5.0)) / 2.0;

        const float a = 1.0 / 2.0;
        const float b = 1.0 / (2.0 * phi);
        vertices  = {
            0,  b, -a,
            b,  a,  0,
           -b,  a,  0,
            0,  b,  a,
            0, -b,  a,
           -a,  0,  b,
            0, -b, -a,
            a,  0, -b,
            a,  0,  b,
           -a,  0, -b,
            b, -a,  0,
           -b, -a,  0,
       };

        triangles = {
            2,  1,  0,
            3,  1,  2,
            8,  3,  4,
            6,  0,  7,
            11, 6,  10,
            4,  11, 10,
            9,  0,  6,
            10, 7,  8,
            5,  2,  9,
            4,  3,  5,
            3,  2,  5,
            7,  1,  8,
            8,  1,  3,
            2,  0,  9,
            7,  0,  1,
            9,  11, 5,
            9,  6,  11,
            10, 6,  7,
            11, 4,  5,
            8,  4,  10,
        };
        patchName = "icosahedron.pd";
    }
};
