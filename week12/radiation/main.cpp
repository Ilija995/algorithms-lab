#include <iostream>
#include <vector>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const int max_power = 30;

void testcase()
{
    int n_healthy, n_tumor;
    std::cin >> n_healthy >> n_tumor;
/*
    std::vector<ET> hx(n_healthy);
    std::vector<ET> hy(n_healthy);
    std::vector<ET> hz(n_healthy);
    for (int i = 0; i < n_healthy; ++i)
    {
        std::cin >> hx[i] >> hy[i] >> hz[i];
    }

    std::vector<ET> tx(n_tumor);
    std::vector<ET> ty(n_tumor);
    std::vector<ET> tz(n_tumor);
    for (int i = 0; i < n_tumor; ++i)
    {
        std::cin >> tx[i] >> ty[i] >> tz[i];
    }

    std::vector< std::vector< std::vector<ET> > > h_poly(n_healthy);
    for (int i_sample = 0; i_sample < n_healthy; ++i_sample)
    {
        h_poly[i_sample].resize(max_power + 1);
        h_poly[i_sample][0].emplace_back(1);
        for (int power = 1; power <= max_power; ++power)
        {
            for (int i_param = 0; i_param < h_poly[i_sample][power - 1].size(); ++i_param)
            {
                h_poly[i_sample][power].emplace_back(hx[i_sample] * h_poly[i_sample][power - 1][i_param]);
                h_poly[i_sample][power].emplace_back(hy[i_sample] * h_poly[i_sample][power - 1][i_param]);
                h_poly[i_sample][power].emplace_back(hz[i_sample] * h_poly[i_sample][power - 1][i_param]);
            }
        }
    }

    std::vector< std::vector< std::vector<ET> > > t_poly(n_tumor);
    for (int i_sample = 0; i_sample < n_tumor; ++i_sample)
    {
        t_poly[i_sample].resize(max_power + 1);
        t_poly[i_sample][0].emplace_back(1);
        for (int power = 1; power <= max_power; ++power)
        {
            for (int i_param = 0; i_param < t_poly[i_sample][power - 1].size(); ++i_param)
            {
                t_poly[i_sample][power].emplace_back(tx[i_sample] * t_poly[i_sample][power - 1][i_param]);
                t_poly[i_sample][power].emplace_back(ty[i_sample] * t_poly[i_sample][power - 1][i_param]);
                t_poly[i_sample][power].emplace_back(tz[i_sample] * t_poly[i_sample][power - 1][i_param]);
            }
        }
    }*/


    std::vector<ET> x(n_healthy + n_tumor);
    std::vector<ET> y(n_healthy + n_tumor);
    std::vector<ET> z(n_healthy + n_tumor);
    for (int i = 0; i < n_healthy + n_tumor; ++i)
    {
        std::cin >> x[i] >> y[i] >> z[i];
    }

    std::vector< std::vector< std::vector<ET> > > poly(n_healthy + n_tumor);
    for (int i_sample = 0; i_sample < n_healthy + n_tumor; ++i_sample)
    {
        poly[i_sample].resize(max_power + 1);
        poly[i_sample][0].emplace_back(1);
        for (int power = 1; power <= max_power; ++power)
        {
            for (int i_param = 0; i_param < poly[i_sample][power - 1].size(); ++i_param)
            {
                poly[i_sample][power].emplace_back(x[i_sample] * poly[i_sample][power - 1][i_param]);
                poly[i_sample][power].emplace_back(y[i_sample] * poly[i_sample][power - 1][i_param]);
                poly[i_sample][power].emplace_back(z[i_sample] * poly[i_sample][power - 1][i_param]);
            }
        }
    }

    std::cout << "Processed parameters\n";

    int l = 0, r = max_power + 1;
    bool solution_found = false;
    while (r - l > 1)
    {
        int m = (l + r) / 2;

        Program lp (CGAL::SMALLER, false, 0, false, 0);

        int n_variables;
        for (int i_sample = 0; i_sample < n_healthy + n_tumor; ++i_sample)
        {
            int i_var = 0;
            ET y = (i_sample < n_healthy) ? ET(1) : ET(-1);
            for (int power = 0; power <= m; ++power)
            {
                for (int i_param = 0; i_param < poly[i_sample][power].size(); ++i_param)
                {
                    lp.set_a(i_var++, i_sample, y * poly[i_sample][power][i_param]);
                }
            }
            n_variables = i_var;
            lp.set_b(i_sample, y);
        }

        for (int i_var = 0; i_var < n_variables; ++i_var) lp.set_c(i_var, 1);

        Solution s = CGAL::solve_linear_program(lp, ET());
        assert (s.solves_linear_program(lp));

        solution_found = s.is_optimal();
        if (solution_found) r = m;
        else l = m;
    }

    if (l == max_power && !solution_found) std::cout << "Impossible!" << std::endl;
    else std::cout << r << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
