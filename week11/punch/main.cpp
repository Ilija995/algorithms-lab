#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

const int MAX_COST = 200000000;

void testcase()
{
    //std::cout << "\n----------------------\n";

    int n_drinks, n_people;
    std::cin >> n_drinks >> n_people;

    //std::cout << n_drinks << " " << n_people << std::endl;

    std::vector<int> costs(n_drinks);
    std::vector<int> volumes(n_drinks);
    for (int i = 0; i < n_drinks; ++i) std::cin >> costs[i] >> volumes[i];

    auto max_el_it = std::max_element(std::begin(volumes), std::end(volumes));

    int d_size = n_people + *max_el_it;
    std::vector<int> d_cost(d_size, MAX_COST);
    std::vector<int> d_drinks(d_size, 0);
    for (int i = 0; i < n_drinks; ++i)
    {
        //std::cout << ">> " << i << std::endl;

        std::vector<int> new_d_cost(d_size, 0);
        std::vector<int> new_d_drinks(d_size, 0);
        std::vector<bool> bought(d_size, false);
        for (int j = 1; j < d_size; ++j)
        {
            if (j < volumes[i])
            {
                new_d_cost[j] = d_cost[j];
                new_d_drinks[j] = d_drinks[j];
            }
            else
            {
                if (new_d_cost[j - volumes[i]] + costs[i] < d_cost[j] || (new_d_cost[j - volumes[i]] + costs[i] == d_cost[j] && new_d_drinks[j - volumes[i]] + !bought[j - volumes[i]] > d_drinks[j]))
                {
                    new_d_cost[j] = new_d_cost[j - volumes[i]] + costs[i];
                    new_d_drinks[j] = new_d_drinks[j - volumes[i]] + !bought[j - volumes[i]];
                    bought[j] = true;
                }
                else
                {
                    new_d_cost[j] = d_cost[j];
                    new_d_drinks[j] = d_drinks[j];
                }
            }
        }
        d_cost = new_d_cost;
        d_drinks = new_d_drinks;

        //for(int i = 0; i < d_size; ++i) std::cout << d_cost[i] << " ";
        //std::cout << std::endl;
        //for(int i = 0; i < d_size; ++i) std::cout << d_drinks[i] << " ";
        //std::cout << std::endl;
    }

    int min_cost = MAX_COST;
    int max_drinks = 0;
    for (int i = n_people; i < d_size; ++i)
    {
        if (d_cost[i] < min_cost || (d_cost[i] == min_cost && d_drinks[i] > max_drinks))
        {
            min_cost = d_cost[i];
            max_drinks = d_drinks[i];
        }
    }

    std::cout << min_cost << " " << max_drinks << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
