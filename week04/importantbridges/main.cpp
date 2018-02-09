#include <iostream>
#include <vector>
#include <algorithm>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}

// Directed graph with integer weights on edges.
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::property<boost::edge_component_t, int> >		Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type
typedef boost::graph_traits<Graph>::edge_descriptor		    Edge;	// Edge type
typedef boost::graph_traits<Graph>::edge_iterator		    EdgeIt;	// Edge iterator
// Property map edge -> component
typedef boost::property_map<Graph, boost::edge_component_t>::type	ComponentMap;

void testcase()
{
    int N, M;
    std::cin >> N >> M;

    Graph g(N);
    int u, v;
    for (int i = 0; i < M; ++i)
    {
        std::cin >> u >> v;
        if (u < v) add_edge(u, v, g);
        else add_edge(v, u, g);
    }

    ComponentMap components = boost::get(boost::edge_component, g);
    int num_comps = boost::biconnected_components(g, components);

    std::vector<int> edges_per_component(num_comps);
    for (int i = 0; i < num_comps; ++i) edges_per_component[i] = 0;

    EdgeIt ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
        ++edges_per_component[components[*ei]];

    std::vector< std::pair<int, int> > result_edges;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    {
        if (edges_per_component[components[*ei]] == 1)
            result_edges.emplace_back(std::make_pair(boost::source(*ei, g), boost::target(*ei, g)));
    }

    std::sort(result_edges.begin(), result_edges.end(), [](const std::pair<int, int> &lhs, const std::pair<int, int> &rhs) {
                return (lhs.first == rhs.first) ? lhs.second < rhs.second : lhs.first < rhs.first;
              });

    std::cout << result_edges.size() << std::endl;
    for (const auto &edge : result_edges) std::cout << edge.first << " " << edge.second << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
