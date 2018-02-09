#include <iostream>
#include <vector>
#include <unordered_map>

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

void testcase()
{
    int k, n, q;
    std::cin >> k >> n >> q;
    std::vector< std::pair<int, int> > bounds(k);
    for (int i = 0; i < k; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        bounds.push_back(std::pair<int, int>(a, b));
    }
    std::unordered_map< std::vector<int>, int, std::hash< std::vector<int> > > experiments;
    for (int i = 0; i < n; ++i)
    {
        std::vector<int> params(k);
        for (int j = 0; j < k; ++j)
            std::cin >> params[j];
        int r;
        std::cin >> r;
        experiments.emplace(make_pair(params, r));
    }
    for (int i = 0; i < q; ++i)
    {
        std::vector<int> params(k);
        for (int j = 0; j < k; ++j)
            std::cin >> params[j];
        auto found = experiments.find(params);
        if (found != experiments.end())
            std::cout << found->second << std::endl;
        else
            std::cout << "-1" << std::endl;
    }
}

int main()
{
    int T;
    std::cin >> T;
    while (T--) testcase();
    return 0;
}
