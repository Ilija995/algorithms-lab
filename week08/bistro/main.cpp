#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

void testcase(int n)
{
    std::vector<K::Point_2> bistros;
    bistros.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        bistros.emplace_back();
        std::cin >> bistros.back();
    }

    Triangulation t;
    t.insert(bistros.begin(), bistros.end());

    int m;
    std::cin >> m;
    K::Point_2 p;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> p;
        auto vh = t.nearest_vertex(p);
        std::cout << (long long)CGAL::to_double(CGAL::squared_distance(vh->point(), p)) << std::endl;
    }
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
