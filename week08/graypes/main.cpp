#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

double ceil_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

void testcase(int n)
{
    std::vector<K::Point_2> graypes;
    graypes.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        graypes.emplace_back();
        std::cin >> graypes.back();
    }

    Triangulation t;
    t.insert(graypes.begin(), graypes.end());

    K::FT min_dist = INT_MAX;
    for (auto ei = t.edges_begin(); ei != t.edges_end(); ++ei)
    {
        K::FT dist = sqrt(t.segment(*ei).squared_length());
        if (dist < min_dist)
            min_dist = dist;
    }

    std::cout << (int) ceil_to_double(min_dist * 50) << std::endl;
}

int main()
{
    int n;
    std::cin >> n;
    while (n > 0)
    {
        testcase(n);
        std::cin >> n;
    }
    return 0;
}
