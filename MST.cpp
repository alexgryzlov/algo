#include <iostream>
#include <vector>

#define INF 2147483647
#define None Edge(-1, -1, INF)


struct Edge {
    int source, destination, weight;
    Edge(int source, int destination, int weight) : source(source),
                                                    destination(destination),
                                                    weight(weight) {}
};

struct Graph {
    size_t V;
    std::vector<Edge> edges;

    Graph(size_t V) : V(V) {}
    void add_edge(int source, int destination, int weight) {
        edges.emplace_back(Edge(source, destination, weight));
    }
};

// Disjoint-set-union
class DSU {
private:
    std::vector<int> parent;
    std::vector<int> rank;
public:
    int find(int); // returns element's component
    void unite(int, int); // unites sets of given elements
    void make_set(int); // adds new element, creating a new set for it
    int get_component(int);

    int set_number = 0;
};


// return MST weight
int boruvka(Graph& graph) {
    int mst_weight = 0;
    DSU dsu;
    std::vector<Edge> cheapest_edge(graph.V, None);
    for (size_t i = 0; i < graph.V; ++i) {
        dsu.make_set(i);
    }

    while (dsu.set_number > 1) {
        for (size_t i = 0; i < cheapest_edge.size(); ++i) {
            cheapest_edge[i] = None;
        }

        for (auto edge : graph.edges) {
            int source_set = dsu.find(edge.source);
            int destination_set = dsu.find(edge.destination);

            if (source_set == destination_set) {
                continue;
            }

            if (edge.weight < cheapest_edge[source_set].weight) {
                cheapest_edge[source_set] = edge;
            }

            if (edge.weight < cheapest_edge[destination_set].weight) {
                cheapest_edge[destination_set] = edge;
            }
        }

        for (size_t i = 0; i < graph.V; ++i) {
            if (cheapest_edge[i].weight != INF) {
                int source_set = dsu.find(cheapest_edge[i].source);
                int destination_set = dsu.find(cheapest_edge[i].destination);

                if (source_set == destination_set) {
                    continue;
                }

                mst_weight += cheapest_edge[i].weight;
                dsu.unite(source_set, destination_set);
            }
        }
    }

    return mst_weight;
}

int DSU::find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void DSU::unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (rank[x] < rank[y]) {
        parent[x] = y;
    }
    else if (rank[x] > rank[y]) {
        parent[y] = x;
    }
        // If ranks are equal
    else {
        parent[y] = x;
        ++rank[x];
    }
    --set_number;
}

void DSU::make_set(int x) {
    parent.push_back(x);
    rank.push_back(1);
    ++set_number;
}

int DSU::get_component(int x) {
    return parent[x];
}