#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

bool solve_goldfinger(int max_intensity,
                      std::vector< std::pair<K::Point_2, int> > *sensors_ptr,
                      std::vector<K::Point_2> *mpes_ptr,
                      std::vector<ET> *mpe_sq_ranges_ptr,
                      int last_mpe)
{
    //std::vector< std::pair<K::Point_2, int> > sensors = *sensors_ptr;
    //std::vector<K::Point_2> mpes = *mpes_ptr;
    //std::vector<ET> mpe_sq_ranges = *mpe_sq_ranges_ptr;

    Program lp (CGAL::LARGER, true, 0, false, 0);

    for (int i_sensor = 0; i_sensor < (*sensors_ptr).size(); ++i_sensor)
    {
        for (int i_mpe = 0; i_mpe <= last_mpe; ++i_mpe)
        {
            auto sq_distance = CGAL::squared_distance((*sensors_ptr)[i_sensor].first, (*mpes_ptr)[i_mpe]);
            if (sq_distance < (*mpe_sq_ranges_ptr)[i_mpe]) lp.set_a(i_mpe, i_sensor, ET(1) / sq_distance);
            else lp.set_a(i_mpe, i_sensor, ET(0));
        }
        lp.set_b(i_sensor, ET((*sensors_ptr)[i_sensor].second));
    }

    for (int i_mpe = 0; i_mpe <= last_mpe; ++i_mpe) lp.set_c(i_mpe, ET(1));

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert (s.solves_nonnegative_linear_program(lp));
/*
    if (s.is_optimal())
    {
        std::cout << s.objective_value().numerator() / s.objective_value().denominator() << " <= " << max_intensity << std::endl;
    }
    else std::cout << "Not optimal" << std::endl;*/
    return s.is_optimal() && s.objective_value() <= max_intensity;
}

void testcase()
{
    int n_sensors, n_mpes, n_henchmen, max_intensity;
    std::cin >> n_sensors >> n_mpes >> n_henchmen >> max_intensity;

    std::vector< std::pair<K::Point_2, int> > sensors;
    sensors.reserve(n_sensors);
    K::Point_2 position;
    int val;
    for (int i_sensor = 0; i_sensor < n_sensors; i_sensor++)
    {
        std::cin >> position >> val;
        sensors.emplace_back(std::make_pair(position, val));
    }

    std::vector<K::Point_2> mpes;
    mpes.reserve(n_mpes);
    for (int i_mpe = 0; i_mpe < n_mpes; ++i_mpe)
    {
        mpes.emplace_back();
        std::cin >> mpes.back();
    }

    std::vector<K::Point_2> henchmen;
    henchmen.reserve(n_henchmen);
    for (int i_henchman = 0; i_henchman < n_henchmen; ++i_henchman)
    {
        henchmen.emplace_back();
        std::cin >> henchmen.back();
    }

    //std::cout << "Loaded parameters" << std::endl;

    Triangulation t;
    t.insert(henchmen.begin(), henchmen.end());

    std::vector<ET> mpe_sq_ranges;
    mpe_sq_ranges.reserve(n_mpes);
    for (int i_mpe = 0; i_mpe < n_mpes; ++i_mpe)
    {
        if (n_henchmen == 0)
        {
            mpe_sq_ranges.emplace_back(ET(INT_MAX));
            continue;
        }
        Triangulation::Vertex_handle closest_henchman_handler = t.nearest_vertex(mpes[i_mpe]);
        mpe_sq_ranges.emplace_back(CGAL::squared_distance(mpes[i_mpe], closest_henchman_handler->point()));
    }

    //std::cout << "Computed ranges" << std::endl;

    if (solve_goldfinger(max_intensity, &sensors, &mpes, &mpe_sq_ranges, 0))
    {
        std::cout << 1 << std::endl;
        return;
    }

    int left = 0;
    int right = std::min(20, n_mpes - 1);
    while (right - left > 1)
    {
        int mid = (right + left) / 2;
        //std::cout << "Try for first " << mid + 1 << " MPEs" << std::endl;
        if (solve_goldfinger(max_intensity, &sensors, &mpes, &mpe_sq_ranges, mid))
        {
            right = mid;
        }
        else
        {
            left = mid;
        }
    }

    if (right == n_mpes - 1 && !solve_goldfinger(max_intensity, &sensors, &mpes, &mpe_sq_ranges, right))
    {
        std::cout << "impossible" << std::endl;
    }
    else
    {
        std::cout << right + 1 << std::endl;
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
