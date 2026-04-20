#ifndef NODE_HPP
#define NODE_HPP

#include <set>

namespace graph {

template<typename T>
class Graph;

template<typename T>
class Node {
private:
    T data;
    int id;
    std::set<int> incomingEdges;
    std::set<int> outgoingEdges;
    Graph<T>* ownerGraph;

public:
    Node(Graph<T>* owner, int id, const T& data);
    
    int getId() const;
    T& getData();
    const T& getData() const;
    void setData(const T& newData);
    Graph<T>* getOwnerGraph() const;
    
    const std::set<int>& getIncomingEdges() const;
    const std::set<int>& getOutgoingEdges() const;
    
    size_t getInDegree() const;
    size_t getOutDegree() const;
    size_t getDegree() const;
    
    void addIncomingEdge(int edgeId);
    void addOutgoingEdge(int edgeId);
    void removeIncomingEdge(int edgeId);
    void removeOutgoingEdge(int edgeId);
};

template<typename T>
Node<T>::Node(Graph<T>* owner, int id, const T& data) 
    : ownerGraph(owner), id(id), data(data) {}

template<typename T>
int Node<T>::getId() const { return id; }

template<typename T>
T& Node<T>::getData() { return data; }

template<typename T>
const T& Node<T>::getData() const { return data; }

template<typename T>
void Node<T>::setData(const T& newData) { data = newData; }

template<typename T>
Graph<T>* Node<T>::getOwnerGraph() const { return ownerGraph; }

template<typename T>
const std::set<int>& Node<T>::getIncomingEdges() const { return incomingEdges; }

template<typename T>
const std::set<int>& Node<T>::getOutgoingEdges() const { return outgoingEdges; }

template<typename T>
size_t Node<T>::getInDegree() const { return incomingEdges.size(); }

template<typename T>
size_t Node<T>::getOutDegree() const { return outgoingEdges.size(); }

template<typename T>
size_t Node<T>::getDegree() const { 
    if (ownerGraph && ownerGraph->isDirectedGraph()) {
        return incomingEdges.size() + outgoingEdges.size();
    }
    return outgoingEdges.size();
}

template<typename T>
void Node<T>::addIncomingEdge(int edgeId) { incomingEdges.insert(edgeId); }

template<typename T>
void Node<T>::addOutgoingEdge(int edgeId) { outgoingEdges.insert(edgeId); }

template<typename T>
void Node<T>::removeIncomingEdge(int edgeId) { incomingEdges.erase(edgeId); }

template<typename T>
void Node<T>::removeOutgoingEdge(int edgeId) { outgoingEdges.erase(edgeId); }

template class Node<int>;
template class Node<std::string>;

}

#endif