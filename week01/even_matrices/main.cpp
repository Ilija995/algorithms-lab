#include <iostream>
#include <vector>

void testcase()
{
    //std::cout << "-------------------------\n";

    int n;
    std::cin >> n;

    std::vector< std::vector<int> > mat(n);
    for (int i = 0; i < n; ++i)
    {
        mat[i].reserve(n);
        for (int j = 0; j < n; ++j)
        {
            mat[i].emplace_back();
            std::cin >> mat[i].back();
        }
    }

    std::vector< std::vector<int> > cum_sum(n + 1);
    for (int i = 0; i < n + 1; ++i) cum_sum[i].resize(n);
    for (int j = 0; j < n; ++j) cum_sum[0][j] = 0;
    for (int j = 0; j < n; ++j)
    {
        for (int i = 1; i < n + 1; ++i)
        {
            cum_sum[i][j] = cum_sum[i - 1][j] + mat[i - 1][j];
        }
    }
/*
    for (int i = 0; i < n + 1; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::cout << cum_sum[i][j] << " ";
        }
        std::cout << std::endl;
    }
*/
    long long solution = 0L;
    for (int i2 = 1; i2 < n + 1; ++i2)
    {
        for (int i1 = 0; i1 < i2; ++i1)
        {
            std::vector<int> col_sum(n);
            for (int j = 0; j < n; ++j)
            {
                col_sum[j] = cum_sum[i2][j] - cum_sum[i1][j];
            }
/*
            std::cout << "\n>> " << i1 << " " << i2 << std::endl;;
            for (int j = 0; j < n; ++j) std::cout << col_sum[j] << " ";
            std::cout << std::endl;
*/
            std::vector<int> cum_cols(n);
            std::vector<int> even(n);
            std::vector<int> odd(n);
            cum_cols[0] = col_sum[0];
            even[0] = cum_cols[0] % 2 == 0;
            odd[0] = 1 - even[0];
            for (int j = 1; j < n; ++j)
            {
                cum_cols[j] = cum_cols[j - 1] + col_sum[j];
                even[j] = even[j - 1] + (cum_cols[j] % 2 == 0);
                odd[j] = odd[j - 1] + (cum_cols[j] % 2 != 0);
            }
/*
            for (int j = 0; j < n; ++j) std::cout << cum_cols[j] << " ";
            std::cout << std::endl;
            for (int j = 0; j < n; ++j) std::cout << even[j] << " ";
            std::cout << std::endl;
            for (int j = 0; j < n; ++j) std::cout << odd[j] << " ";
            std::cout << std::endl;
*/
            long long part_sol = (long long) (col_sum[0] % 2 == 0);
            //std::cout << part_sol << " ";
            for (int j = 1; j < n; ++j)
            {
                if (cum_cols[j] % 2 == 0) part_sol = part_sol + 1 + even[j - 1];
                else part_sol = part_sol + odd[j - 1];
                //std::cout << part_sol << " ";
            }
            //std::cout << std::endl;

            solution += part_sol;
        }
    }

    std::cout << solution << std::endl;
}

int main()
{
    std::ios::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
