#include <iostream>
#include <vector>
#include <algorithm>

void testcase()
{
    int n;
    std::cin >> n;
    std::vector<int> parasols;
    parasols.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        parasols.emplace_back();
        std::cin >> parasols.back();
    }

    std::sort(parasols.begin(), parasols.end());

    int max_parasols = 0, min_dist = 1 << 30;
    std::vector<int> solutions;
    int i = 0, j = 1;
    while (j < n)
    {
        while(j < n && parasols[j] - parasols[i] <= 200) ++j;
        if (j - i > max_parasols || (j - i == max_parasols && (parasols[j - 1] - parasols[i] + 1) / 2 < min_dist))
        {
            max_parasols = j - i;
            min_dist = (parasols[j - 1] - parasols[i] + 1) / 2;
            //std::cout << "Updating solution to " << max_parasols << std::endl;
            solutions.clear();
            solutions.emplace_back(parasols[i] + (parasols[j - 1] - parasols[i]) / 2);
            //std::cout << "Adding "
            if ((parasols[j - 1] - parasols[i]) % 2 == 1)
                solutions.emplace_back(solutions.back() + 1);
        }
        else if (j - i == max_parasols && (parasols[j - 1] - parasols[i] + 1) / 2 == min_dist)
        {
            solutions.emplace_back(parasols[i] + (parasols[j - 1] - parasols[i]) / 2);
            if ((parasols[j - 1] - parasols[i]) % 2 == 1)
                solutions.emplace_back(solutions.back() + 1);
        }

        ++i;
        if (i == j) ++j;
    }

    std::cout << max_parasols << " " << min_dist << std::endl;
    std::cout << solutions[0];
    for (int i = 1; i < solutions.size(); ++i)
        std::cout << " " << solutions[i];
    std::cout << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while(T--) testcase();
    return 0;
}
