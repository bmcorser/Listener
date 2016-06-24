#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;

using float_set = vector<float>;
using int_set = vector<int>;

/*
void do_something(const number_set::value_type& v)
{
 // ...whatever...
}
*/

struct PlatonicSolid
{
    float_set   vertices;
    int_set     triangles;

    /*
 Polyhedron()
 {
    // ...showing a couple ways to do things:
    for_each(begin(vertices), end(vertices),
            [](const auto& v) {
                    do_something(v);
            });

    for(const auto& t : triangles)
        do_something(t);
 }
 */
};

/*
enum class shape_t : int { tetrahedron, octahedron };
const int max_shape_t = 2;      // JFW: feels a bit hacky, but we'll move on

Polyhedron maybe_random(const shape_t shape_type)
{
 switch(shape_type)
 {
  case shape_t::tetrahedron:
                return Tetrahedron();
                break;

  case shape_t::octahedron:
                return Octahedron();
                break;
 }

 // Overkill, but cute:
 ostringstream os;
 os << "shape_t " << to_string(static_cast<int>(shape_type));
 throw std::out_of_range(os.str());
}

Polyhedron maybe_random()
{
 static default_random_engine e;
 static uniform_int_distribution<int> d(1, max_shape_t);

 return maybe_random(static_cast<shape_t>(d(e)));
}
*/
