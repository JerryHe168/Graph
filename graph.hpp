#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <string>
#include "node.hpp"
#include "edge.hpp"

namespace graph {

template<typename T = int>
class Graph : public std::enable_shared_from_this<Graph<T>> {

template<typename U>
friend std::ostream& operator<<(std::ostream& os, const Graph<U>& g);

private:
    std::string name;
    std::map<int, std::shared_ptr<Node<T>>> nodes;
    std::map<int, std::shared_ptr<Edge<T>>> edges;
    int nextNodeId;
    int nextEdgeId;
    bool isDirected;

    std::weak_ptr<Graph<T>> parentGraph;
    std::vector<std::shared_ptr<Graph<T>>> subGraphs;

    int generateNodeId();
    int generateEdgeId();

    void deepCopy(const Graph& other);
    void deepCopySubGraphs(const Graph& other, std::shared_ptr<Graph<T>> newParent);

    std::shared_ptr<Graph<T>> getSharedPtr();
    std::shared_ptr<const Graph<T>> getSharedPtr() const;

public:
    Graph(const std::string& name = "", bool directed = false);

    Graph(const Graph& other) = delete;
    Graph& operator=(const Graph& other) = delete;

    Graph(Graph&& other) = default;
    Graph& operator=(Graph&& other) = default;

    ~Graph() = default;

    static std::shared_ptr<Graph<T>> create(const std::string& name = "", bool directed = false);

    const std::string& getName() const;
    void setName(const std::string& newName);

    bool isEmpty() const;
    size_t getNodeCount() const;
    size_t getEdgeCount() const;
    bool isDirectedGraph() const;

    Node<T>* addNode(const T& data = T());

    Node<T>* addNodeWithId(int id, const T& data = T());

    bool removeNode(int nodeId);

    bool removeNode(Node<T>* node);

    Node<T>* getNode(int nodeId);

    const Node<T>* getNode(int nodeId) const;

    bool containsNode(int nodeId) const;

    Edge<T>* addEdge(int fromNodeId, int toNodeId, double weight = 1.0);

    Edge<T>* addEdge(Node<T>* fromNode, Node<T>* toNode, double weight = 1.0);

    bool removeEdge(int edgeId);

    bool removeEdge(Edge<T>* edge);

    Edge<T>* getEdge(int edgeId);

    const Edge<T>* getEdge(int edgeId) const;

    bool containsEdge(int edgeId) const;

    Edge<T>* findEdge(int fromNodeId, int toNodeId) const;

    Edge<T>* findEdge(const Node<T>* fromNode, const Node<T>* toNode) const;

    bool hasEdge(int fromNodeId, int toNodeId) const;

    bool hasEdge(const Node<T>* fromNode, const Node<T>* toNode) const;

    std::vector<Node<T>*> getNeighbors(int nodeId) const;

    std::vector<Node<T>*> getNeighbors(const Node<T>* node) const;

    std::vector<const Node<T>*> getNeighborsConst(int nodeId) const;

    std::vector<const Node<T>*> getNeighborsConst(const Node<T>* node) const;

    std::vector<int> getNeighborIds(int nodeId) const;

    std::vector<Node<T>*> getAllNodes() const;

    std::vector<const Node<T>*> getAllNodesConst() const;

    std::vector<Edge<T>*> getAllEdges() const;

    std::vector<Edge<T>*> getEdgesFrom(int nodeId) const;

    std::vector<Edge<T>*> getEdgesTo(int nodeId) const;

    std::shared_ptr<Graph<T>> createSubGraph(const std::string& subGraphName = "");

    std::shared_ptr<Graph<T>> getParentGraph() const;

    std::vector<std::shared_ptr<Graph<T>>> getSubGraphs() const;

    std::shared_ptr<Graph<T>> getSubGraph(const std::string& subGraphName) const;

    std::shared_ptr<Graph<T>> getSubGraphAt(size_t index) const;

    size_t getSubGraphCount() const;

    bool removeSubGraph(std::shared_ptr<Graph<T>> subGraph);

    bool removeSubGraph(const std::string& subGraphName);

    bool removeSubGraphAt(size_t index);

    int getDepth() const;

    std::shared_ptr<Graph<T>> getRootGraph();

    std::shared_ptr<const Graph<T>> getRootGraph() const;

    bool isDescendantOf(const Graph* ancestor) const;

    bool isAncestorOf(const Graph* descendant) const;

    bool isRoot() const;

    bool isLeaf() const;

    void clear();

    void clearAll();

    void print(std::ostream& os = std::cout) const;

    void printHierarchy(std::ostream& os = std::cout, int indent = 0) const;

    std::shared_ptr<Graph<T>> deepClone() const;

private:
    template<typename NodePtr>
    std::vector<NodePtr> getNeighborsImpl(int nodeId) const;

    template<typename NodePtr>
    std::vector<NodePtr> getAllNodesImpl() const;
};

template<typename T>
int Graph<T>::generateNodeId() { return nextNodeId++; }

template<typename T>
int Graph<T>::generateEdgeId() { return nextEdgeId++; }

template<typename T>
Graph<T>::Graph(const std::string& name, bool directed)
    : name(name), nextNodeId(0), nextEdgeId(0), isDirected(directed) {}

template<typename T>
std::shared_ptr<Graph<T>> Graph<T>::create(const std::string& name, bool directed) {
    return std::make_shared<Graph<T>>(name, directed);
}

template<typename T>
std::shared_ptr<Graph<T>> Graph<T>::getSharedPtr() {
    return this->shared_from_this();
}

template<typename T>
std::shared_ptr<const Graph<T>> Graph<T>::getSharedPtr() const {
    return this->shared_from_this();
}

template<typename T>
const std::string& Graph<T>::getName() const { return name; }

template<typename T>
void Graph<T>::setName(const std::string& newName) { name = newName; }

template<typename T>
bool Graph<T>::isEmpty() const { return nodes.empty(); }

template<typename T>
size_t Graph<T>::getNodeCount() const { return nodes.size(); }

template<typename T>
size_t Graph<T>::getEdgeCount() const { return edges.size(); }

template<typename T>
bool Graph<T>::isDirectedGraph() const { return isDirected; }

template<typename T>
Node<T>* Graph<T>::addNode(const T& data) {
    int id = generateNodeId();
    auto node = std::make_shared<Node<T>>(this, id, data);
    nodes[id] = node;
    return node.get();
}

template<typename T>
Node<T>* Graph<T>::addNodeWithId(int id, const T& data) {
    if (nodes.find(id) != nodes.end()) {
        return nullptr;
    }
    auto node = std::make_shared<Node<T>>(this, id, data);
    nodes[id] = node;
    if (id >= nextNodeId) {
        nextNodeId = id + 1;
    }
    return node.get();
}

template<typename T>
bool Graph<T>::removeNode(int nodeId) {
    auto nodeIt = nodes.find(nodeId);
    if (nodeIt == nodes.end()) {
        return false;
    }

    auto node = nodeIt->second;

    std::vector<int> edgesToRemove;
    for (int edgeId : node->getIncomingEdges()) {
        edgesToRemove.push_back(edgeId);
    }
    for (int edgeId : node->getOutgoingEdges()) {
        edgesToRemove.push_back(edgeId);
    }

    for (int edgeId : edgesToRemove) {
        removeEdge(edgeId);
    }

    nodes.erase(nodeIt);
    return true;
}

template<typename T>
bool Graph<T>::removeNode(Node<T>* node) {
    if (!node || node->getOwnerGraph() != this) {
        return false;
    }
    return removeNode(node->getId());
}

template<typename T>
Node<T>* Graph<T>::getNode(int nodeId) {
    auto it = nodes.find(nodeId);
    return (it != nodes.end()) ? it->second.get() : nullptr;
}

template<typename T>
const Node<T>* Graph<T>::getNode(int nodeId) const {
    auto it = nodes.find(nodeId);
    return (it != nodes.end()) ? it->second.get() : nullptr;
}

template<typename T>
bool Graph<T>::containsNode(int nodeId) const {
    return nodes.find(nodeId) != nodes.end();
}

template<typename T>
Edge<T>* Graph<T>::addEdge(int fromNodeId, int toNodeId, double weight) {
    if (nodes.find(fromNodeId) == nodes.end() || nodes.find(toNodeId) == nodes.end()) {
        return nullptr;
    }

    auto fromNode = nodes[fromNodeId];
    auto toNode = nodes[toNodeId];

    int id = generateEdgeId();
    auto edge = std::make_shared<Edge<T>>(this, id, fromNodeId, toNodeId, weight, isDirected);

    fromNode->addOutgoingEdge(id);
    toNode->addIncomingEdge(id);

    if (!isDirected) {
        toNode->addOutgoingEdge(id);
        fromNode->addIncomingEdge(id);
    }

    edges[id] = edge;

    return edge.get();
}

template<typename T>
Edge<T>* Graph<T>::addEdge(Node<T>* fromNode, Node<T>* toNode, double weight) {
    if (!fromNode || !toNode ||
        fromNode->getOwnerGraph() != this ||
        toNode->getOwnerGraph() != this) {
        return nullptr;
    }
    return addEdge(fromNode->getId(), toNode->getId(), weight);
}

template<typename T>
bool Graph<T>::removeEdge(int edgeId) {
    auto edgeIt = edges.find(edgeId);
    if (edgeIt == edges.end()) {
        return false;
    }

    auto edge = edgeIt->second;
    int fromId = edge->getFromNodeId();
    int toId = edge->getToNodeId();

    auto fromNode = nodes.find(fromId);
    if (fromNode != nodes.end()) {
        fromNode->second->removeOutgoingEdge(edgeId);
        if (!isDirected) {
            fromNode->second->removeIncomingEdge(edgeId);
        }
    }

    auto toNode = nodes.find(toId);
    if (toNode != nodes.end()) {
        toNode->second->removeIncomingEdge(edgeId);
        if (!isDirected) {
            toNode->second->removeOutgoingEdge(edgeId);
        }
    }

    edges.erase(edgeIt);
    return true;
}

template<typename T>
bool Graph<T>::removeEdge(Edge<T>* edge) {
    if (!edge || edge->getOwnerGraph() != this) {
        return false;
    }
    return removeEdge(edge->getId());
}

template<typename T>
Edge<T>* Graph<T>::getEdge(int edgeId) {
    auto it = edges.find(edgeId);
    return (it != edges.end()) ? it->second.get() : nullptr;
}

template<typename T>
const Edge<T>* Graph<T>::getEdge(int edgeId) const {
    auto it = edges.find(edgeId);
    return (it != edges.end()) ? it->second.get() : nullptr;
}

template<typename T>
bool Graph<T>::containsEdge(int edgeId) const {
    return edges.find(edgeId) != edges.end();
}

template<typename T>
Edge<T>* Graph<T>::findEdge(int fromNodeId, int toNodeId) const {
    for (const auto& pair : edges) {
        const Edge<T>* edge = pair.second.get();
        if (edge->getFromNodeId() == fromNodeId && edge->getToNodeId() == toNodeId) {
            return const_cast<Edge<T>*>(edge);
        }
        if (!isDirected && edge->getFromNodeId() == toNodeId && edge->getToNodeId() == fromNodeId) {
            return const_cast<Edge<T>*>(edge);
        }
    }
    return nullptr;
}

template<typename T>
Edge<T>* Graph<T>::findEdge(const Node<T>* fromNode, const Node<T>* toNode) const {
    if (!fromNode || !toNode) {
        return nullptr;
    }
    return findEdge(fromNode->getId(), toNode->getId());
}

template<typename T>
bool Graph<T>::hasEdge(int fromNodeId, int toNodeId) const {
    return findEdge(fromNodeId, toNodeId) != nullptr;
}

template<typename T>
bool Graph<T>::hasEdge(const Node<T>* fromNode, const Node<T>* toNode) const {
    return findEdge(fromNode, toNode) != nullptr;
}

template<typename T>
template<typename NodePtr>
std::vector<NodePtr> Graph<T>::getNeighborsImpl(int nodeId) const {
    std::vector<NodePtr> neighbors;
    const Node<T>* node = getNode(nodeId);
    if (!node) return neighbors;

    std::unordered_set<int> added;
    for (int edgeId : node->getOutgoingEdges()) {
        auto edgeIt = edges.find(edgeId);
        if (edgeIt != edges.end()) {
            int neighborId = edgeIt->second->getOtherNodeId(nodeId);
            if (added.find(neighborId) == added.end() && neighborId >= 0) {
                auto nodeIt = nodes.find(neighborId);
                if (nodeIt != nodes.end()) {
                    if constexpr (std::is_same_v<NodePtr, const Node<T>*>) {
                        neighbors.push_back(nodeIt->second.get());
                    } else {
                        neighbors.push_back(const_cast<Node<T>*>(nodeIt->second.get()));
                    }
                    added.insert(neighborId);
                }
            }
        }
    }

    return neighbors;
}

template<typename T>
std::vector<Node<T>*> Graph<T>::getNeighbors(int nodeId) const {
    return getNeighborsImpl<Node<T>*>(nodeId);
}

template<typename T>
std::vector<Node<T>*> Graph<T>::getNeighbors(const Node<T>* node) const {
    if (!node) {
        return std::vector<Node<T>*>();
    }
    return getNeighbors(node->getId());
}

template<typename T>
std::vector<const Node<T>*> Graph<T>::getNeighborsConst(int nodeId) const {
    return getNeighborsImpl<const Node<T>*>(nodeId);
}

template<typename T>
std::vector<const Node<T>*> Graph<T>::getNeighborsConst(const Node<T>* node) const {
    if (!node) {
        return std::vector<const Node<T>*>();
    }
    return getNeighborsConst(node->getId());
}

template<typename T>
std::vector<int> Graph<T>::getNeighborIds(int nodeId) const {
    std::vector<int> neighborIds;
    auto node = getNode(nodeId);
    if (!node) return neighborIds;

    std::unordered_set<int> added;
    for (int edgeId : node->getOutgoingEdges()) {
        auto edgeIt = edges.find(edgeId);
        if (edgeIt != edges.end()) {
            int neighborId = edgeIt->second->getOtherNodeId(nodeId);
            if (added.find(neighborId) == added.end() && neighborId >= 0) {
                neighborIds.push_back(neighborId);
                added.insert(neighborId);
            }
        }
    }

    return neighborIds;
}

template<typename T>
template<typename NodePtr>
std::vector<NodePtr> Graph<T>::getAllNodesImpl() const {
    std::vector<NodePtr> result;
    for (const auto& pair : nodes) {
        if constexpr (std::is_same_v<NodePtr, const Node<T>*>) {
            result.push_back(pair.second.get());
        } else {
            result.push_back(const_cast<Node<T>*>(pair.second.get()));
        }
    }
    return result;
}

template<typename T>
std::vector<Node<T>*> Graph<T>::getAllNodes() const {
    return getAllNodesImpl<Node<T>*>();
}

template<typename T>
std::vector<const Node<T>*> Graph<T>::getAllNodesConst() const {
    return getAllNodesImpl<const Node<T>*>();
}

template<typename T>
std::vector<Edge<T>*> Graph<T>::getAllEdges() const {
    std::vector<Edge<T>*> result;
    for (const auto& pair : edges) {
        result.push_back(pair.second.get());
    }
    return result;
}

template<typename T>
std::vector<Edge<T>*> Graph<T>::getEdgesFrom(int nodeId) const {
    std::vector<Edge<T>*> result;
    auto node = getNode(nodeId);
    if (!node) return result;

    for (int edgeId : node->getOutgoingEdges()) {
        auto it = edges.find(edgeId);
        if (it != edges.end()) {
            result.push_back(it->second.get());
        }
    }
    return result;
}

template<typename T>
std::vector<Edge<T>*> Graph<T>::getEdgesTo(int nodeId) const {
    std::vector<Edge<T>*> result;
    auto node = getNode(nodeId);
    if (!node) return result;

    for (int edgeId : node->getIncomingEdges()) {
        auto it = edges.find(edgeId);
        if (it != edges.end()) {
            result.push_back(it->second.get());
        }
    }
    return result;
}

template<typename T>
std::shared_ptr<Graph<T>> Graph<T>::createSubGraph(const std::string& subGraphName) {
    std::string actualName = subGraphName;
    if (actualName.empty()) {
        actualName = name + "_sub_" + std::to_string(subGraphs.size());
    }

    auto subGraph = Graph<T>::create(actualName, isDirected);
    subGraph->parentGraph = this->getSharedPtr();
    subGraphs.push_back(subGraph);
    return subGraph;
}

template<typename T>
std::shared_ptr<Graph<T>> Graph<T>::getParentGraph() const {
    return parentGraph.lock();
}

template<typename T>
std::vector<std::shared_ptr<Graph<T>>> Graph<T>::getSubGraphs() const {
    return subGraphs;
}

template<typename T>
std::shared_ptr<Graph<T>> Graph<T>::getSubGraph(const std::string& subGraphName) const {
    for (const auto& sub : subGraphs) {
        if (sub->getName() == subGraphName) {
            return sub;
        }
    }
    return nullptr;
}

template<typename T>
std::shared_ptr<Graph<T>> Graph<T>::getSubGraphAt(size_t index) const {
    if (index >= subGraphs.size()) {
        return nullptr;
    }
    return subGraphs[index];
}

template<typename T>
size_t Graph<T>::getSubGraphCount() const {
    return subGraphs.size();
}

template<typename T>
bool Graph<T>::removeSubGraph(std::shared_ptr<Graph<T>> subGraph) {
    auto it = std::find(subGraphs.begin(), subGraphs.end(), subGraph);
    if (it == subGraphs.end()) {
        return false;
    }
    subGraphs.erase(it);
    return true;
}

template<typename T>
bool Graph<T>::removeSubGraph(const std::string& subGraphName) {
    auto subGraph = getSubGraph(subGraphName);
    if (subGraph) {
        return removeSubGraph(subGraph);
    }
    return false;
}

template<typename T>
bool Graph<T>::removeSubGraphAt(size_t index) {
    if (index >= subGraphs.size()) {
        return false;
    }
    subGraphs.erase(subGraphs.begin() + index);
    return true;
}

template<typename T>
int Graph<T>::getDepth() const {
    int depth = 0;
    auto current = parentGraph.lock();
    while (current) {
        depth++;
        current = current->parentGraph.lock();
    }
    return depth;
}

template<typename T>
std::shared_ptr<Graph<T>> Graph<T>::getRootGraph() {
    auto current = this->getSharedPtr();
    while (auto p = current->parentGraph.lock()) {
        current = p;
    }
    return current;
}

template<typename T>
std::shared_ptr<const Graph<T>> Graph<T>::getRootGraph() const {
    auto current = this->getSharedPtr();
    while (auto p = current->parentGraph.lock()) {
        current = p;
    }
    return current;
}

template<typename T>
bool Graph<T>::isDescendantOf(const Graph* ancestor) const {
    auto current = parentGraph.lock();
    while (current) {
        if (current.get() == ancestor) {
            return true;
        }
        current = current->parentGraph.lock();
    }
    return false;
}

template<typename T>
bool Graph<T>::isAncestorOf(const Graph* descendant) const {
    return descendant->isDescendantOf(this);
}

template<typename T>
bool Graph<T>::isRoot() const {
    return parentGraph.expired();
}

template<typename T>
bool Graph<T>::isLeaf() const {
    return subGraphs.empty();
}

template<typename T>
void Graph<T>::clear() {
    nodes.clear();
    edges.clear();
    nextNodeId = 0;
    nextEdgeId = 0;
}

template<typename T>
void Graph<T>::clearAll() {
    clear();
    subGraphs.clear();
}

template<typename T>
void Graph<T>::deepCopySubGraphs(const Graph<T>& other, std::shared_ptr<Graph<T>> newParent) {
    for (const auto& sub : other.subGraphs) {
        auto newSub = Graph<T>::create(sub->getName(), sub->isDirected);
        newSub->parentGraph = newParent;

        for (const auto& pair : sub->nodes) {
            auto node = std::make_shared<Node<T>>(newSub.get(), pair.first, pair.second->getData());
            newSub->nodes[pair.first] = node;
        }

        for (const auto& pair : sub->edges) {
            const Edge<T>* origEdge = pair.second.get();
            auto edge = std::make_shared<Edge<T>>(
                newSub.get(), origEdge->getId(),
                origEdge->getFromNodeId(), origEdge->getToNodeId(),
                origEdge->getWeight(), origEdge->isDirected()
            );

            auto fromNode = newSub->nodes.find(origEdge->getFromNodeId());
            if (fromNode != newSub->nodes.end()) {
                fromNode->second->addOutgoingEdge(origEdge->getId());
                if (!newSub->isDirected) {
                    fromNode->second->addIncomingEdge(origEdge->getId());
                }
            }

            auto toNode = newSub->nodes.find(origEdge->getToNodeId());
            if (toNode != newSub->nodes.end()) {
                toNode->second->addIncomingEdge(origEdge->getId());
                if (!newSub->isDirected) {
                    toNode->second->addOutgoingEdge(origEdge->getId());
                }
            }

            newSub->edges[pair.first] = edge;
        }

        newSub->nextNodeId = sub->nextNodeId;
        newSub->nextEdgeId = sub->nextEdgeId;

        newSub->deepCopySubGraphs(*sub, newSub);

        newParent->subGraphs.push_back(newSub);
    }
}

template<typename T>
void Graph<T>::deepCopy(const Graph<T>& other) {
    name = other.name;
    isDirected = other.isDirected;
    nextNodeId = other.nextNodeId;
    nextEdgeId = other.nextEdgeId;

    nodes.clear();
    edges.clear();
    subGraphs.clear();
    parentGraph.reset();

    for (const auto& pair : other.nodes) {
        auto node = std::make_shared<Node<T>>(this, pair.first, pair.second->getData());
        nodes[pair.first] = node;
    }

    for (const auto& pair : other.edges) {
        const Edge<T>* origEdge = pair.second.get();
        auto edge = std::make_shared<Edge<T>>(
            this, origEdge->getId(),
            origEdge->getFromNodeId(), origEdge->getToNodeId(),
            origEdge->getWeight(), origEdge->isDirected()
        );
        edges[pair.first] = edge;

        auto fromNode = nodes.find(origEdge->getFromNodeId());
        if (fromNode != nodes.end()) {
            fromNode->second->addOutgoingEdge(origEdge->getId());
            if (!isDirected) {
                fromNode->second->addIncomingEdge(origEdge->getId());
            }
        }

        auto toNode = nodes.find(origEdge->getToNodeId());
        if (toNode != nodes.end()) {
            toNode->second->addIncomingEdge(origEdge->getId());
            if (!isDirected) {
                toNode->second->addOutgoingEdge(origEdge->getId());
            }
        }
    }

    deepCopySubGraphs(other, this->getSharedPtr());
}

template<typename T>
std::shared_ptr<Graph<T>> Graph<T>::deepClone() const {
    auto clone = Graph<T>::create(name, isDirected);
    clone->deepCopy(*this);
    return clone;
}

template<typename T>
void Graph<T>::print(std::ostream& os) const {
    os << "Graph: " << name << std::endl;
    os << "Type: " << (isDirected ? "Directed" : "Undirected") << std::endl;
    os << "Nodes (" << nodes.size() << "):" << std::endl;
    for (const auto& pair : nodes) {
        os << "  Node " << pair.first << ": " << pair.second->getData()
           << " (in: " << pair.second->getInDegree()
           << ", out: " << pair.second->getOutDegree() << ")" << std::endl;
    }
    os << "Edges (" << edges.size() << "):" << std::endl;
    for (const auto& pair : edges) {
        const Edge<T>* e = pair.second.get();
        os << "  Edge " << e->getId() << ": "
           << e->getFromNodeId() << " -> " << e->getToNodeId()
           << " (weight: " << e->getWeight() << ")" << std::endl;
    }

    if (!subGraphs.empty()) {
        os << "Subgraphs (" << subGraphs.size() << "):" << std::endl;
        for (const auto& sub : subGraphs) {
            os << "  - " << sub->getName() << " (depth: " << sub->getDepth()
               << ", nodes: " << sub->getNodeCount() << ")" << std::endl;
        }
    }

    if (auto p = parentGraph.lock()) {
        os << "Parent graph: " << p->getName() << std::endl;
    }
    os << "------------------------" << std::endl;
}

template<typename T>
void Graph<T>::printHierarchy(std::ostream& os, int indent) const {
    std::string indentStr(indent * 2, ' ');
    os << indentStr << "Graph: " << name
       << " (Nodes: " << nodes.size() << ", Edges: " << edges.size()
       << ", Depth: " << getDepth() << ")" << std::endl;

    for (const auto& sub : subGraphs) {
        sub->printHierarchy(os, indent + 1);
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Graph<T>& g) {
    g.print(os);
    return os;
}

}

#endif