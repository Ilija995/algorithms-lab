#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

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
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

int T;

void testcase()
{
    bool DEBUG = false;

    int n;
    std::cin >> n;

    if (DEBUG)
    {
        std::cout << "===============" << std::endl;
        std::cout << n << std::endl;
    }
    std::vector<std::vector<int>> table(n);
    for (int i = 0; i < n; ++i)
    {
        table[i].resize(n);
        for (int j = 0; j < n; ++j)
        {
            std::cin >> table[i][j];
        }
    }

    if (DEBUG)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j) std::cout << table[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

	Graph G(1 + n * n + 1);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	int source = 0, target = 1 + n * n;

	int di[] = {-2, -2, -1, -1, 1, 1, 2, 2};
	int dj[] = {-1, 1, -2, 2, -2, 2, -1, 1};

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (table[i][j])
            {
                if ((i + j) % 2 == 0)
                {
                    eaG.addEdge(source, 1 + i * n + j, 1);

                    for (int l = 0; l < 8; ++l)
                    {
                        int ii = i + di[l], jj = j + dj[l];
                        if (ii >= 0 && ii < n && jj >= 0 && jj < n && table[ii][jj])
                        {
                            eaG.addEdge(1 + i * n + j, 1 + ii * n + jj, 1);
                        }
                    }
                }
                else eaG.addEdge(1 + i * n + j, target, 1);
            }
        }
    }

	// Find a min cut via maxflow
	int flow = boost::push_relabel_max_flow(G, source, target);

	// BFS to find vertex set S
	std::vector<int> vis(1 + n * n + 1, false); // visited flags
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
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

	int count_positions = 0;
	// Output S
	for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (table[i][j])
                if (((i + j) % 2 == 0 && vis[1 + i * n + j]) || ((i + j) % 2 == 1 && !vis[1 + i * n + j])) ++count_positions;
        }
    }

    if (DEBUG)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j) std::cout << vis[1 + i * n + j] << " ";
            std::cout << std::endl;
        }
    }

	std::cout << count_positions << std::endl;

	if (DEBUG) std::cout << "=============================" << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin >> T; while (T--) testcase();
    return 0;
}
