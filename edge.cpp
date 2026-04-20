#include "edge.h"
#include "graph.h"

namespace graph {

template<typename T>
Edge<T>::Edge(Graph<T>* owner, int id, int from, int to, double weight, bool directed)
    : ownerGraph(owner), id(id), fromNodeId(from), toNodeId(to),
      weight(weight), directed(directed) {}

template<typename T>
int Edge<T>::getId() const { return id; }

template<typename T>
int Edge<T>::getFromNodeId() const { return fromNodeId; }

template<typename T>
int Edge<T>::getToNodeId() const { return toNodeId; }

template<typename T>
double Edge<T>::getWeight() const { return weight; }

template<typename T>
void Edge<T>::setWeight(double newWeight) { weight = newWeight; }

template<typename T>
bool Edge<T>::isDirected() const { return directed; }

template<typename T>
Graph<T>* Edge<T>::getOwnerGraph() const { return ownerGraph; }

template<typename T>
int Edge<T>::getOtherNodeId(int nodeId) const {
    if (nodeId == fromNodeId) return toNodeId;
    if (nodeId == toNodeId) return fromNodeId;
    return -1;
}

template class Edge<int>;
template class Edge<std::string>;

}
