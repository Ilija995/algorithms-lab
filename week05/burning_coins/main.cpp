#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

const int N = 2505;
int d[N][N][2];

void testcase()
{
    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; ++i)
        cin >> v[i];

    for (int i = 0; i < n; ++i)
    {
        d[i][i][1] = v[i];
        d[i][i][0] = 0;
    }

    for (int diag = 1; diag < n; ++diag)
    {
        for (int side = 0; side < 2; ++side)
        {
            for (int i = 0, j = diag; j < n; ++i, ++j)
            {
                if (side == 1)
                    d[i][j][side] = max(v[i] + d[i + 1][j][1 - side], v[j] + d[i][j - 1][1 - side]);
                else
                    d[i][j][side] = min(d[i + 1][j][1 - side], d[i][j - 1][1 - side]);
            }
        }
    }

    cout << d[0][n - 1][1] << endl;
}

int main()
{
    int T;
    cin >> T;
    while (T--) testcase();
    return 0;
}
