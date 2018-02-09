#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n, int m)
{
    std::vector< std::pair<int, int> > nut_bounds(n);
    std::vector< std::vector<int> > products(m);
    std::vector<int> prices(m);

    int min_bound, max_bound;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> min_bound >> max_bound;
        nut_bounds[i] = std::make_pair(min_bound, max_bound);
    }

    for (int i = 0; i < m; ++i)
    {
        std::cin >> prices[i];
        products[i].resize(n);
        for (int j = 0; j < n; ++j)
            std::cin >> products[i][j];
    }

    Program lp (CGAL::SMALLER, true, 0, false, 0);

    // set upper bounds
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
            lp.set_a(j, i, products[j][i]);
        lp.set_b(i, nut_bounds[i].second);
    }

    // set lower bounds
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
            lp.set_a(j, i + n, -products[j][i]);
        lp.set_b(i + n, -nut_bounds[i].first);
    }

    // set objective function
    for (int i = 0; i < m; ++i)
        lp.set_c(i, prices[i]);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));

    if (s.is_optimal())
        std::cout << s.objective_value().numerator() / s.objective_value().denominator() << std::endl;
    else
        std::cout << "No such diet." << std::endl;

}

int main()
{
    int n, m;
    std::cin >> n >> m;
    while (n != 0 || m != 0)
    {
        testcase(n, m);
        std::cin >> n >> m;
    }
    return 0;
}
