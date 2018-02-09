#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
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
    int n_vertex, n_edges, n_agents, n_shelters, shelter_capacity, protocol_time;
    std::cin >> n_vertex >> n_edges >> n_agents >> n_shelters >> shelter_capacity >> protocol_time;

    Graph G(n_vertex);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
    char w;
    int x, y, z;
    for (int i_edge = 0; i_edge < n_edges; ++i_edge)
    {
        std::cin >> w >> x >> y >> z;
		Edge e;	bool success;
		if (w == 'S')
        {
            tie(e, success) = add_edge(x, y, G);
            weightmap[e] = z;
        }
        else
        {
            tie(e, success) = add_edge(x, y, G);
            weightmap[e] = z;

            tie(e, success) = add_edge(y, x, G);
            weightmap[e] = z;
        }
    }

    std::vector<int> agents;
    agents.reserve(n_agents);
    for (int i_agent = 0; i_agent < n_agents; ++i_agent)
    {
        agents.emplace_back();
        std::cin >> agents.back();
    }

    std::vector<int> shelters;
    shelters.reserve(n_shelters);
    for (int i_shelter = 0; i_shelter < n_shelters; ++i_shelter)
    {
        shelters.emplace_back();
        std::cin >> shelters.back();
    }

    std::vector< std::vector<int> > dist_agent_shelter(n_agents);
    int max_dist = 0;
    for (int i_agent = 0; i_agent < n_agents; ++i_agent)
    {
        std::vector<int> distmap(n_vertex);
        boost::dijkstra_shortest_paths(G, agents[i_agent], boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
        dist_agent_shelter[i_agent].reserve(n_shelters);
        for (int i_shelter = 0; i_shelter < n_shelters; ++i_shelter)
        {
            dist_agent_shelter[i_agent].emplace_back(distmap[shelters[i_shelter]]);
            if (distmap[shelters[i_shelter]] < INT_MAX)
                max_dist = std::max(max_dist, distmap[shelters[i_shelter]]);
        }
    }

    int left = 0, right = max_dist + protocol_time, mid;
    while (right - left > 1)
    {
        mid = (right + left) / 2;
        int dist_bound = mid - protocol_time;
        Graph G2(n_agents + shelter_capacity * n_shelters);
        for (int i_agent = 0; i_agent < n_agents; ++i_agent)
        {
            for (int i_shelter = 0; i_shelter < n_shelters; ++i_shelter)
            {
                if (dist_agent_shelter[i_agent][i_shelter] <= dist_bound)
                {
                    boost::add_edge(i_agent, n_agents + i_shelter, G2);
                }
                if (shelter_capacity == 2 && dist_agent_shelter[i_agent][i_shelter] <= dist_bound - protocol_time)
                {
                    boost::add_edge(i_agent, n_agents + n_shelters + i_shelter, G2);
                }
            }
        }

        std::vector<Vertex> matemap(n_agents + shelter_capacity * n_shelters);
        boost::edmonds_maximum_cardinality_matching(G2, boost::make_iterator_property_map(matemap.begin(), boost::get(boost::vertex_index, G2)));
        int matchingsize = boost::matching_size(G2, boost::make_iterator_property_map(matemap.begin(), boost::get(boost::vertex_index, G2)));

        if (matchingsize == n_agents) right = mid;
        else left = mid;
    }

    std::cout << right << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
