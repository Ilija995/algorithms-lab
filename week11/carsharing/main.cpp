#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
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

struct request
{
    int source;
    int target;
    int departure;
    int arrival;
    int profit;
};

void testcase()
{
    int N, S;
    std::cin >> N >> S;

    std::vector<int> initial_cars;
    initial_cars.reserve(S);
    for (int i = 0; i < S; ++i)
    {
        initial_cars.emplace_back();
        std::cin >> initial_cars.back();
    }

    std::vector<request> requests(N);
    std::vector<std::set<int>> vertices(S);
    for (int i = 0; i < N; ++i)
    {
        int sr, tg;
        std::cin >> sr >> tg >> requests[i].departure >> requests[i].arrival >> requests[i].profit;
        requests[i].source = sr - 1;
        requests[i].target = tg - 1;
        vertices[requests[i].source].insert(requests[i].departure);
        vertices[requests[i].target].insert(requests[i].arrival);
    }

    std::vector<int> cum_num_vertices(S + 1);
    cum_num_vertices[0] = 1;
    for (int i = 1; i < S + 1; ++i) cum_num_vertices[i] = cum_num_vertices[i - 1] + vertices[i - 1].size();


    // Create Graph and Maps
    Graph G(cum_num_vertices[S] + 1);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    int max_unit_profit = 100;

    int source = 0, target = cum_num_vertices[S];
    // Source to first vertices of each station thread
    for (int i = 0; i < S; ++i)
    {
        eaG.addEdge(source, cum_num_vertices[i], initial_cars[i], max_unit_profit * *vertices[i].begin());
    }

    // Ride requests
    for (int i = 0; i < N; ++i)
    {
        int u = cum_num_vertices[requests[i].source] + std::distance(vertices[requests[i].source].begin(), vertices[requests[i].source].find(requests[i].departure));
        int v = cum_num_vertices[requests[i].target] + std::distance(vertices[requests[i].target].begin(), vertices[requests[i].target].find(requests[i].arrival));
        eaG.addEdge(u, v, 1, max_unit_profit * (requests[i].arrival - requests[i].departure) - requests[i].profit);
    }

    // Edges along threads
    for (int i = 0; i < S; ++i)
    {
        for (auto it = vertices[i].begin(); std::next(it) != vertices[i].end(); ++it)
        {
            int u = cum_num_vertices[i] + std::distance(vertices[i].begin(), it);
            eaG.addEdge(u, u + 1, 1000, max_unit_profit * (*std::next(it) - *it));
        }
    }

    int last_timestamp = 0;
    for (int i = 0; i < S; ++i) if (last_timestamp < *vertices[i].rbegin()) last_timestamp = *vertices[i].rbegin();

    // Last in thread to target
    for (int i = 0; i < S; ++i)
    {
        int u = cum_num_vertices[i] + vertices[i].size() - 1;
        eaG.addEdge(u, target, 1000, max_unit_profit * (last_timestamp - *vertices[i].rbegin()));
    }

    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost2 = boost::find_flow_cost(G);
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    std::cout << s_flow * last_timestamp * max_unit_profit - cost2 << std::endl; // 12
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
