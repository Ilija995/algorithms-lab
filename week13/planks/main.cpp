#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<int>> partition_sums;

void get_partitions(std::vector<int> &planks, partition_sums &partitions, int side1, int side2, int side3, int side4, int limit, int curr_index)
{
    if (curr_index == (int) planks.size())
    {
        std::vector<int> sides = {side1, side2, side3, side4};
        partitions.emplace_back(sides);
        return;
    }

    if (side1 + planks[curr_index] <= limit) get_partitions(planks, partitions, side1 + planks[curr_index], side2, side3, side4, limit, curr_index + 1);
    if (side2 + planks[curr_index] <= limit) get_partitions(planks, partitions, side1, side2 + planks[curr_index], side3, side4, limit, curr_index + 1);
    if (side3 + planks[curr_index] <= limit) get_partitions(planks, partitions, side1, side2, side3 + planks[curr_index], side4, limit, curr_index + 1);
    if (side4 + planks[curr_index] <= limit) get_partitions(planks, partitions, side1, side2, side3, side4 + planks[curr_index], limit, curr_index + 1);
}

void testcase()
{
    int n;
    std::cin >> n;

    std::vector<int> planks1(n / 2);
    for (int i = 0; i < n / 2; ++i) std::cin >> planks1[i];

    std::vector<int> planks2(n - n / 2);
    for (int i = 0; i < n - n / 2; ++i) std::cin >> planks2[i];

    int sum_planks = std::accumulate(planks1.begin(), planks1.end(), 0) + std::accumulate(planks2.begin(), planks2.end(), 0);
    if (sum_planks % 4 != 0)
    {
        std::cout << "0" << std::endl;
        return;
    }

    int sum_side = sum_planks / 4;

    long long solution = 0L;

    partition_sums partitions1;
    get_partitions(planks1, partitions1, 0, 0, 0, 0, sum_side, 0);

    std::sort(partitions1.begin(), partitions1.end());

    partition_sums partitions2;
    get_partitions(planks2, partitions2, 0, 0, 0, 0, sum_side, 0);

    for (auto sums : partitions2)
    {
        std::vector<int> to_find(4);
        for (int i = 0; i < 4; ++i) to_find[i] = sum_side - sums[i];
        auto it_pair = std::equal_range(partitions1.begin(), partitions1.end(), to_find);
        solution += (long long) std::distance(it_pair.first, it_pair.second);
    }

    std::cout << solution / 24 << std::endl;
}

int main()
{
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
