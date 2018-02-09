#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property,
                        property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

const int NNODES = 5;
const int NEDGES = 8;

const int EDGES[NEDGES][3] = {{0, 1, 1}, {0, 2, 5}, {1, 2, 4}, {1, 3, 2},
                                {2, 4, 3}, {3, 2, 1}, {3, 4, 7}, {4, 0, 1}};
const int SOURCE = 0;
const int TARGET = 4;

int main()
{
    Graph g(NNODES);
    property_map<Graph, edge_weight_t>::type weightMap = get(edge_weight, g);
    for(int i = 0; i < NEDGES; ++i)
    {
        bool success;
        Edge e;
        tie(e, success) = add_edge(EDGES[i][0], EDGES[i][1], g);
        weightMap[e] = EDGES[i][2];
    }

    vector<Vertex> predecessors(num_vertices(g));
    vector<int> distances(num_vertices(g));
    dijkstra_shortest_paths(g, SOURCE,
                            predecessor_map(make_iterator_property_map(predecessors.begin(), get(vertex_index, g))).
                            distance_map(make_iterator_property_map(distances.begin(), get(vertex_index, g))));

    cout << "Calculated distances from " << SOURCE << ":" << endl;
    for(int i = 0; i < NNODES; ++i)
        cout << SOURCE << " to " << i << ": " << distances[i] << endl;
    cout << endl;

    cout << "Shortest path from " << SOURCE << " to " << TARGET << ":" << endl;
    cout << TARGET;
    int curr = TARGET;
    while(curr != SOURCE)
        cout << " <- " << (curr = predecessors[curr]);
    cout << endl;
}
