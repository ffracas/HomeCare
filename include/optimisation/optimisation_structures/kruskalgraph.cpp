#include "kruskalgraph.hpp"

using namespace std;
using namespace homecare;

KruskalGraph::KruskalGraph(int N) {
    parent = new int[N];
    this -> nNodes = N;
    for (int i = 0; i < N; i++) { parent[i] = i; }
    graph.clear();
    mst.clear();
}

void KruskalGraph::addWeightedEdge(int u, int v, int w) {
    graph.push_back(make_pair(w, edge(u, v)));
}

int KruskalGraph::findSet(int i) {
    int representative = i;
    // If i is not the parent of itself then i is not the representative of his set, search for parent
    while (representative != parent[representative]) {
        representative = parent[representative];
    }
    // If i is the parent of itself
    return representative;
}

void KruskalGraph::mergeSet(int u, int v) {
    parent[u] = parent[v];
}

void KruskalGraph::generateKruskalTree() {
    int i, uRep, vRep;
    sort(graph.begin(), graph.end());  // increasing weight

    for (i = 0; i < graph.size(); i++) {
        uRep = findSet(graph[i].second.first);
        vRep = findSet(graph[i].second.second);
        if (uRep != vRep) {
            mst.push_back(graph[i]);  // add to tree
            mergeSet(uRep, vRep);
        }
    }
}

void KruskalGraph::print() {
    cout << "Edge :" << " Weight" << '\n';
    for (int i = 0; i < mst.size(); i++) {
        cout << mst[i].second.first << " - " << mst[i].second.second << " : "
        << mst[i].first;
        cout << '\n';
    }
}

vector<int> KruskalGraph::getKruskalRank(int elementToDestroy) {
    generateKruskalTree();
    vector<int> sorted;
    sorted.push_back(mst[0].second.first);

    for (int i = 0; i < sorted.size(); ++i) {
        for (int j = 0; j < mst.size(); ++j) {
            if (mst[j].second.first == sorted[i]) {
                addIfNotPresent(sorted, mst[j].second.second);
            } else if (mst[j].second.second == sorted[i]) {
                addIfNotPresent(sorted, mst[j].second.first);
            }
        }
    }
    if (sorted.size() <= elementToDestroy) { return sorted; }
    vector<int> rankedElements(sorted.begin(), sorted.begin() + elementToDestroy);
    return rankedElements;
}

void KruskalGraph::addIfNotPresent(vector<int>& sorted, int value) {
    if (find(sorted.begin(), sorted.end(), value) == sorted.end()) {
        sorted.push_back(value);
    }
}
