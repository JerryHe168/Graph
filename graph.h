#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <string>

namespace graph {

template<typename T = int>
class Graph {
public:
    class Node {
    private:
        T data;
        int id;
        std::set<int> incomingEdges;
        std::set<int> outgoingEdges;
        Graph* ownerGraph;
    
    public:
        Node(Graph* owner, int id, const T& data) 
            : ownerGraph(owner), id(id), data(data) {}
        
        int getId() const { return id; }
        T& getData() { return data; }
        const T& getData() const { return data; }
        void setData(const T& newData) { data = newData; }
        Graph* getOwnerGraph() const { return ownerGraph; }
        
        const std::set<int>& getIncomingEdges() const { return incomingEdges; }
        const std::set<int>& getOutgoingEdges() const { return outgoingEdges; }
        
        size_t getInDegree() const { return incomingEdges.size(); }
        size_t getOutDegree() const { return outgoingEdges.size(); }
        size_t getDegree() const { 
            if (ownerGraph && ownerGraph->isDirectedGraph()) {
                return incomingEdges.size() + outgoingEdges.size();
            }
            return outgoingEdges.size();
        }
        
        void addIncomingEdge(int edgeId) { incomingEdges.insert(edgeId); }
        void addOutgoingEdge(int edgeId) { outgoingEdges.insert(edgeId); }
        void removeIncomingEdge(int edgeId) { incomingEdges.erase(edgeId); }
        void removeOutgoingEdge(int edgeId) { outgoingEdges.erase(edgeId); }
    };

    class Edge {
    private:
        int id;
        int fromNodeId;
        int toNodeId;
        double weight;
        bool directed;
        Graph* ownerGraph;
    
    public:
        Edge(Graph* owner, int id, int from, int to, double weight = 1.0, bool directed = false)
            : ownerGraph(owner), id(id), fromNodeId(from), toNodeId(to), 
              weight(weight), directed(directed) {}
        
        int getId() const { return id; }
        int getFromNodeId() const { return fromNodeId; }
        int getToNodeId() const { return toNodeId; }
        double getWeight() const { return weight; }
        void setWeight(double newWeight) { weight = newWeight; }
        bool isDirected() const { return directed; }
        Graph* getOwnerGraph() const { return ownerGraph; }
        
        int getOtherNodeId(int nodeId) const {
            if (nodeId == fromNodeId) return toNodeId;
            if (nodeId == toNodeId) return fromNodeId;
            return -1;
        }
    };

private:
    std::string name;
    std::map<int, std::shared_ptr<Node>> nodes;
    std::map<int, std::shared_ptr<Edge>> edges;
    int nextNodeId;
    int nextEdgeId;
    bool isDirected;
    
    Graph* parentGraph;
    std::vector<Graph*> subGraphs;
    
    int generateNodeId() { return nextNodeId++; }
    int generateEdgeId() { return nextEdgeId++; }
    
    void deepCopy(const Graph& other);

public:
    Graph(const std::string& name = "", bool directed = false)
        : name(name), nextNodeId(0), nextEdgeId(0), isDirected(directed), 
          parentGraph(nullptr) {}
    
    Graph(const Graph& other) : parentGraph(nullptr) {
        deepCopy(other);
    }
    
    Graph& operator=(const Graph& other) {
        if (this != &other) {
            for (auto sub : subGraphs) {
                delete sub;
            }
            subGraphs.clear();
            parentGraph = nullptr;
            deepCopy(other);
        }
        return *this;
    }
    
    ~Graph() {
        for (auto subgraph : subGraphs) {
            delete subgraph;
        }
    }
    
    const std::string& getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }
    
    bool isEmpty() const { return nodes.empty(); }
    size_t getNodeCount() const { return nodes.size(); }
    size_t getEdgeCount() const { return edges.size(); }
    bool isDirectedGraph() const { return isDirected; }
    
    Node* addNode(const T& data = T()) {
        int id = generateNodeId();
        auto node = std::make_shared<Node>(this, id, data);
        nodes[id] = node;
        return node.get();
    }
    
    Node* addNodeWithId(int id, const T& data = T()) {
        if (nodes.find(id) != nodes.end()) {
            return nullptr;
        }
        auto node = std::make_shared<Node>(this, id, data);
        nodes[id] = node;
        if (id >= nextNodeId) {
            nextNodeId = id + 1;
        }
        return node.get();
    }
    
    bool removeNode(int nodeId) {
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
    
    bool removeNode(Node* node) {
        if (!node || node->getOwnerGraph() != this) {
            return false;
        }
        return removeNode(node->getId());
    }
    
    Node* getNode(int nodeId) {
        auto it = nodes.find(nodeId);
        return (it != nodes.end()) ? it->second.get() : nullptr;
    }
    
    const Node* getNode(int nodeId) const {
        auto it = nodes.find(nodeId);
        return (it != nodes.end()) ? it->second.get() : nullptr;
    }
    
    bool containsNode(int nodeId) const {
        return nodes.find(nodeId) != nodes.end();
    }
    
    Edge* addEdge(int fromNodeId, int toNodeId, double weight = 1.0) {
        if (nodes.find(fromNodeId) == nodes.end() || nodes.find(toNodeId) == nodes.end()) {
            return nullptr;
        }
        
        int id = generateEdgeId();
        auto edge = std::make_shared<Edge>(this, id, fromNodeId, toNodeId, weight, isDirected);
        edges[id] = edge;
        
        nodes[fromNodeId]->addOutgoingEdge(id);
        nodes[toNodeId]->addIncomingEdge(id);
        
        if (!isDirected) {
            nodes[toNodeId]->addOutgoingEdge(id);
            nodes[fromNodeId]->addIncomingEdge(id);
        }
        
        return edge.get();
    }
    
    Edge* addEdge(Node* fromNode, Node* toNode, double weight = 1.0) {
        if (!fromNode || !toNode || 
            fromNode->getOwnerGraph() != this || 
            toNode->getOwnerGraph() != this) {
            return nullptr;
        }
        return addEdge(fromNode->getId(), toNode->getId(), weight);
    }
    
    bool removeEdge(int edgeId) {
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
    
    bool removeEdge(Edge* edge) {
        if (!edge || edge->getOwnerGraph() != this) {
            return false;
        }
        return removeEdge(edge->getId());
    }
    
    Edge* getEdge(int edgeId) {
        auto it = edges.find(edgeId);
        return (it != edges.end()) ? it->second.get() : nullptr;
    }
    
    const Edge* getEdge(int edgeId) const {
        auto it = edges.find(edgeId);
        return (it != edges.end()) ? it->second.get() : nullptr;
    }
    
    bool containsEdge(int edgeId) const {
        return edges.find(edgeId) != edges.end();
    }
    
    Edge* findEdge(int fromNodeId, int toNodeId) const {
        for (const auto& pair : edges) {
            const Edge* edge = pair.second.get();
            if (edge->getFromNodeId() == fromNodeId && edge->getToNodeId() == toNodeId) {
                return const_cast<Edge*>(edge);
            }
            if (!isDirected && edge->getFromNodeId() == toNodeId && edge->getToNodeId() == fromNodeId) {
                return const_cast<Edge*>(edge);
            }
        }
        return nullptr;
    }
    
    Edge* findEdge(const Node* fromNode, const Node* toNode) const {
        if (!fromNode || !toNode) {
            return nullptr;
        }
        return findEdge(fromNode->getId(), toNode->getId());
    }
    
    bool hasEdge(int fromNodeId, int toNodeId) const {
        return findEdge(fromNodeId, toNodeId) != nullptr;
    }
    
    bool hasEdge(const Node* fromNode, const Node* toNode) const {
        return findEdge(fromNode, toNode) != nullptr;
    }
    
    std::vector<Node*> getNeighbors(int nodeId) const {
        std::vector<Node*> neighbors;
        auto node = getNode(nodeId);
        if (!node) return neighbors;
        
        std::set<int> added;
        for (int edgeId : node->getOutgoingEdges()) {
            auto edgeIt = edges.find(edgeId);
            if (edgeIt != edges.end()) {
                int neighborId = edgeIt->second->getOtherNodeId(nodeId);
                if (added.find(neighborId) == added.end() && neighborId >= 0) {
                    auto nodeIt = nodes.find(neighborId);
                    if (nodeIt != nodes.end()) {
                        neighbors.push_back(nodeIt->second.get());
                        added.insert(neighborId);
                    }
                }
            }
        }
        
        return neighbors;
    }
    
    std::vector<Node*> getNeighbors(const Node* node) const {
        if (!node) {
            return std::vector<Node*>();
        }
        return getNeighbors(node->getId());
    }
    
    std::vector<int> getNeighborIds(int nodeId) const {
        std::vector<int> neighborIds;
        auto node = getNode(nodeId);
        if (!node) return neighborIds;
        
        std::set<int> added;
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
    
    std::vector<Node*> getAllNodes() const {
        std::vector<Node*> result;
        for (const auto& pair : nodes) {
            result.push_back(pair.second.get());
        }
        return result;
    }
    
    std::vector<Edge*> getAllEdges() const {
        std::vector<Edge*> result;
        for (const auto& pair : edges) {
            result.push_back(pair.second.get());
        }
        return result;
    }
    
    std::vector<Edge*> getEdgesFrom(int nodeId) const {
        std::vector<Edge*> result;
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
    
    std::vector<Edge*> getEdgesTo(int nodeId) const {
        std::vector<Edge*> result;
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
    
    Graph* createSubGraph(const std::string& subGraphName = "") {
        std::string actualName = subGraphName;
        if (actualName.empty()) {
            actualName = name + "_sub_" + std::to_string(subGraphs.size());
        }
        
        Graph* subGraph = new Graph(actualName, isDirected);
        subGraph->parentGraph = this;
        subGraphs.push_back(subGraph);
        return subGraph;
    }
    
    Graph* getParentGraph() const {
        return parentGraph;
    }
    
    std::vector<Graph*> getSubGraphs() const {
        return subGraphs;
    }
    
    Graph* getSubGraph(const std::string& subGraphName) const {
        for (Graph* sub : subGraphs) {
            if (sub->getName() == subGraphName) {
                return sub;
            }
        }
        return nullptr;
    }
    
    Graph* getSubGraphAt(size_t index) const {
        if (index >= subGraphs.size()) {
            return nullptr;
        }
        return subGraphs[index];
    }
    
    size_t getSubGraphCount() const {
        return subGraphs.size();
    }
    
    bool removeSubGraph(Graph* subGraph) {
        auto it = std::find(subGraphs.begin(), subGraphs.end(), subGraph);
        if (it == subGraphs.end()) {
            return false;
        }
        subGraphs.erase(it);
        delete subGraph;
        return true;
    }
    
    bool removeSubGraph(const std::string& subGraphName) {
        Graph* subGraph = getSubGraph(subGraphName);
        if (subGraph) {
            return removeSubGraph(subGraph);
        }
        return false;
    }
    
    bool removeSubGraphAt(size_t index) {
        if (index >= subGraphs.size()) {
            return false;
        }
        Graph* subGraph = subGraphs[index];
        subGraphs.erase(subGraphs.begin() + index);
        delete subGraph;
        return true;
    }
    
    int getDepth() const {
        int depth = 0;
        Graph* current = parentGraph;
        while (current) {
            depth++;
            current = current->parentGraph;
        }
        return depth;
    }
    
    Graph* getRootGraph() {
        Graph* current = this;
        while (current->parentGraph) {
            current = current->parentGraph;
        }
        return current;
    }
    
    const Graph* getRootGraph() const {
        const Graph* current = this;
        while (current->parentGraph) {
            current = current->parentGraph;
        }
        return current;
    }
    
    bool isDescendantOf(const Graph* ancestor) const {
        const Graph* current = parentGraph;
        while (current) {
            if (current == ancestor) {
                return true;
            }
            current = current->parentGraph;
        }
        return false;
    }
    
    bool isAncestorOf(const Graph* descendant) const {
        return descendant->isDescendantOf(this);
    }
    
    bool isRoot() const {
        return parentGraph == nullptr;
    }
    
    bool isLeaf() const {
        return subGraphs.empty();
    }
    
    void clear() {
        nodes.clear();
        edges.clear();
        nextNodeId = 0;
        nextEdgeId = 0;
    }
    
    void clearAll() {
        clear();
        for (auto sub : subGraphs) {
            delete sub;
        }
        subGraphs.clear();
    }
    
    void print(std::ostream& os = std::cout) const {
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
            const Edge* e = pair.second.get();
            os << "  Edge " << e->getId() << ": " 
               << e->getFromNodeId() << " -> " << e->getToNodeId()
               << " (weight: " << e->getWeight() << ")" << std::endl;
        }
        
        if (!subGraphs.empty()) {
            os << "Subgraphs (" << subGraphs.size() << "):" << std::endl;
            for (const Graph* sub : subGraphs) {
                os << "  - " << sub->getName() << " (depth: " << sub->getDepth() 
                   << ", nodes: " << sub->getNodeCount() << ")" << std::endl;
            }
        }
        
        if (parentGraph) {
            os << "Parent graph: " << parentGraph->getName() << std::endl;
        }
        os << "------------------------" << std::endl;
    }
    
    void printHierarchy(std::ostream& os = std::cout, int indent = 0) const {
        std::string indentStr(indent * 2, ' ');
        os << indentStr << "Graph: " << name 
           << " (Nodes: " << nodes.size() << ", Edges: " << edges.size() 
           << ", Depth: " << getDepth() << ")" << std::endl;
        
        for (const Graph* sub : subGraphs) {
            sub->printHierarchy(os, indent + 1);
        }
    }
    
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Graph<U>& g);
};

template<typename T>
void Graph<T>::deepCopy(const Graph<T>& other) {
    name = other.name;
    isDirected = other.isDirected;
    nextNodeId = other.nextNodeId;
    nextEdgeId = other.nextEdgeId;
    
    nodes.clear();
    edges.clear();
    
    for (const auto& pair : other.nodes) {
        auto node = std::make_shared<Node>(this, pair.first, pair.second->getData());
        nodes[pair.first] = node;
    }
    
    for (const auto& pair : other.edges) {
        const Edge* origEdge = pair.second.get();
        auto edge = std::make_shared<Edge>(
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
    
    for (const Graph* sub : other.subGraphs) {
        Graph* newSub = new Graph(*sub);
        newSub->parentGraph = this;
        subGraphs.push_back(newSub);
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Graph<T>& g) {
    g.print(os);
    return os;
}

}

#endif
