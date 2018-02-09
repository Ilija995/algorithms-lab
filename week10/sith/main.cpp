#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

void testcase()
{
    int N;
    K::FT r;
    std::cin >> N >> r;

    std::vector<std::pair<K::Point_2, int>> planets(N);
    int x, y;
    for (int i = 0; i < N; ++i)
    {
        std::cin >> x >> y;
        K::Point_2 p(x, y);
        planets[i] = std::make_pair(p, i);
    }

    int left = 1, right = N;
    while (right - left > 1)
    {
        int k = (left + right) / 2;

        Triangulation t;
        t.insert(planets.begin() + k, planets.end());

        Graph G(N);
        for (Edge_iterator ei = t.finite_edges_begin(); ei != t.finite_edges_end(); ++ei)
        {
            auto v1 = ei->first->vertex((ei->second + 1) % 3);
            auto v2 = ei->first->vertex((ei->second + 2) % 3);
            if (v1->info() >= k && v2->info() >= k && CGAL::squared_distance(v1->point(), v2->point()) <= r * r)
            {
                boost::add_edge(v1->info(), v2->info(), G);
            }
        }

        std::vector<int> componentmap(N);
        int ncc = boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G)));
        std::vector<int> componentsize(ncc);
        for (int i = 0; i < N; ++i)
            ++componentsize[componentmap[i]];

        auto max_component_it = std::max_element(std::begin(componentsize), std::end(componentsize));
        if (*max_component_it >= k) left = k;
        else right = k;
    }

    std::cout << left << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
