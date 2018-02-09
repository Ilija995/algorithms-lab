#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase()
{
    //std::cout << "--------------------\n";

    //std::cout << "1\n";
    int z, u, v, w;
    std::cin >> z >> u >> v >> w;
    int a, g;
    std::cin >> a >> g;
    std::vector<K::Point_2> as(a);
    std::vector< std::pair<K::Point_2, int> > gs;
    gs.reserve(g);
    std::vector<int> us(g), vs(g), ws(g), zs(a);

    K::Point_2 p;
    for (int i = 0; i < g; ++i)
    {
        gs.emplace_back();
        std::cin >> gs.back().first >> us[i] >> vs[i] >> ws[i];
        gs.back().second = i;
    }

    //std::cout << "2\n";

    for (int i = 0; i < a; ++i)
        std::cin >> as[i] >> zs[i];
/*
    if (z != 24 && z != 4250 && z != 2780 && z != 6409)
        return;*/
/*
    for (int i = 0; i < g; ++i)
        std::cout << "G(" << i << "): " << us[i] << " " << vs[i] << " " << ws[i] << std::endl;
    for (int i = 0; i < a; ++i)
        std::cout << "A(" << i << "): " << zs[i] << std::endl;*/

    Delaunay dt;
    dt.insert(gs.begin(), gs.end());

    std::unordered_map<int, int> g_to_a;
    for (int i = 0; i < a; ++i)
    {
        auto vh = dt.nearest_vertex(as[i]);
        int closest_g = vh->info();
        auto found = g_to_a.find(closest_g);
        if (found == g_to_a.end() || zs[g_to_a[closest_g]] > zs[i])
            g_to_a[closest_g] = i;
    }


    //std::cout << "3\n";
    Program lp(CGAL::LARGER, true, 0, true, 24);

    int i = 0;
    for (auto const& mapper : g_to_a)
        lp.set_a(i++, 0, us[mapper.first]);
    lp.set_b(0, u);

    i = 0;
    for (auto const& mapper : g_to_a)
        lp.set_a(i++, 1, vs[mapper.first]);
    lp.set_b(1, v);

    i = 0;
    for (auto const& mapper : g_to_a)
        lp.set_a(i++, 2, ws[mapper.first]);
    lp.set_b(2, w);

    //i = 0;
    //for (auto const& mapper : g_to_a)
    //    lp.set_u(i++, 2, ws[mapper.first]);

    i = 0;
    for (auto const& mapper : g_to_a)
        lp.set_c(i++, zs[mapper.second]);
/*
    for (auto const& mapper : g_to_a)
        std::cout << "G(" << mapper.first << ") -> A(" << mapper.second << ")\n";*/

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    //std::cout << "5\n";

    if (s.is_optimal())
    {/*
        std::cout << s.objective_value().numerator() / s.objective_value().denominator() << std::endl;
        Solution::Variable_value_iterator opt = s.variable_values_begin();
        for (Solution::Variable_value_iterator opt = s.variable_values_begin(); opt != s.variable_values_end(); opt++)
            std::cout << (*opt) << std::endl;*/

        std::cout << ((s.objective_value().numerator() <= z * s.objective_value().denominator()) ? "L" : "H") << std::endl;
    }
    else std::cout << "H" << std::endl;
}

int main()
{
    int T;
    std::cin >> T;
    while (T--) testcase();
    return 0;
}
