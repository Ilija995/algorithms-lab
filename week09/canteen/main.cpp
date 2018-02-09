// ALGOLAB BGL Tutorial 3
// Code snippets demonstrating
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <unordered_map>
#include <boost/functional/hash.hpp>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap)
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};

void testcase()
{
    int N;
    std::cin >> N;

    // Create Graph and Maps
    Graph G(N + 2);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    int source = 0, sink = N + 1;

    int a, c;
    for (int i = 1; i <= N; ++i)
    {
        std::cin >> a >> c;
        eaG.addEdge(source, i, a, c);
    }

    int s, p;
    int sum_s = 0;
    //std::unordered_map< std::pair<int, int>, int, boost::hash<std::pair<int, int> > > ps;
    for (int i = 1; i <= N; ++i)
    {
        std::cin >> s >> p;
        sum_s += s;
        eaG.addEdge(i, sink, s, -p);
        //std::pair<int, int> sts = std::make_pair(i, sink);
        //ps[sts] = p;
    }

    int v_i, e_i;
    for (int i = 1; i < N; ++i)
    {
        std::cin >> v_i >> e_i;
        eaG.addEdge(i, i + 1, v_i, e_i);
    }

    // Run the algorithm
    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow1 = boost::push_relabel_max_flow(G, source, sink);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
/*
    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost2 = boost::find_flow_cost(G);
    // Or equivalently, you can do the summation at the sink, but with reversed sign.
    OutEdgeIt e, eend;
    int t_flow = 0;
    int saled_profit = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(sink,G), G); e != eend; ++e) {
        int edge_flow = rescapacitymap[*e] - capacitymap[*e];
        t_flow += edge_flow;
        int u = boost::source(*e, G);
        int v = boost::target(*e, G);
        saled_profit += edge_flow * ps[std::make_pair(v, u)];
    }*/
    std::cout << ((sum_s == flow1) ? "possible " : "impossible ") << flow1 << " " << -cost1 << std::endl;
/*
    // BFS to find vertex set S
	std::vector<int> vis(2 * N + 2, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[source] = true; // Mark the source as visited
	Q.push(source);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			std::cout << u << " --" << rescapacitymap[*ebeg] << "--> " << v << std::endl;
			if (vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}*/
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T;
    std::cin >> T;
    while (T--) testcase();
    return 0;
}
