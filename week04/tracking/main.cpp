#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
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
    int N, M, K, X, Y;
    std::cin >> N >> M >> K >> X >> Y;

    int L = K + 1;
    Graph g(N * L);
    WeightMap weightmap = get(boost::edge_weight, g);
    int a, b, c, d;
    Edge e;	bool success;
    for (int i = 0; i < M; ++i)
    {
        std::cin >> a >> b >> c >> d;
        for (int level = 0; level < L; ++level)
        {
            if (d && level < K)
            {
                tie(e, success) = add_edge(level * N + a, (level + 1) * N + b, g);
                weightmap[e] = c;
                tie(e, success) = add_edge((level + 1) * N + a, level * N + b, g);
                weightmap[e] = c;
            }
            tie(e, success) = add_edge(level * N + a, level * N + b, g);
            weightmap[e] = c;
        }
    }

    // Dijkstra shortest paths
	// =======================
	std::vector<Vertex> predmap(N * L);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	std::vector<int> distmap(N * L);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	boost::dijkstra_shortest_paths(g, X, // We MUST provide at least one of the two maps
		predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, g))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, g))));	// distance map as Named Parameter

    std::cout << distmap[K * N + Y] << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
