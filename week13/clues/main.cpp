#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/bipartite.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Finite_edges_iterator Edge_iter;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Vertex_iterator Vertex_iter;
typedef Triangulation::Finite_vertices_iterator Vertex_finite_iter;
typedef Triangulation::Vertex_circulator Vertex_circ;

typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> Graph;

void testcase()
{
    int n, m;
    long long r;
    std::cin >> n >> m >> r;
    std::vector< std::pair<K::Point_2, int> > stations;
    stations.reserve(n);

    K::Point_2 point;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> point;
        stations.emplace_back(std::make_pair(point, i));
    }

    Triangulation dt;
    dt.insert(stations.begin(), stations.end());

    Graph G(n);

	std::vector<int> visited(n, -1);
	int round = 0;
	for (Vertex_finite_iter vi = dt.finite_vertices_begin(); vi != dt.finite_vertices_end(); ++vi)
    {
        std::queue<Vertex_handle> q;
        q.push(vi);
        visited[vi->info()] = round;
        while (!q.empty())
        {
            Vertex_handle vii = q.front();
            q.pop();
            Vertex_circ vc = dt.incident_vertices(vii);
            do {
                if (!dt.is_infinite(vc) && visited[vc->info()] != round && CGAL::squared_distance(vi->point(), vc->point()) <= r * r)
                {
                    boost::add_edge(vi->info(), vc->info(), G);
                    visited[vc->info()] = round;
                    q.push(vc);
                }
            } while (++vc != dt.incident_vertices(vii));
        }
        ++round;
    }

    bool without_interferences = boost::is_bipartite(G);

    std::vector<int> component(n);
    if (without_interferences)
    {
        boost::connected_components(G, &component[0]);
    }

    K::Point_2 a, b;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> a >> b;
        if (!without_interferences)
        {
            std::cout << "n";
            continue;
        }

        if (CGAL::squared_distance(a, b) <= r * r)
        {
            std::cout << "y";
            continue;
        }

        auto vh_a = dt.nearest_vertex(a);
        auto vh_b = dt.nearest_vertex(b);

        if (CGAL::squared_distance(a, vh_a->point()) > r * r || CGAL::squared_distance(b, vh_b->point()) > r * r)
        {
            std::cout << "n";
            continue;
        }

        std::cout << ((component[vh_a->info()] == component[vh_b->info()]) ? "y" : "n");
    }

    std::cout << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
