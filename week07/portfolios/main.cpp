#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose exact integral type
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
/*
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

}*/

bool possible_portfolio(std::vector<int> *cost,
                        std::vector<int> *ret,
                        std::vector< std::vector<int> > *var,
                        int cost_bound,
                        int ret_bound,
                        int var_bound)
{
    int n = cost->size();
    std::vector<int> &cost_ref = *cost;
    std::vector<int> &ret_ref = *ret;
    std::vector< std::vector<int> > &var_ref = *var;
    Program qp (CGAL::SMALLER, true, 0, false, 0);

    // set cost upper bound
    for (int i = 0; i < n; ++i)
    {
        //std::cout << "alpha_" << i << " * " << cost_ref[i];
        //if (i != n-1)
         //   std::cout << " + ";
        qp.set_a(i, 0, cost_ref[i]);
    }
    //std::cout << " <= " << cost_bound << std::endl;
    qp.set_b(0, cost_bound);

    // set return lower bound
    for (int i = 0; i < n; ++i)
    {
        //std::cout << "alpha_" << i << " * " << cost_ref[i];
        //if (i != n-1)
         //   std::cout << " + ";
        qp.set_a(i, 1, -ret_ref[i]);
    }
    qp.set_b(1, -ret_bound);

    // set objective function to calculate variance
    for (int i = 0; i < n; ++i)
        for (int j = i; j < n; ++j)
            qp.set_d(j, i, 2 * var_ref[i][j]);

    // solve the program, using ET as the exact type
    assert(qp.is_nonnegative());
    Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
    assert (s.solves_quadratic_program(qp));
/*
    if (s.is_optimal())
    {
        std::cout << ">> " << s.objective_value().numerator() / s.objective_value().denominator() << " <= " << var_bound << std::endl;
        int cnt = 0;
        for (Solution::Variable_value_iterator opt = s.variable_values_begin(); opt != s.variable_values_end(); opt++)
        {
            ET alpha_ratio = opt->numerator() / opt->denominator();
            std::cout << "alpha_" << cnt++ << " = " << alpha_ratio << " = " << opt->numerator() << " / " << opt->denominator() << std::endl;
        }
    }*/
    return s.is_optimal() && s.objective_value().numerator() / s.objective_value().denominator() <= var_bound;
}

void testcase(int n, int m)
{
    std::vector<int> cost(n);
    std::vector<int> ret(n);
    std::vector< std::vector<int> > var(n);

    for (int i = 0; i < n; ++i)
        std::cin >> cost[i] >> ret[i];

    for (int i = 0; i < n; ++i)
    {
        var[i].resize(n);
        for (int j = 0; j < n; ++j)
            std::cin >> var[i][j];
    }

    int cost_bound, ret_bound, var_bound;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> cost_bound >> ret_bound >> var_bound;
        std::cout << ((possible_portfolio(&cost, &ret, &var, cost_bound, ret_bound, var_bound)) ? "Yes." : "No.") << std::endl;
    }
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
