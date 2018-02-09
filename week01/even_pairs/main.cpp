#include <iostream>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    size_t t;
    cin >> t;
    for (size_t test_case = 0; test_case < t; ++test_case)
    {
        size_t n, *arr;
        cin >> n;
        arr = new size_t[n];
        for (size_t i = 0; i < n; ++i)
            cin >> arr[i];

        size_t *cum_sum, *even, *odd, *res;
        cum_sum = new size_t[n];
        even = new size_t[n];
        odd = new size_t[n];
        res = new size_t[n];

        cum_sum[0] = arr[0];
        for (size_t i = 1; i < n; ++i)
            cum_sum[i] = cum_sum[i - 1] + arr[i];

        even[0] = 1;
        odd[0] = 0;
        res[0] = 1 - cum_sum[0];
        for (size_t i = 1; i < n; ++i)
        {
            even[i] = even[i - 1] + 1 - (cum_sum[i - 1] % 2);
            odd[i] = odd[i - 1] + (cum_sum[i - 1] % 2);
            res[i] = res[i - 1] + ((cum_sum[i] % 2 == 0) ? even[i] : odd[i]);
        }

        cout << res[n - 1] << endl;
    }
    return 0;
}
