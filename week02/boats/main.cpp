#include <iostream>
#include <vector>
#include <algorithm>

void testcase()
{
    int N;
    std::cin >> N;

    std::vector< std::pair<int, int> > boats;
    boats.reserve(N);
    int length, position;
    for (int i = 0; i < N; ++i)
    {
        std::cin >> length >> position;
        boats.emplace_back(std::make_pair(length, position));
    }

    std::sort(boats.begin(), boats.end(), [](const std::pair<int, int> &lhs, const std::pair<int, int> &rhs) { return lhs.second < rhs.second; });

    int curr_end = boats[0].second;
    int last_boat = 0;
    int no_boats = 1;

    for (int i = 1; i < N; ++i)
    {
        if (curr_end <= boats[i].second)
        {
            curr_end = std::max(curr_end + boats[i].first, boats[i].second);
            ++no_boats;
            last_boat = i;
        }
        else
        {
            int prev_end = curr_end - boats[last_boat].first;
            int potential_end = std::max(prev_end + boats[i].first, boats[i].second);
            if (potential_end < curr_end)
            {
                curr_end = potential_end;
                last_boat = i;
            }
        }
    }

    std::cout << no_boats << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
