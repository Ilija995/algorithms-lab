#include <iostream>
#include <algorithm>
#include <vector>

struct team
{
    long long s;
    int n;
};

void generate_subsets(int len, int base, std::vector<std::vector<int>>& subsets, std::vector<int>& curr_vec, int curr_index)
{
    if (curr_index == len)
    {
        std::vector<int> new_(curr_vec);
        subsets.push_back(new_);
    }
    else
    {
        for (int i = 0; i < base; ++i)
        {
            curr_vec[curr_index] = i;
            generate_subsets(len, base, subsets, curr_vec, curr_index + 1);
        }
    }
}

int T;

void testcase()
{
    bool DEBUG = false;

    int n, k;
    std::cin >> n >> k;
    std::vector<long long> a;
    a.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        a.emplace_back();
        std::cin >> a.back();
    }


    std::vector<std::vector<int>> subsets;
    std::vector<int> empty_vec(n / 2);
    generate_subsets(n / 2, 3, subsets, empty_vec, 0);

if (DEBUG) {
    for (auto vec : subsets)
    {
        for (auto item : vec) std::cout << item << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;}

    std::vector<std::pair<team, team>> l1;
    for (auto subset : subsets)
    {
        team r, b;
        r.s = 0; r.n = 0;
        b.s = 0; b.n = 0;

        for (int i = 0; i < n / 2; ++i)
        {
            if (subset[i] == 1)
            {
                r.s += a[i];
                r.n++;
            }
            else if (subset[i] == 2)
            {
                b.s += a[i];
                b.n++;
            }
        }
        if (DEBUG)
        {
            for (int i = 0; i < n / 2; ++i) std::cout << subset[i] << " ";
            std::cout << ":\t" << r.s << " [" << r.n << "] ; " << b.s << " [" << b.n << "]" << " ; " << r.s - b.s << std::endl;
        }
        l1.push_back(std::make_pair(r, b));
    }

    auto comparator = [n, k](const std::pair<team, team>& lhs, const std::pair<team, team>& rhs)
    {
        return (lhs.first.s - lhs.second.s == rhs.first.s - rhs.second.s) ? lhs.first.n + lhs.second.n < rhs.first.n + rhs.second.n : lhs.first.s - lhs.second.s < rhs.first.s - rhs.second.s;
    };
    std::sort(l1.begin(), l1.end(), comparator);

    subsets.clear();
    empty_vec.clear();
    empty_vec.resize(n - n / 2);
    generate_subsets(n - n / 2, 3, subsets, empty_vec, 0);

    //std::cout << " >>>>>>>>>>>>>>> " << subsets.size() << std::endl;

    long long result = 0;
    for (auto subset : subsets)
    {
        team r, b;
        r.s = 0; r.n = 0;
        b.s = 0; b.n = 0;

        for (int i = 0; i < n - n / 2; ++i)
        {
            if (subset[i] == 1)
            {
                r.s += a[i + n / 2];
                r.n++;
            }
            else if (subset[i] == 2)
            {
                b.s += a[i + n / 2];
                b.n++;
            }
        }
/*
        auto it_pair = std::equal_range(l1.begin(), l1.end(), std::make_pair(b, r), comparator);

        for (auto it = it_pair.first; it != it_pair.second; ++it)
        {
            if (n - it->first.n - it->second.n - r.n - b.n <= k) result++;
        }*/

        auto it_start = std::lower_bound(l1.begin(), l1.end(), std::make_pair(b, r), [n, k](const std::pair<team, team>& lhs, const std::pair<team, team>& rhs)
        {
            return (lhs.first.s - lhs.second.s == rhs.first.s - rhs.second.s) ? lhs.first.n + lhs.second.n + rhs.first.n + rhs.second.n >= n - k : lhs.first.s - lhs.second.s < rhs.first.s - rhs.second.s;
        });
    }

    std::cout << result << std::endl;
}

int main()
{
    std::cin >> T;
    while (T--) testcase();
    return 0;
}
