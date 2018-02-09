#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

void testcase()
{
    int h, w;
    std::cin >> h >> w;

    std::vector<std::vector<int>> floor_plan(h, std::vector<int>(w));
    std::vector<int> floor_bits(h);
    for (int i = 0; i < h; ++i)
    {
        floor_bits[i] = 0;
        for (int j = 0; j < w; ++j)
        {
            std::cin >> floor_plan[i][j];
            floor_bits[i] += floor_plan[i][j] * (1 << j);
        }
    }

    std::vector<int> num_pairs(1 << w, 0);
    std::vector<std::vector<int>> last_bitmasks(1 << w);
    for (int bitmask = 0; bitmask < 1 << w; ++bitmask)
    {
        for (int i = 1; i < w; ++i)
        {
            if ((bitmask & (1 << i)) && (bitmask & (1 << (i - 1))))
            {
                num_pairs[bitmask]++;
                i++;
            }
        }

        for (int i = 0; i < w; ++i)
        {
            if (bitmask & (1 << i))
            {
                last_bitmasks[bitmask].push_back(bitmask - (1 << i));
            }
        }
    }

    std::vector<int> dp(1 << w, 0);
    for (int i = 1; i < h; ++i)
    {
        std::vector<int> ndp(1 << w, 0);
        ndp[0] = dp[(1 << w) - 1];
        for (int bitmask = 1; bitmask < 1 << w; ++bitmask)
        {

            for (int last_bitmask : last_bitmasks[bitmask]) ndp[bitmask] = std::max(ndp[bitmask], ndp[last_bitmask]);
            ndp[bitmask] = std::max(ndp[bitmask], dp[(1 << w) - 1 - bitmask] + num_pairs[floor_bits[i] & floor_bits[i - 1] & bitmask]);
        }
        dp = ndp;
    }

    std::cout << dp[(1 << w) - 1] << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
