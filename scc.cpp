/* Дан ориентированный граф. Определите,
 * какое минимальное число ребер необходимо добавить, чтобы граф стал сильносвязным.
 * В графе возможны петли.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>


struct IGraph {
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;
};


class ListGraph : public IGraph {
private:
    std::vector<std::vector<int>> verticesNeighbors;

public:
    ListGraph(int verticesCount);
    ListGraph(const IGraph*);
    virtual ~ListGraph() override;

    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;
};

ListGraph::ListGraph(int verticesCount) {
    verticesNeighbors.resize(verticesCount);
}

ListGraph::ListGraph(const IGraph* graph) {
    verticesNeighbors.resize(graph->VerticesCount());
    for (size_t i = 0; static_cast<int>(i) < graph->VerticesCount(); ++i) {
        graph->GetNextVertices(i, verticesNeighbors[i]);
    }
}

ListGraph::~ListGraph() {

}


void ListGraph::AddEdge(int from, int to) {
    verticesNeighbors[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return static_cast<int>(verticesNeighbors.size());
}

void ListGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const {
    vertices.clear();
    vertices = verticesNeighbors[vertex];
}

void ListGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const {
    vertices.clear();
    for (size_t v = 0; v < verticesNeighbors.size(); ++v) {
        for (size_t i = 0; i < verticesNeighbors[v].size(); ++i) {
            if (verticesNeighbors[v][i] == vertex) {
                vertices.push_back(v);
            }
        }
    }
}

IGraph* invertGraph(IGraph* graph) {
    IGraph* invertedGraph = new ListGraph(graph->VerticesCount());
    std::vector<int> verticeDescendants;
    for (int i = 0; i < graph->VerticesCount(); ++i) {
        graph->GetNextVertices(i, verticeDescendants);
        for (auto j : verticeDescendants) {
            invertedGraph->AddEdge(j, i);
        }
    }
    return invertedGraph;
}

void DFS(IGraph* graph, int vertice, std::vector<int>& sortedVertices, std::vector<bool>& used) {
    used[vertice] = true;
    std::vector<int> verticeNeighbors;
    graph->GetNextVertices(vertice, verticeNeighbors);
    for (auto neighbor : verticeNeighbors) {
        if (used[neighbor] == false) {
            DFS(graph, neighbor, sortedVertices, used);
        }
    }
    sortedVertices.push_back(vertice);
}

// marking elements from a connected component with the same mark
void DFS(IGraph* graph, int vertice, int mark, std::vector<bool>& used, std::vector<std::vector<int>>& ConnectedComponents) {
    used[vertice] = true;
    std::vector<int> verticeNeighbors;
    graph->GetNextVertices(vertice, verticeNeighbors);
    ConnectedComponents[mark].push_back(vertice);
    for (auto neighbor : verticeNeighbors) {
        if (used[neighbor] == false) {
            DFS(graph, neighbor, mark, used, ConnectedComponents);
        }
    }
}

// Kosaraju's strongly connected components algorithm
std::vector<std::vector<int>> SCC(IGraph* graph) {
    IGraph* invertedGraph = invertGraph(graph);
    std::vector<int> sortedVertices; // sorted by exit time

    std::vector<bool> used(graph->VerticesCount());
    for (int vertice = 0; vertice < graph->VerticesCount(); ++vertice) {
        if (used[vertice] == false) {
            DFS(graph, vertice, sortedVertices, used);
        }
    }
    std::reverse(std::begin(sortedVertices), std::end(sortedVertices));
    for (int i = 0; i < graph->VerticesCount(); ++i) {
        used[i] = false;
    }

    int mark = 0;
    std::vector<std::vector<int>> ConnectedComponents(mark + 1);
    for (auto vertice : sortedVertices) {
        if (used[vertice] == false) {
            DFS(invertedGraph, vertice, mark, used, ConnectedComponents);
            mark++;
            ConnectedComponents.resize(mark + 1);
        }
    }
    ConnectedComponents.resize(mark); // last component is empty
    return ConnectedComponents;
}

bool SameColor(int vertice1, int vertice2, const std::vector<int>& color) {
    return color[vertice1] == color[vertice2];
}


int main() {
    int v, e;
    std::cin >> v >> e;

    IGraph* graph = new ListGraph(v);


    for (int i = 0; i < e; ++i) {
        int edgeStart, edgeEnd;
        std::cin >> edgeStart >> edgeEnd;
        graph->AddEdge(edgeStart - 1, edgeEnd - 1);
    }

    std::vector<std::vector<int>> CC = SCC(graph);

    std::vector<bool> isSink(CC.size(), true);
    std::vector<bool> isSource(CC.size(), true);
    std::vector<int> color(graph->VerticesCount()); // color means that i-th SCC contains current vertice
    for (int i = 0; i < CC.size(); ++i) {
        for (int j = 0; j < CC[i].size(); ++j) {
            color[CC[i][j]] = i;
        }
    }

    for (int i = 0; i < graph->VerticesCount(); ++i) {
        std::vector<int> verticeNeighbors;
        graph->GetNextVertices(i, verticeNeighbors);
        for (auto neighbor : verticeNeighbors) {
            if (!SameColor(i, neighbor, color)) {
                isSink[color[i]] = false;
                isSource[color[neighbor]] = false;
            }
        }
    }
    int sinks = 0; int sources = 0;
    for (int i = 0; i < static_cast<int>(CC.size()); ++i) {
        sinks += static_cast<int>(isSink[i]);
        sources += static_cast<int>(isSource[i]);
    }
    if (CC.size() > 1) {
        std::cout << std::max(sinks, sources);
    }
    else {
        std::cout << 0;
    }
    return 0;
}