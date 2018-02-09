#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_set>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		 // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				                                         // interior properties of vertices
		boost::property<boost::edge_weight_t, int> 		                         // interior properties of edges
		>					                                        Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor VertexDescriptor;
const VertexDescriptor NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();

// A linear traversal over the two sorted lists of interests.
// Returns true if there are at least ‘k’ common interests.
bool haveKMatches(const std::vector<std::string>& s1, const std::vector<std::string>& s2, int k) {
    int i1 = 0, i2 = 0;
    // Stop if reaching the end of one of the lists
    // or there are already enough common interests.
    while(i1 < s1.size() && i2 < s2.size() && k > 0) {
        if(s1[i1] == s2[i2])
            --k;

        // Move the pointer to the ‘smaller’ interest.
        // In case of equality both pointers are moved on after another.
        if(s1[i1] <= s2[i2])
            ++i1;
        else
            ++i2;
    }

    return k <= 0;
}

void testcase()
{
    int N, C, f;
    std::cin >> N >> C >> f;

    std::vector< std::vector<std::string> > characteristics(N);
    for (int i = 0; i < N; ++i)
    {
        characteristics[i].reserve(C);
        for (int j = 0; j < C; ++j)
        {
            characteristics[i].emplace_back();
            std::cin >> characteristics[i].back();
        }
        std::sort(characteristics[i].begin(), characteristics[i].end());
    }

    Graph g(N);
    for (int i = 0; i < N - 1; ++i)
    {
        for (int j = i + 1; j < N; ++j)
        {
            /*
            std::unordered_set<std::string> char_union;
            char_union.insert(characteristics[i].begin(), characteristics[i].end());
            char_union.insert(characteristics[j].begin(), characteristics[j].end());

            int char_inter_no = 2 * C - char_union.size();
            if (char_inter_no > f) boost::add_edge(i, j, g);
            */
            if(haveKMatches(characteristics[i], characteristics[j], f + 1)) boost::add_edge(i, j, g);
        }
    }

    // Edmonds' maximum cardinality matching
    std::vector<VertexDescriptor> matemap(N);		// We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
    boost::edmonds_maximum_cardinality_matching(g, boost::make_iterator_property_map(matemap.begin(), boost::get(boost::vertex_index, g)));
    int matchingsize = boost::matching_size(g, boost::make_iterator_property_map(matemap.begin(), boost::get(boost::vertex_index, g)));
/*
    // Check if all buddies are included in the matching.
    bool success = true;
    for(int i = 0; i < N; ++i)
        if(matemap[i] == NULL_VERTEX) {
            success = false;
            break;
        }
*/
    std::cout << ((N == 2 * matchingsize) ? "not optimal" : "optimal") << std::endl;
    //std::cout << (success ? "not optimal" : "optimal") << std::endl;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int T; std::cin >> T; while (T--) testcase();
    return 0;
}
