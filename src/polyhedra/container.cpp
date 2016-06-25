#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;

using floats = vector<float>;
using ints = vector<int>;

struct PlatonicSolid
{
    floats   vertices;
    ints     triangles;
    string   patchName;

};
