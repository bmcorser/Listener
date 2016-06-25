struct Hexahedron : public PlatonicSolid
{
    Hexahedron()
    {
        vertices = {
            -1,  1,  1,
            -1,  1, -1,
            -1, -1,  1,
            -1, -1, -1,
             1,  1,  1,
             1,  1, -1,
             1, -1,  1,
             1, -1, -1,
        };

        for(auto& v: vertices)
            v = v / 2;

        triangles = {
            3, 7, 6,
            6, 2, 3,
            3, 2, 0,
            0, 1, 3,
            2, 6, 4,
            4, 0, 2,
            1, 0, 4,
            4, 5, 1,
            7, 5, 4,
            4, 6, 7,
            3, 1, 5,
            5, 7, 3,
        };
        patchName = "hexahedron.pd";
    }
};
