#include <iostream>
#include <vector>

void testcase()
{
    int N, K, x;
    std::cin >> N >> K >> x;

    std::vector<int> pattern(K);
    for (int i = K - 1; i >= 0; --i)
    {
        pattern[i] = x & 1;
        x = x >> 1;
    }

    std::vector<int> lights(N);
    for (int i = 0; i < N; ++i) std::cin >> lights[i];

    int pos = 0;
    int neg = 1 << 30;
    for (int i_block = N / K - 1; i_block >= 0; --i_block)
    {
        int pos_diff = 0;
        for (int idx = 0; idx < K; ++idx) pos_diff += lights[K * i_block + idx] != pattern[idx];
        int neg_diff = K - pos_diff;

        int pos_old = pos;
        pos = pos_diff + std::min(pos, neg + 1);
        neg = neg_diff + std::min(neg, pos_old + 1);
    }

    std::cout << std::min(pos, neg) << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
