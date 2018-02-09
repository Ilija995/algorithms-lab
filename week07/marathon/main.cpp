// ALGOLAB BGL Tutorial 2
// Flow example demonstrating
// - interior graph properties for flow algorithms
// - custom edge adder

// Compile and run with one of the following:
// g++ -std=c++11 -O2 flows.cpp -o flows ./flows
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 flows.cpp -o flows; ./flows

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// Namespaces
// using namespace std;
//using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_weight_t, int> > > UDGraph;
// Interior Property Maps
typedef	boost::property_map<UDGraph, boost::edge_capacity_t>::type    		UDEdgeCapacityMap;
typedef boost::property_map<UDGraph, boost::edge_weight_t>::type	        UDEdgeWeightMap;
typedef	boost::graph_traits<UDGraph>::vertex_descriptor			            UDVertex;
typedef	boost::graph_traits<UDGraph>::edge_descriptor			            UDEdge;
typedef boost::graph_traits<UDGraph>::out_edge_iterator		                UDOutEdgeIt;


typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_weight_t, int,
                boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type    		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		        ReverseEdgeMap;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	            EdgeWeightMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			            Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			                Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator		                OutEdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


// Functions
// =========
// Function for an individual testcase
void testcases() {
    int n, m, s, f;
    std::cin >> n >> m >> s >> f;
	// Create Graph and Maps
	UDGraph G_raw(n);
	UDEdgeCapacityMap capacitymap_raw = boost::get(boost::edge_capacity, G_raw);
	UDEdgeWeightMap weightmap_raw = boost::get(boost::edge_weight, G_raw);
	int a, b, c, d;
	for (int i = 0; i < m; ++i)
    {
        std::cin >> a >> b >> c >> d;
        if (a == b) continue;
		UDEdge e;
		bool success;
        boost::tie(e, success) = edge(a, b, G_raw);
        if (success)
        {
            if (weightmap_raw[e] == d)
                capacitymap_raw[e] += c;
            else if (weightmap_raw[e] > d)
            {
                weightmap_raw[e] = d;
                capacitymap_raw[e] = c;
            }
        }
        else
        {
            boost::tie(e, success) = boost::add_edge(a, b, G_raw);
            capacitymap_raw[e] = c;
            weightmap_raw[e] = d;
        }
    }

	// Dijkstra shortest paths
	// =======================
	std::vector<UDVertex> predmap(n);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	std::vector<int> distmap(n);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	dijkstra_shortest_paths(G_raw, s, // We MUST provide at least one of the two maps
		predecessor_map(make_iterator_property_map(predmap.begin(), get(boost::vertex_index, G_raw))).	// predecessor map as Named Parameter
		distance_map(make_iterator_property_map(distmap.begin(), get(boost::vertex_index, G_raw))));	// distance map as Named Parameter

	// Create Graph and Maps
	Graph G(n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	auto cmp = [](std::pair<UDVertex, int> lhs, std::pair<UDVertex, int> rhs) { return lhs.second < rhs.second; };
    std::priority_queue<std::pair<UDVertex, int>, std::vector<std::pair<UDVertex, int>>, decltype(cmp) > q(cmp);
    std::vector<int> traversed(n);
    for (int i = 0; i < n; ++i) traversed[i] = 0;

    q.push(std::make_pair(f, distmap[f]));
    while (!q.empty())
    {
        UDVertex u = q.top().first;
        q.pop();
        traversed[u] = 2;

        //std::cout << u << ": Dist (" << distmap[u] << ")" << std::endl;

		UDOutEdgeIt oebeg, oeend;
		for (tie(oebeg, oeend) = out_edges(u, G_raw); oebeg != oeend; ++oebeg) {
			UDVertex v = target(*oebeg, G_raw);
			if (traversed[v] != 2 && distmap[v] + weightmap_raw[*oebeg] == distmap[u])
            {
                if (!traversed[v])
                {
                    q.push(std::make_pair(v, distmap[v]));
                    traversed[v] = 1;
                }
                eaG.addEdge(v, u, capacitymap_raw[*oebeg]);
                //std::cout << ">> " << v << ": Dist (" << distmap[v] << ") + Weight (" << weightmap_raw[*oebeg] << ") = " << distmap[v] + weightmap_raw[*oebeg] << std::endl;
            }
		}
    }

	// Calculate flow
	// If not called otherwise, the flow algorithm uses the interior properties
	// - edge_capacity, edge_reverse (read access),
	// - edge_residual_capacity (read and write access).
	long flow = boost::push_relabel_max_flow(G, s, f);
	//std::cout << "Flow: ";
	std::cout << flow << std::endl;
    //std::cout << "------------------------------------------\n";
}

// Main function to loop over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	int T;
	std::cin >> T;
	for (; T > 0; --T)	testcases();
	return 0;
}
