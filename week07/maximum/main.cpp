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

void solve_problem_1(int a, int b)
{
    Program qp (CGAL::SMALLER, true, 0, false, 0);

    // now set the non-default entries
    const int X = 0;
    const int Y = 1;

    qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, 4);       // x + y <= 4
    qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_b(1, a * b);   // 4x + 2y <= ab
    qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, 1);      // -x + y <= 1

    // objective function: min a * x^2 - b * y
    qp.set_d(X, X, 2 * a);
    qp.set_d(Y, X, 0);
    qp.set_d(Y, Y, 0);
    qp.set_c(Y, -b);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_quadratic_program(qp, ET());
    assert (s.solves_linear_program(qp));

    if (s.is_optimal())
        std::cout << -1 * s.objective_value().numerator() / s.objective_value().denominator() << std::endl;
    else
        std::cout << "no" << std::endl;

}

void solve_problem_2(int a, int b)
{
    Program qp (CGAL::LARGER, false, 0, false, 0);

    // now set the non-default entries
    const int X = 0;
    const int Y = 1;
    const int Z2 = 2;

    qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_a(Z2, 0, 0); qp.set_b(0, -4);       // x + y >= -4
    qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_a(Z2, 1, 1); qp.set_b(1, -1 * a * b);   // 4x + 2y + z^2 >= -ab
    qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_a(Z2, 2, 0); qp.set_b(2, -1);      // -x + y >= -1

    // objective function: min a * x^2 + b * y + z^4
    qp.set_d(X, X, 2 * a);
    qp.set_d(Y, X, 0);
    qp.set_d(Z2, X, 0);
    qp.set_d(Y, Y, 0);
    qp.set_d(Z2, Y, 0);
    qp.set_d(Z2, Z2, 2);
    qp.set_c(Y, b);

    qp.set_u(X, true, 0);
    qp.set_u(Y, true, 0);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_quadratic_program(qp, ET());
    assert (s.solves_linear_program(qp));

    if (s.is_optimal())
        std::cout << s.objective_value().numerator() / s.objective_value().denominator() << std::endl;
    else
        std::cout << "no" << std::endl;

}

int main()
{
    int p, a, b;
    std::cin >> p;
    while (p != 0)
    {
        std::cin >> a >> b;
        if (p == 1) solve_problem_1(a, b);
        else solve_problem_2(a, b);

        std::cin >> p;
    }

    return 0;
}
