#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

template <class T>
inline void hash_combine(std::size_t& seed, T const& v)
{
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
	template<typename T>
	struct hash { };
    template<typename T>
    struct hash<std::vector<T>>
    {
        typedef std::vector<T> argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& in) const
        {
            size_t size = in.size();
            result_type seed = 0;
            for (size_t i = 0; i < size; i++)
                //Combine the hash of the current vector with the hashes of the previous ones
                hash_combine(seed, in[i]);
            return seed;
        }
    };
}

std::unordered_map< std::vector< std::vector<int> >, int, std::hash< std::vector< std::vector<int> > > > memo;

int solve(std::vector< std::vector<int> > curr_state, int depth)
{
    if (memo.find(curr_state) != memo.end())
        return memo[curr_state];

    int no_stacks = curr_state.size();
    std::unordered_set<int> popped;
    std::multimap<int, int, std::greater<int>> stacks_by_size;
    for (int i = 0; i < no_stacks; ++i)
        stacks_by_size.insert( std::pair<int, int>(curr_state[i].size(), (curr_state[i].size() != 0) ? *curr_state[i].rbegin() : -1));

    int max_solution = 0;
    for (auto const& item : stacks_by_size)
    {
        int val_to_pop = item.second;
        if ((popped.find(val_to_pop) != popped.end()) || val_to_pop == -1) continue;
        popped.insert(val_to_pop);

        std::vector<int> stacks_to_pop;
        for (int i = 0; i < no_stacks; ++i)
        {
            if (curr_state[i].size() != 0 && *curr_state[i].rbegin() == val_to_pop)
                stacks_to_pop.push_back(i);
        }

        for (auto const& index : stacks_to_pop)
            curr_state[index].pop_back();

        int solution = solve(curr_state, depth + 1);

        for (auto const& index : stacks_to_pop)
            curr_state[index].push_back(val_to_pop);

        max_solution = std::max(max_solution, solution);
    }

    memo[curr_state] = max_solution;
    return max_solution;
}

void testcase()
{
    memo.clear();

    int n;
    std::cin >> n;

    std::vector<int> stack_sizes(n);
    for (int i = 0; i < n; ++i)
        std::cin >> stack_sizes[i];

    std::vector< std::vector<int> > start_state(n);
    for (int i = 0; i < n; ++i)
    {
        start_state[i].resize(stack_sizes[i]);
        for (int j = 0; j < stack_sizes[i]; ++j)
            std::cin >> start_state[i][j];
    }

    std::cout << solve(start_state, 0) << std::endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);
    int T;
    std::cin >> T;
    while (T--) testcase();
    return 0;
}
