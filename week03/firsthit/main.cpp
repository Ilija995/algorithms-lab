#include <iostream>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase(int N)
{
    Point origin, direction;
    std::cin >> origin >> direction;
    Ray r = Ray(origin, direction);

    Point u, v;
    bool found_intersection = false;
    Point closest_point;
    K::FT min_sq_dist;
    for (int i = 0; i < N; ++i)
    {
        long long a,b,c,d;
        std::cin >> a >> b >> c >> d;
        Point ab(a,b);
        Point cd(c,d);
        Segment s(ab,cd);

        if (CGAL::do_intersect(r, s))
        {
            auto o = CGAL::intersection(r, s);
            if (const Point* op = boost::get<Point>(&*o))
            {
                K::FT sq_dist = CGAL::squared_distance(origin, *op);
                if (!found_intersection || sq_dist < min_sq_dist)
                {
                    min_sq_dist = sq_dist;
                    closest_point = *op;
                    found_intersection = true;
                }
            }
            else if (const Segment* os = boost::get<Segment>(&*o))
            {
                K::FT sq_dist_source = CGAL::squared_distance(origin, os->source());
                K::FT sq_dist_target = CGAL::squared_distance(origin, os->target());

                if (sq_dist_source < sq_dist_target)
                {
                    if (!found_intersection || sq_dist_source < min_sq_dist)
                    {
                        min_sq_dist = sq_dist_source;
                        closest_point = os->source();
                        found_intersection = true;
                    }
                }
                else
                {
                    if (!found_intersection || sq_dist_target < min_sq_dist)
                    {
                        min_sq_dist = sq_dist_target;
                        closest_point = os->target();
                        found_intersection = true;
                    }
                }
            }
        }
    }

    if (!found_intersection) std::cout << "no" << std::endl;
    else std::cout << std::fixed << std::setprecision(0) << floor_to_double(closest_point.x()) << " " << floor_to_double(closest_point.y()) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int N;
    std::cin >> N;
    while (N)
    {
        testcase(N);
        std::cin >> N;
    }
    return 0;
}
