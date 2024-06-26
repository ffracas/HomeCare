#include "kruskalgraph.hpp"

using namespace std;
using namespace homecare;

KruskalGraph::KruskalGraph(int N) {
    parent.push_back(0);
    for (int i = 1; i <= N; ++i) {
        parent.push_back(i);
    }
    graph.clear();
    mst.clear();
}

void KruskalGraph::addWeightedEdge(int u, int v, int w) {
    graph.emplace_back(w, edge(u, v));
}

int KruskalGraph::findSet(int i) {
    int representative = i;
    // If i is not the parent of itself then i is not the representative of his set, search for parent
    while (representative != parent[representative]) {
        representative = parent[representative];
    }
    return representative;
}

void KruskalGraph::mergeSet(int u, int v) {
    parent[u] = parent[v];
}

void KruskalGraph::generateKruskalTree() {
    sort(graph.begin(), graph.end(),                                                                // increasing weight
        [] (const pair<int, edge>& lhs, const pair<int, edge>& rhs) {return lhs.first < rhs.first; });  

    for (const auto& edge : graph) {
        int uRep = findSet(edge.second.first);
        int vRep = findSet(edge.second.second);
        if (uRep != vRep) {
            mst.push_back(edge);  // add to tree
            mergeSet(uRep, vRep);
        }
    }
}

void KruskalGraph::print() {
    cout << "Edge :" << " Weight" << '\n';
    for (const auto& edge : mst) {
        cout << edge.second.first << " - " << edge.second.second << " : "<< edge.first << '\n';
    }
}

vector<int> KruskalGraph::getKruskalRank(int elementToDestroy) {
    generateKruskalTree();
    vector<int> sorted;
    sorted.push_back(mst[0].second.first);

    for (size_t i = 0; i < sorted.size(); ++i) {
        for (const auto& edge : mst) {
            if (edge.second.first == sorted[i]) {
                addIfNotPresent(sorted, edge.second.second);
            } else if (edge.second.second == sorted[i]) {
                addIfNotPresent(sorted, edge.second.first);
            }
        }
    }
    if (sorted.size() <= elementToDestroy) {
        return sorted;
    }
    vector<int> rankedElements(sorted.begin(), sorted.begin() + elementToDestroy);
    return rankedElements;
}

void KruskalGraph::addIfNotPresent(vector<int>& sorted, int value) {
    if (find(sorted.begin(), sorted.end(), value) == sorted.end()) {
        sorted.push_back(value);
    }
}
