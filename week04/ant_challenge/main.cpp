#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;


typedef adjacency_list<vecS, vecS, undirectedS,
		no_property,
		property<edge_weight_t, int>
		>					Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;
typedef graph_traits<Graph>::edge_iterator		    EdgeIt;
typedef graph_traits<Graph>::out_edge_iterator		OutEdgeIt;
typedef property_map<Graph, edge_weight_t>::type	WeightMap;

void testcases()
{
	int V, E, S, start_v, target_v;
	cin >> V >> E >> S >> start_v >> target_v;
	vector<Graph> specie_graphs(S);
	vector<WeightMap> specie_weight_maps(S);
	for (int i = 0; i < S; ++i) {
        Graph specie_graph(V);
        specie_graphs.push_back(specie_graph);
        specie_weight_maps.push_back(get(edge_weight, specie_graphs[i]));
    }
	for (int i = 0; i < E; ++i) {
	    int u, v, w;
	    cin >> u >> v;
        for (int j = 0; j < S; ++j) {
            cin >> w;
            Edge e;	bool success;
            tie(e, success) = add_edge(u, v, specie_graphs[j]);
            specie_weight_maps[j][e] = w;
            assert(source(e, specie_graphs[j]) == u && target(e, specie_graphs[j]) == v);
        }
	}

	Graph G(V);
	WeightMap weightmap = get(edge_weight, G);

	for (int i = 0; i < S; ++i) {
        Vertex hive;
        cin >> hive;
        vector<Vertex> primpredmap(V);
        prim_minimum_spanning_tree(specie_graphs[i], make_iterator_property_map(primpredmap.begin(), get(vertex_index, specie_graphs[i])), root_vertex(hive));
        for (int j = 0; j < V; ++j) {
            if (primpredmap[j] != j) {
                Edge specie_e; bool specie_success;
                tie(specie_e, specie_success) = edge(j, primpredmap[j], specie_graphs[i]);
                Vertex u = source(specie_e, specie_graphs[i]), v = target(specie_e, specie_graphs[i]);

                Edge e; bool success;
                tie(e, success) = edge(u, v, G);
                if (success) {
                    if (specie_weight_maps[i][specie_e] < weightmap[e])
                        weightmap[e] = specie_weight_maps[i][specie_e];
                }
                else {
                    tie(e, success) = add_edge(u, v, G);
                    weightmap[e] = specie_weight_maps[i][specie_e];
                    assert(source(e, G) == u && target(e, G) == v);
                }
            }
        }
    }

	vector<Vertex> predmap(V);
	vector<int> distmap(V);
	dijkstra_shortest_paths(G, start_v,
		predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));
	cout << distmap[target_v] << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int T;
	cin >> T;
	while(T--)	testcases();
	return 0;
}
