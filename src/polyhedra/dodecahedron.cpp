struct Dodecahedron : public PlatonicSolid
{
    Dodecahedron()
    {
        const float phi = (1.0 + sqrt(5.0)) / 2.0;

        const float b = 1.0 / phi;
        const float c = 2.0 - phi;
        vertices  = {
             0,  1,  c,
             0,  1, -c,
             0, -1,  c,
             0, -1, -c,
             1,  c,  0,
             1, -c,  0,
             b,  b,  b,
             b,  b, -b,
             b, -b,  b,
             b, -b, -b,
             c,  0,  1,
             c,  0, -1,
            -1,  c,  0,
            -1, -c,  0,
            -b,  b,  b,
            -b,  b, -b,
            -b, -b,  b,
            -b, -b, -b,
            -c,  0,  1,
            -c,  0, -1,
        };

        for(auto& v: vertices)
            v = v / 2;

        triangles = {
            18, 10, 14,
             6,  0, 14,
             6, 14, 10,

             8, 10, 18,
            16,  2,  8,
            16,  8, 18,

            11, 17, 19,
            17,  9,  3,
            11,  9, 17,

             7, 11, 19,
            15,  1,  7,
            15,  7, 19,

             1,  0,  6,
             6,  4,  7,
             1,  6,  7,

             0,  1, 15,
            15, 12, 14,
             0, 15, 14,

             3,  2, 16,
            16, 13, 17,
             3, 16, 17,

             2,  3,  9,
             9,  5,  8,
             2,  9,  8,

             4,  8,  5,
             8,  6, 10,
             4,  6,  8,

             7,  4,  5,
             9, 11,  7,
             9,  7,  5,

            17, 13, 12,
            15, 19, 17,
            15, 17, 12,

            12, 13, 14,
            18, 14, 16,
            14, 13, 16,

        };
        patchName = "dodecahedron.pd";
    }
};
