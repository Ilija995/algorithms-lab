#include <iostream>
#include <vector>
#include <unordered_set>

int T;

long long solve(int k, std::vector<int> *a, int curr_index, int no_red, int sum_red, int no_blue, int sum_blue)
{
    bool DEBUG = 8 - T == 2;
    if (curr_index == a->size())
    {
        if (DEBUG && sum_red == sum_blue && a->size() - no_red - no_blue <= k)
            std::cout << sum_red << " [" << no_red << "] ; " << sum_blue << " [" << no_blue << "]" << std::endl;
        return (sum_red == sum_blue && a->size() - no_red - no_blue <= k) ? 1L : 0L;
    }

    if (curr_index - no_red - no_blue > k)
        return 0L;

    return solve(k, a, curr_index + 1, no_red + 1, sum_red + (*a)[curr_index], no_blue, sum_blue)
            + solve(k, a, curr_index + 1, no_red, sum_red, no_blue + 1, sum_blue + (*a)[curr_index])
            + solve(k, a, curr_index + 1, no_red, sum_red, no_blue, sum_blue);
}

void testcase()
{
    int n, k;
    std::cin >> n >> k;
    std::vector<int> a;
    a.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        a.emplace_back();
        std::cin >> a.back();
    }

    std::unordered_set<int> red;
    std::unordered_set<int> blue;

    std::cout << solve(k, &a, 0, 0, 0, 0, 0) << std::endl;
}

int main()
{
    std::cin >> T;
    while (T--) testcase();
    return 0;
}
