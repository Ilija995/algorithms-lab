#include <iostream>
#include <queue>
#include <cmath>
#include <vector>

using namespace std;

struct bomb
{
    size_t explosion_time;
    size_t index;

    bool operator<(const bomb& b) const
    {
        return explosion_time > b.explosion_time;
    }
};

int main()
{
    size_t t;
    cin >> t;
    for (size_t test_case = 0; test_case < t; ++test_case)
    {
        size_t n;
        vector<size_t> b;
        vector<bool> d;
        vector<size_t> dd;
        priority_queue<bomb> q;

        cin >> n;
        b.resize(n);
        d.resize(n);
        dd.resize(n);
        for (size_t i = 0; i < n; ++i)
        {
            cin >> b[i];
            bomb bmb = {b[i], i};
            q.push(bmb);
            d[i] = false;
            dd[i] = 0;
        }

        size_t curr_time = 0;
        bool explosion = false;
        size_t no_levels = (size_t) log2(n + 1);
        while (!q.empty())
        {
            bomb bmb = q.top();
            q.pop();
            // check if deactivated
            size_t index = bmb.index;
            while (index > 0 && !d[index]) index = (index - 1) / 2;
            if (index > 0 || d[index]) continue;

            // it is not deactivated, so try
            size_t level = (size_t) log2(bmb.index + 1);
            size_t to_deactivate = ((size_t) pow(2.0, no_levels - level)) - 1 - dd[bmb.index];
            curr_time += to_deactivate;
            if (bmb.explosion_time < curr_time)
            {
                explosion = true;
                break;
            }
            d[bmb.index] = true;

            index = bmb.index;
            while (index > 0)
            {
                index = (index - 1) / 2;
                dd[index] += to_deactivate;
            }
        }
        cout << ((explosion) ? "no" : "yes") << endl;
    }
    return 0;
}
