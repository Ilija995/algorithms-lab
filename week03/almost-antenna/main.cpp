#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <vector>
#include <cmath>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef  K::Point_2                      Point;

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a - 1 >= x) a -= 1;
    return a;
}

void testcase(int N)
{
    Point points[N];
    long x, y;
    for (int i = 0; i < N; ++i)
    {
        std::cin >> x >> y;
        points[i] = Point(x, y);
    }

    Min_circle mc(points, points + N, true);
    Traits::Circle c = mc.circle();

    auto min_sq_radius = c.squared_radius();
    for (auto it_point = mc.support_points_begin(); it_point != mc.support_points_end(); ++it_point)
    {
        Point new_points[N - 1];
        bool skipped = false;
        for (int i = 0; i < N; ++i)
        {
            if (points[i] != *it_point)
            {
                new_points[(skipped) ? i - 1 : i] = points[i];
            }
            else skipped = true;
        }

        Min_circle mc1(new_points, new_points + N - 1, true);
        Traits::Circle c1 = mc1.circle();

        min_sq_radius = std::min(min_sq_radius, c1.squared_radius());
    }

    K::FT d = sqrt(min_sq_radius);
    std::cout << std::fixed << std::setprecision(0) << ceil_to_double(d) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int N;
    std::cin >> N;
    while (N > 0)
    {
        testcase(N);
        std::cin >> N;
    }
    return 0;
}
