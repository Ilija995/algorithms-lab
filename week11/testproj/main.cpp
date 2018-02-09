#include <iostream>
#include <vector>

void generate_subsets(int len, int base, std::vector<std::vector<int>>& subsets, std::vector<int> curr_vec)
{
    if (curr_vec.size() == len) subsets.push_back(curr_vec);
    else
    {
        curr_vec.emplace_back();
        for (int i = 0; i < base; ++i)
        {
            curr_vec[curr_vec.size() - 1] = i;
            generate_subsets(len, base, subsets, curr_vec);
        }
    }
}

int main()
{
    std::vector<std::vector<int>> subsets;
    std::vector<int> empty_vec;
    generate_subsets(11, 3, subsets, empty_vec);

    for (auto vec : subsets)
    {
        for (auto item : vec) std::cout << item << " ";
        std::cout << std::endl;
    }
    return 0;
}
