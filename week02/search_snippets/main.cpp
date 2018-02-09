#include <iostream>
#include <vector>
#include <queue>

struct word_pos
{
    int pos;
    int i;
    int j;
};

class ComparatorLess
{
public:
    bool operator() (word_pos lhs, word_pos rhs)
    {
        return lhs.pos < rhs.pos;
    }
};

class ComparatorGreather
{
public:
    bool operator() (word_pos lhs, word_pos rhs)
    {
        return lhs.pos > rhs.pos;
    }
};

void testcase()
{
    int N;
    std::cin >> N;

    std::vector<int> Ms(N);
    for (int i = 0; i < N; ++i) std::cin >> Ms[i];

    std::vector< std::vector<int> > positions(N);
    for (int i = 0; i < N; ++i)
    {
        positions[i].reserve(Ms[i]);
        for (int j = 0; j < Ms[i]; ++j)
        {
            positions[i].emplace_back();
            std::cin >> positions[i].back();
        }
    }

    std::priority_queue<word_pos, std::vector<word_pos>, ComparatorGreather> smallest;
    std::priority_queue<word_pos, std::vector<word_pos>, ComparatorLess> largest;

    for (int i = 0; i < N; ++i)
    {
        word_pos wp;
        wp.i = i;
        wp.j = 0;
        wp.pos = positions[i][0];

        smallest.push(wp);
        largest.push(wp);
    }

    int min_snippet = largest.top().pos - smallest.top().pos + 1;

    while (true)
    {
        word_pos curr_smallest = smallest.top();
        smallest.pop();

        if (curr_smallest.j == positions[curr_smallest.i].size() - 1) break;

        word_pos new_wp;
        new_wp.i = curr_smallest.i;
        new_wp.j = curr_smallest.j + 1;
        new_wp.pos = positions[new_wp.i][new_wp.j];

        smallest.push(new_wp);
        largest.push(new_wp);

        int curr_snippet = largest.top().pos- smallest.top().pos + 1;
        min_snippet = std::min(min_snippet, curr_snippet);
    }

    std::cout << min_snippet << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
