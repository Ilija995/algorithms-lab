#include <iostream>
#include <unordered_set>

int main()
{
    std::ios_base::sync_with_stdio(false);
    size_t t;
    std::cin >> t;
    for (size_t test_case = 0; test_case < t; ++test_case)
    {
        size_t n, k;
        std::unordered_set<size_t> less_union, less_inter, greater_union, greater_inter, valid_coins;
        std::cin >> n >> k;

        size_t p, *fst = new size_t[n / 2], *snd = new size_t[n / 2];
        char sgn;
        bool had_less = false;
        for (size_t i = 0; i < k; ++i)
        {
            std::cin >> p;
            for (size_t j = 0; j < p; ++j)
                std::cin >> fst[j];
            for (size_t j = 0; j < p; ++j)
                std::cin >> snd[j];
            std::cin >> sgn;

            if (sgn == '=')
            {
                for (size_t j = 0; j < p; ++j)
                {
                    valid_coins.insert(fst[j]);
                    valid_coins.insert(snd[j]);
                }
                continue;
            }

            if (sgn == '>')
            {
                size_t *tmp;
                tmp = fst;
                fst = snd;
                snd = tmp;
            }

            for (size_t j = 0; j < p; ++j)
            {
                less_union.insert(fst[j]);
                greater_union.insert(snd[j]);
            }

            if (!had_less)
            {
                had_less = true;
                for (size_t j = 0; j < p; ++j)
                {
                    less_inter.insert(fst[j]);
                    greater_inter.insert(snd[j]);
                }
            }
            else
            {
                std::unordered_set<size_t> tmp_less, tmp_greater;
                for (size_t j = 0; j < p; ++j)
                {
                    if (less_inter.find(fst[j]) != less_inter.end())
                        tmp_less.insert(fst[j]);
                    if (greater_inter.find(snd[j]) != greater_inter.end())
                        tmp_greater.insert(snd[j]);
                }
                less_inter = tmp_less;
                greater_inter = tmp_greater;
            }
        }

        if (valid_coins.size() == n - 1)
        {
            size_t sum = 0;
            for (const size_t& valid : valid_coins) sum += valid;
            std::cout << ((n + 1) * n ) / 2 - sum << std::endl;
            continue;
        }

        for (const size_t& valid : valid_coins)
        {
            less_inter.erase(valid);
            less_union.erase(valid);
            greater_inter.erase(valid);
            greater_union.erase(valid);
        }
        std::unordered_set<size_t>::iterator it;
        for (it = less_inter.begin(); it != less_inter.end();)
            if (greater_union.find(*it) != greater_union.end())
                it = less_inter.erase(it);
            else
                ++it;
        for (it = greater_inter.begin(); it != greater_inter.end();)
            if (less_union.find(*it) != less_union.end())
                it = greater_inter.erase(it);
            else
                ++it;

        if (less_inter.size() + greater_inter.size() != 1)
            std::cout << "0\n";
        else
        {
            if (less_inter.size() == 1)
                std::cout << *(less_inter.begin()) << std::endl;
            else
                std::cout << *(greater_inter.begin()) << std::endl;
        }
    }
    return 0;
}
