#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

void testcase()
{
    //std::cout << "-------------------";
    int a, s, b;
    std::cin >> a >> s >> b;
    int e;
    std::cin >> e;
    std::vector< std::pair<K::Point_2, int> > asteroid_particles;
    std::vector<K::Point_2> shooting_points;
    std::vector<K::Point_2> bounty_hunters;

    //std::cout << "Input : " << a << " " << s << " " << b << std::endl;

    asteroid_particles.reserve(a);
    shooting_points.reserve(s);
    if (b > 0)
        bounty_hunters.reserve(b);

    K::Point_2 p;
    int d;
    //int counter = 0;
    for (int i = 0; i < a; ++i)
    {
        std::cin >> p >> d;
        asteroid_particles.emplace_back(std::make_pair(p, d));
        //++counter;
    }
    //std::cout << "a counter " << counter << std::endl;

    //counter = 0;
    for (int i = 0; i < s; ++i)
    {
        shooting_points.emplace_back();
        std::cin >> shooting_points.back();
        //++counter;
    }
    //std::cout << "s counter " << counter << std::endl;

    //counter = 0;
    //std::cout<< "B: " << b << std::endl;
    for (int i = 0; i < b; ++i)
    {
        //std::cout<< "B: " << b << std::endl;
        bounty_hunters.emplace_back();
        std::cin >> bounty_hunters.back();
        //++counter;
    }
    //std::cout << "b counter " << counter << std::endl;

    std::vector<K::FT> r_sq;
    r_sq.reserve(s);

    if (b > 0)
    {
        Triangulation t;
        t.insert(bounty_hunters.begin(), bounty_hunters.end());

        for (int i = 0; i < s; ++i)
        {
            auto vh = t.nearest_vertex(shooting_points[i]);
            r_sq.emplace_back(CGAL::squared_distance(shooting_points[i], vh->point()));
        }
    }
    else
    {
        for (int i = 0; i < s; ++i)
        {
            r_sq.emplace_back(CGAL::square(2000000000L));
        }
    }


    //std::cout << "r_sq[0] == " << r_sq[0] << std::endl;
    Program lp (CGAL::LARGER, true, 0, false, 0);
    // particle_index == equation_index
    for (int particle_index = 0; particle_index < a; ++particle_index)
    {
        // shooting_pint_index == variable_index
        for (int shooting_point_index = 0; shooting_point_index < s; ++shooting_point_index)
        {
            K::FT d_sq = CGAL::squared_distance(asteroid_particles[particle_index].first, shooting_points[shooting_point_index]);
            //std::cout << "From particle " << particle_index << " (" << asteroid_particles[particle_index].first << ") to point " << shooting_point_index << " (" << shooting_points[shooting_point_index] << ") squared distance : " << d_sq << std::endl;
            if (d_sq < r_sq[shooting_point_index])
            {
                //std::cout << "1" ;
                lp.set_a(shooting_point_index, particle_index, ET(1) / CGAL::max(ET(1), d_sq));
            }
            else
            {
                lp.set_a(shooting_point_index, particle_index, 0);
            }
        }

        lp.set_b(particle_index, asteroid_particles[particle_index].second);
    }


    for (int shooting_point_index = 0; shooting_point_index < s; ++shooting_point_index)
    {
        lp.set_c(shooting_point_index, 1);
    }


    // solve the program, using ET as the exact type
    Solution sol = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert (sol.solves_nonnegative_linear_program(lp));

    // output exposure center and radius, if they exist
    //std::cout << "Sol : " << sol.objective_value().numerator() / sol.objective_value().denominator() << std::endl;
    std::cout << ((sol.is_optimal() && (sol.objective_value() <= e)) ? "y" : "n") << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T;
    std::cin >> T;
    while (T--) testcase();
    return 0;
}
