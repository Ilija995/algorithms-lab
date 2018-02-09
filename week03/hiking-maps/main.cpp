#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

bool contains(const std::vector<K::Point_2>& t, const K::Point_2& p)
{
    return !CGAL::right_turn(t[0],t[1],p) &&
            !CGAL::right_turn(t[2],t[3],p) &&
            !CGAL::right_turn(t[4],t[5],p);
}

void testcase()
{
    int n_points, n_triangles;
    std::cin >> n_points >> n_triangles;

    //std::cout << n_points << ", " << n_triangles << std::endl;

    std::vector<K::Point_2> path;
    path.reserve(n_points);
    int a, b, c, d;
    for (int i_point = 0; i_point < n_points; ++i_point)
    {
        std::cin >> a >> b;
        //std::cout << "A: " << a << ", B: " << b << std::endl;
        K::Point_2 point(a, b);
        //std::cout << "Point: " << point << std::endl;
        path.emplace_back(point);
        //std::cout << "Loaded point " << i_point << ": " << path[i_point] << std::endl;
    }

    //for (int i = 0; i < n_points; ++i) std::cout << path[i] << std::endl;

    //std::cout << "Loaded legs\n";

    std::vector< std::vector<K::Point_2> > triangles;
    triangles.reserve(n_triangles);
    for (int i_triangle = 0; i_triangle < n_triangles; ++i_triangle)
    {
        std::vector<K::Point_2> edge_points;
        edge_points.reserve(6);
        for (int i_edge_point = 0; i_edge_point < 6; ++i_edge_point)
        {
            std::cin >> a >> b;
            K::Point_2 point(a, b);
            edge_points.emplace_back(point);
        }

        for (int i_edge_point = 0; i_edge_point < 6; i_edge_point += 2)
        {
            if (CGAL::right_turn(edge_points[i_edge_point], edge_points[i_edge_point + 1], edge_points[(i_edge_point + 2) % 6]))
                std::swap(edge_points[i_edge_point], edge_points[i_edge_point + 1]);
        }

        triangles.emplace_back(edge_points);
    }

    //std::cout << "Loaded triangles\n";

    int min_cost = INT_MAX;
    int i_triangle = -1;
    bool contains_leg = false;
    while (i_triangle < n_triangles - 1 && !contains_leg)
    {
        ++i_triangle;
        int i_point = -1;
        while (i_point < n_points - 2 && !contains_leg)
        {
            ++i_point;
            contains_leg = contains(triangles[i_triangle], path[i_point]) && contains(triangles[i_triangle], path[i_point + 1]);
        }
    }

    //std::cout << "Found first triangle " << i_triangle << std::endl;

    std::vector<int> leg_to_triangle(n_points - 1);
    for (int i = 0; i < n_points - 1; ++i) leg_to_triangle[i] = -1;

    int i_point;
    int j_triangle = i_triangle;
    while (j_triangle < n_triangles)
    {
        for (i_point = 0; i_point < n_points - 1; ++i_point)
        {
            if (contains(triangles[j_triangle], path[i_point]) && contains(triangles[j_triangle], path[i_point + 1]))
            {
                leg_to_triangle[i_point] = j_triangle;
            }
        }

        ++j_triangle;

        int min_leg_assignment = INT_MAX;
        for (i_point = 0; i_point < n_points - 1; ++i_point)
        {
            if (leg_to_triangle[i_point] != -1 && leg_to_triangle[i_point] < min_leg_assignment)
                min_leg_assignment = leg_to_triangle[i_point];
        }

        i_triangle = min_leg_assignment;

        i_point = 0;
        while (i_point < n_points - 1 && leg_to_triangle[i_point] != -1) ++i_point;

        //int no_unassigned = 0;
        //for (int i = 0; i < n_points - 1; ++i) if (leg_to_triangle[i] == -1) { ++no_unassigned; std::cout << i << " ";}
        //std::cout << "\nNo. unassigned: " << no_unassigned << std::endl;

        if (i_point == n_points - 1 && j_triangle - i_triangle < min_cost)
        {
            min_cost = j_triangle - i_triangle;
        }

        //std::cout << "New interval [" << i_triangle << ", " << j_triangle << ") - " << j_triangle - i_triangle << std::endl;
    }

    std::cout << min_cost << std::endl;
/*
    std::cout << "Leg 57: " << path[57] << ", " << path[58] << std::endl;
    std::cout << leg_to_triangle[57] << std::endl;
    for (int i = 0; i < n_triangles; ++i)
    {
        std::cout << i << ": " << ((CGAL::do_intersect(triangles[i], path[57])) ? "T" : "F") << " " << ((CGAL::do_intersect(triangles[i], path[58])) ? "T" : "F") << std::endl;
    }*/
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
