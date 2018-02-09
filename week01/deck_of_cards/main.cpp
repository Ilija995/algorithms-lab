#include <iostream>
#include <cstdlib>

int main()
{
    size_t t;
    std::cin >> t;
    for (size_t test_case = 0; test_case < t; ++test_case)
    {
        size_t n, k, *v;
        std::cin >> n >> k;
        v = new size_t[n];
        for (size_t i = 0; i < n; ++i) std::cin >> v[i];

        size_t i, j, res_i, res_j;
        i = j = res_i = res_j = 0;
        size_t sum, best_sum;
        sum = best_sum = v[0];
        while (j < n)
        {
            if (sum < k) sum += v[++j];
            else if (sum > k) sum -=  v[i++];
            else break;
            if (i > j) sum += v[++j];

            if (abs(k - sum) < abs(k - best_sum))
            {
                res_i = i;
                res_j = j;
                best_sum = sum;
            }
        }

        std::cout << res_i << " " << res_j << std::endl;
    }
    return 0;
}
