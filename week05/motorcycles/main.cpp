#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

bool beats(K::Ray_2 *lhs, K::Ray_2 *rhs, bool and_equal)
{
    if (!CGAL::do_intersect(*lhs, *rhs))
        return false;
    auto inter = CGAL::intersection(*lhs, *rhs);
    if (const K::Point_2* inter_point = boost::get<K::Point_2>(&*inter))
    {
        if (and_equal)
            return CGAL::squared_distance(lhs->source(), *inter_point) <= CGAL::squared_distance(rhs->source(), *inter_point);
        else
            return CGAL::squared_distance(lhs->source(), *inter_point) < CGAL::squared_distance(rhs->source(), *inter_point);
    }
    throw runtime_error("strange ray intersection");
}

void testcase()
{
    int n;
    long long y_0, x_1, y_1;
    cin >> n;
    vector< pair<K::Ray_2, int> > rays;
    for (int i = 0; i < n; ++i)
    {
        cin >> y_0 >> x_1 >> y_1;
        K::Ray_2 ray = K::Ray_2(K::Point_2(0, y_0), K::Point_2(x_1, y_1));
        rays.push_back(make_pair(ray, i));
    }

    sort(rays.begin(), rays.end(), [ ](const pair<K::Ray_2, int>& lhs, const pair<K::Ray_2, int>& rhs) { return lhs.first.source() < rhs.first.source(); });

    vector< pair<K::Ray_2, int> > to_infinity;
    to_infinity.push_back(rays[0]);

    for (int i = 1; i < n; ++i)
    {
        while (!to_infinity.empty() && beats(&rays[i].first, &(*to_infinity.rbegin()).first, false)) to_infinity.pop_back();
        if (to_infinity.empty() || !beats(&(*to_infinity.rbegin()).first, &rays[i].first, true)) to_infinity.push_back(rays[i]);
    }

    sort(to_infinity.begin(), to_infinity.end(), [ ] (const pair<K::Ray_2, int>& lhs, const pair<K::Ray_2, int>& rhs) { return lhs.second < rhs.second; });

    for (auto item : to_infinity)
        cout << item.second << " ";
    cout << endl;
}

int main()
{
    int T;
    cin >> T;
    while (T--) testcase();
    return 0;
}
