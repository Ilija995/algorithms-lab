#include <iostream>
#include <vector>
#include <algorithm>
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
    int index;
};

bool compare_by_departure(const request *lhs, const request *rhs)
{
    return lhs->departure < rhs->departure;
}

bool compare_by_arrival(const request *lhs, const request *rhs)
{
    return lhs->arrival < rhs->arrival;
}

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

    std::vector<request*> requests;
    requests.reserve(N);
    int so, ta, de, ar, pr;
    for (int i = 0; i < N; ++i)
    {
        std::cin >> so >> ta >> de >> ar >> pr;
        request *req;
        req = new request();
        req->source = so - 1;
        req->target = ta - 1;
        req->departure = de;
        req->arrival = ar;
        req->profit = pr;
        req->index = 1 + S + 2 * i; // index of the request source vertex
        requests.emplace_back(req);
    }

    std::vector< std::vector<request*> > req_by_source(S);
    for (int i = 0; i < N; ++i)
    {
        req_by_source[requests[i]->source].emplace_back(requests[i]);
    }

    for (int i = 0; i < S; ++i)
    {
        std::sort(req_by_source[i].begin(), req_by_source[i].end(), compare_by_departure);
    }

    // Create Graph and Maps
    Graph G(1 + S + 2 * N + 1);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    int source = 0, target = 1 + S + 2 * N;
    for (int i = 0; i < S; ++i)
    {
        //std::cout << "Add edge from " << source << " to " << 1 + i << " with capacity " << initial_cars[i] << " and weight " << 0 << std::endl;
        eaG.addEdge(source, 1 + i, initial_cars[i], 0);
    }

    for (const request *req : requests)
    {
        //std::cout << "Add edge from " << 1 + req->source << " to " << req->index << " with capacity " << 1 << " and weight " << 0 << std::endl;
        eaG.addEdge(1 + req->source, req->index, 1, 0);

        //std::cout << "Add edge from " << req->index << " to " << req->index + 1 << " with capacity " << 1 << " and weight " << -req->profit << std::endl;
        eaG.addEdge(req->index, req->index + 1, 1, -req->profit);

        request *dummy;
        dummy = new request();
        dummy->departure = req->arrival;
        auto it = std::lower_bound(req_by_source[req->target].begin(), req_by_source[req->target].end(), dummy, compare_by_departure);
        if (it != req_by_source[req->target].end())
        {
            request *min_arr_req;
            min_arr_req = *it;
            while (++it != req_by_source[req->target].end())
            {
                if (min_arr_req->arrival > (*it)->arrival)
                    min_arr_req = *it;
            }
            //std::cout << "Add edge from " << req->index + 1 << " to " << (*it)->index << " with capacity " << "inf" << " and weight " << 0 << std::endl;
            eaG.addEdge(req->index + 1, min_arr_req->index, INT_MAX, 0);
        }

        //std::cout << "Add edge from " << req->index + 1 << " to " << target << " with capacity " << "inf" << " and weight " << 0 << std::endl;
        eaG.addEdge(req->index + 1, target, INT_MAX, 0);
    }

    // Run the algorithm

    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow1 = boost::push_relabel_max_flow(G, source, target);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
    //std::cout << flow1 << std::endl;
    std::cout << -cost1 << std::endl;

/*
    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    std::cout << "-----------------------" << std::endl;
    std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << std::endl;
    std::cout << "cost " << cost2 << std::endl; // 12
    int s_flow = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G)
            << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        s_flow += capacitymap[*e] - rescapacitymap[*e];
    }
    std::cout << "s-out flow " << s_flow << std::endl; // 5
    // Or equivalently, you can do the summation at the sink, but with reversed sign.
    int t_flow = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_target,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G)
            << " with capacity " << capacitymap[*e] << " and residual capacity " << rescapacitymap[*e] << std::endl;
        t_flow += rescapacitymap[*e] - capacitymap[*e];
    }
    std::cout << "t-in flow " << t_flow << std::endl; // 5
    std::cout << "-----------------------" << std::endl;*/
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}

