#include <iostream>

using namespace std;

int main()
{
    size_t t;
    cin >> t;
    for (size_t i = 0; i < t; ++i)
    {
        size_t m;
        double sum, val;
        sum = 0.0;
        cin >> m;
        for (size_t j = 0; j < m; ++j)
        {
            cin >> val;
            sum += val;
        }
        cout << sum << endl;
    }
    return 0;
}
