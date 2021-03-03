#include <iostream>                  // for std::cout
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using std::vector;
using std::cout;
using std::endl;
using namespace boost;

template <class PredecessorMap>
class m_record_predecessors : public dijkstra_visitor<>
{
public:
    m_record_predecessors(PredecessorMap p)
        : m_predecessor(p) { }

    template <class Edge, class Graph>
    void edge_relaxed(Edge e, Graph& g) {
        // set the parent of the target(e) to source(e)
        put(m_predecessor, target(e, g), source(e, g));
    }
protected:
    PredecessorMap m_predecessor;
};

template <class PredecessorMap>
m_record_predecessors<PredecessorMap>
m_make_predecessor_recorder(PredecessorMap p) {
    return m_record_predecessors<PredecessorMap>(p);
}

int main(int,char*[])
{


    typedef adjacency_list<listS, vecS, directedS,
            no_property, property<edge_weight_t, int> > Graph;
    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    const int num_nodes =5;
    typedef std::pair<int,int> Edge;
    enum { A, B, C, D, E, F, G , H, N };
    const char* name = "ABCDEFGH";
    Edge edge_array[] =
    { Edge(F,A), Edge(F,G), Edge(E,F), Edge(A,G), Edge(G,E), Edge(A,B),
      Edge(D,E), Edge(B,H), Edge(H,D), Edge(B,C), Edge(C,H), Edge(C,D) };
    int weights[] = { 2,1,3,4,2,2,1,2,3,3,3,1};

    Graph Gr(edge_array, edge_array + sizeof(edge_array) / sizeof(Edge), weights, num_nodes);


    std::vector<int> d(num_vertices(Gr));

    Vertex s = *(vertices(Gr).first+1);

    dijkstra_shortest_paths(Gr, s, distance_map(&d[0]));

    typename property_map<Graph, vertex_index_t>::type
            index = get(vertex_index, Gr);

    std::cout << "distances from start vertex:" << std::endl;
    graph_traits<Graph>::vertex_iterator vi;
    for(vi = vertices(Gr).first; vi != vertices(Gr).second; ++vi)
        std::cout << "distance(" << name[index[*vi]] << ") = "
                                                     << d[*vi] << std::endl;
    std::cout << std::endl;

    vector<Vertex> p(num_vertices(Gr), graph_traits<Graph>::null_vertex());
    dijkstra_shortest_paths(Gr, s, distance_map(&d[0]).
            visitor(m_make_predecessor_recorder(&p[0])));

    cout << "parents in the tree of shortest paths:" << endl;
    for(vi = vertices(Gr).first; vi != vertices(Gr).second; ++vi) {
        cout << "parent(" << name[*vi];
        if (p[*vi] == graph_traits<Graph>::null_vertex())
            cout << ") = no parent" << endl;
        else
            cout << ") = " << name[p[*vi]] << endl;
    }

}




