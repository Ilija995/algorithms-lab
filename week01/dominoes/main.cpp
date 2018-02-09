#include <iostream>
#include <cstdio>

using namespace std;

int main()
{
    ios_base::sync_with_stdio(false);
    size_t t;
    cin >> t;
    for (size_t test_case = 0; test_case < t; ++test_case)
    {
        size_t n, h, h_prev = 1;
        int res = -1;
        cin >> n;
        for (size_t i = 0; i < n; ++i)
        {
            cin >> h;
            if (res != -1)
                continue;
            h = max(h, h_prev - 1);
            if (res == -1 && h > 1)
                h_prev = h;
            else
                res = i + 1;
        }
        if (res == -1)
            res = n;
        cout << res << endl;
    }
    return 0;
}
