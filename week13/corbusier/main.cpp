#include <iostream>
#include <vector>

void testcase()
{
    int n, i, k;
    std::cin >> n >> i >> k;

    std::vector<int> heights(n);
    for (int index = 0; index < n; ++index) std::cin >> heights[index];

    int sol_mod = i % k;
    std::vector<bool> prev_sol(k);
    std::vector<bool> curr_sol(k);
    for (int index = 0; index < k; ++index)
    {
        prev_sol[index] = false;
        curr_sol[index] = false;
    }

    for (int heights_index = 0; heights_index < n; ++heights_index)
    {
        for (int k_index = 0; k_index < k; ++k_index)
        {
            curr_sol[k_index] = prev_sol[k_index];
            //if (curr_sol[k_index]) continue;

            int curr_mod = heights[heights_index] % k;
            curr_sol[k_index] = curr_sol[k_index] || (curr_mod == k_index);

            int prev_mod = k_index - curr_mod;
            if (prev_mod < 0) prev_mod += k;

            curr_sol[k_index] = curr_sol[k_index] || prev_sol[prev_mod];
            //std::cout << ((curr_sol[k_index]) ? "+" : "-");
        }
        //std::cout << std::endl;

        prev_sol = curr_sol;
        curr_sol.clear();
        curr_sol.resize(k);
    }

    std::cout << ((prev_sol[sol_mod]) ? "yes" : "no") << std::endl;
}

int main()
{
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
