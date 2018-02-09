#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<long long, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Vertex_circulator Vertex_circulator;
typedef Triangulation::Vertex_handle Vertex_handle;

void testcase()
{
    int n_participants, n_lamps;
    std::cin >> n_participants >> n_lamps;

    std::vector<K::Point_2> participants;
    participants.reserve(n_participants);
    std::vector<long long> radii;
    radii.reserve(n_participants);
    for (int i_part = 0; i_part < n_participants; ++i_part)
    {
        int x, y;
        long long r;
        std::cin >> x >> y >> r;

        K::Point_2 p(x, y);
        participants.emplace_back(p);
        radii.emplace_back(r);
    }

    long long height;
    std::cin >> height;

    std::vector< std::pair<K::Point_2, int> > lamps;
    lamps.reserve(n_lamps);
    for (int i_lamp = 0; i_lamp < n_lamps; ++i_lamp)
    {
        int x, y;
        std::cin >> x >> y;
        K::Point_2 lamp(x, y);
        lamps.emplace_back(std::make_pair(lamp, i_lamp));
    }

    Triangulation t;
    t.insert(lamps.begin(), lamps.end());

    std::vector<int> nearest_light(participants.size(), std::numeric_limits<int>::max());
    for(int i = 0; i < (int) participants.size(); ++i) {
        auto vh = t.nearest_vertex(participants[i]);
        int max_light_idx = vh->info();
        auto min_dist = radii[i] + height;
        auto min_dist_sq = min_dist * min_dist;
        K::FT dist_sq = CGAL::squared_distance(vh->point(), participants[i]);
        if(dist_sq >= min_dist_sq) {
            continue;
        }
        for(int j = 0; j <= max_light_idx; ++j) {
            dist_sq = CGAL::squared_distance(lamps[j].first, participants[i]);
            if(dist_sq < min_dist_sq) {
                nearest_light[i] = j;
                break;
            }
        }
    }

    int furtherst_light = *std::max_element(nearest_light.begin(), nearest_light.end());

    for(int i = 0; i < (int) participants.size(); ++i) {
        if(nearest_light[i] == furtherst_light) {
            std::cout << i << " ";
        }
    }
    std::cout << '\n';
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
