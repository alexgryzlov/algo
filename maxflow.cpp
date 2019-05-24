#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>

#define INT_MAX 2147483647

struct Edge {
    int source, destination, flow, capacity, reverse;
    Edge(int source, int destination, int flow, int capacity, int reverse) : source(source), destination(destination),
                                                                             flow(flow), capacity(capacity), reverse(reverse) {}
};

struct Graph {
    size_t vertice_count;
    std::vector<std::vector<Edge>> neighbour;
    Graph(size_t size) : vertice_count(size) {
        neighbour.resize(vertice_count);
    }

    void add_edge(int from, int to, int capacity) {
        assert(to >= 0);
        assert(from >= 0);
        assert(static_cast<size_t>(to) < size());
        assert(static_cast<size_t>(from) < size());
        assert(capacity >= 0);

        neighbour[from].emplace_back(Edge(from, to, 0, capacity, neighbour[to].size()));
        neighbour[to].emplace_back(Edge(to, from, 0, 0, neighbour[from].size() - 1));
    }

    size_t size() {
        return vertice_count;
    }
};

// BFS for finding level graph
bool BFS(Graph& graph, int source, int sink, std::vector<int>& level) {
    assert(source >= 0);
    assert(sink >= 0);
    assert(static_cast<size_t>(source) < graph.size());
    assert(static_cast<size_t>(sink) < graph.size());


    for (size_t i = 0; i < graph.vertice_count; ++i) {
        level[i] = -1;
    }
    level[source] = 0;
    std::queue<int> vertice_queue;
    vertice_queue.push(source);

    while (!vertice_queue.empty()) {
        int current_vertice = vertice_queue.front(); vertice_queue.pop();
        for (auto edge : graph.neighbour[current_vertice]) {
            if (level[edge.destination] == -1 && edge.flow < edge.capacity) {
                level[edge.destination] = level[edge.source] + 1;
                vertice_queue.push(edge.destination);
            }
        }
    }
    return level[sink] + 1; // 'false' if we haven't reached the sink, 'true' otherwise
}

// DFS for finding blocking flow
// ptr is used to ignore all edges that will not lead us the sink
int DFS(Graph& graph, int source, int sink, int flow, std::vector<int>& ptr, std::vector<int>& level) {
    assert(source >= 0);
    assert(sink >= 0);
    assert(static_cast<size_t>(source) < graph.size());
    assert(static_cast<size_t>(sink) < graph.size());

    if (flow == 0) {
        return 0;
    }

    if (sink == source) {
        return flow;
    }

    for (; static_cast<size_t>(ptr[source]) < graph.neighbour[source].size(); ++ptr[source]) {
        Edge& current_edge = graph.neighbour[source][ptr[source]];

        if (level[current_edge.destination] == level[current_edge.source] + 1 &&
            current_edge.flow < current_edge.capacity) {
            int current_flow = std::min(flow, current_edge.capacity - current_edge.flow);
            int blocking_flow = DFS(graph, current_edge.destination, sink, current_flow, ptr, level);

            if (blocking_flow > 0) {
                current_edge.flow += blocking_flow;
                graph.neighbour[current_edge.destination][current_edge.reverse].flow -= blocking_flow;
                return blocking_flow;
            }
        }
    }
    return 0;
}

int dinic(Graph& graph, int source, int sink) {
    assert(source >= 0);
    assert(sink >= 0);
    assert(static_cast<size_t>(source) < graph.size());
    assert(static_cast<size_t>(sink) < graph.size());

    int max_flow = 0;
    std::vector<int> level(graph.vertice_count);
    // While we can reach the sink
    while (BFS(graph, source, sink, level)) {
        std::vector<int> ptr(graph.vertice_count + 1, 0);
        // While blocking flow isn't reached
        while (int path_flow = DFS(graph, source, sink, INT_MAX, ptr, level)) {
            max_flow += path_flow;
        }
    }
    return max_flow;
}
