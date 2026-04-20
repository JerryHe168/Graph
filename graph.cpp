#include "graph.h"

namespace graph {

template<typename T>
Graph<T>::Node::Node(Graph* owner, int id, const T& data) 
    : ownerGraph(owner), id(id), data(data) {}

template<typename T>
int Graph<T>::Node::getId() const { return id; }

template<typename T>
T& Graph<T>::Node::getData() { return data; }

template<typename T>
const T& Graph<T>::Node::getData() const { return data; }

template<typename T>
void Graph<T>::Node::setData(const T& newData) { data = newData; }

template<typename T>
Graph<T>* Graph<T>::Node::getOwnerGraph() const { return ownerGraph; }

template<typename T>
const std::set<int>& Graph<T>::Node::getIncomingEdges() const { return incomingEdges; }

template<typename T>
const std::set<int>& Graph<T>::Node::getOutgoingEdges() const { return outgoingEdges; }

template<typename T>
size_t Graph<T>::Node::getInDegree() const { return incomingEdges.size(); }

template<typename T>
size_t Graph<T>::Node::getOutDegree() const { return outgoingEdges.size(); }

template<typename T>
size_t Graph<T>::Node::getDegree() const { 
    if (ownerGraph && ownerGraph->isDirectedGraph()) {
        return incomingEdges.size() + outgoingEdges.size();
    }
    return outgoingEdges.size();
}

template<typename T>
void Graph<T>::Node::addIncomingEdge(int edgeId) { incomingEdges.insert(edgeId); }

template<typename T>
void Graph<T>::Node::addOutgoingEdge(int edgeId) { outgoingEdges.insert(edgeId); }

template<typename T>
void Graph<T>::Node::removeIncomingEdge(int edgeId) { incomingEdges.erase(edgeId); }

template<typename T>
void Graph<T>::Node::removeOutgoingEdge(int edgeId) { outgoingEdges.erase(edgeId); }

template<typename T>
Graph<T>::Edge::Edge(Graph* owner, int id, int from, int to, double weight, bool directed)
    : ownerGraph(owner), id(id), fromNodeId(from), toNodeId(to), 
      weight(weight), directed(directed) {}

template<typename T>
int Graph<T>::Edge::getId() const { return id; }

template<typename T>
int Graph<T>::Edge::getFromNodeId() const { return fromNodeId; }

template<typename T>
int Graph<T>::Edge::getToNodeId() const { return toNodeId; }

template<typename T>
double Graph<T>::Edge::getWeight() const { return weight; }

template<typename T>
void Graph<T>::Edge::setWeight(double newWeight) { weight = newWeight; }

template<typename T>
bool Graph<T>::Edge::isDirected() const { return directed; }

template<typename T>
Graph<T>* Graph<T>::Edge::getOwnerGraph() const { return ownerGraph; }

template<typename T>
int Graph<T>::Edge::getOtherNodeId(int nodeId) const {
    if (nodeId == fromNodeId) return toNodeId;
    if (nodeId == toNodeId) return fromNodeId;
    return -1;
}

template<typename T>
int Graph<T>::generateNodeId() { return nextNodeId++; }

template<typename T>
int Graph<T>::generateEdgeId() { return nextEdgeId++; }

template<typename T>
Graph<T>::Graph(const std::string& name, bool directed)
    : name(name), nextNodeId(0), nextEdgeId(0), isDirected(directed), 
      parentGraph(nullptr) {}

template<typename T>
Graph<T>::Graph(const Graph& other) : parentGraph(nullptr) {
    deepCopy(other);
}

template<typename T>
Graph<T>& Graph<T>::operator=(const Graph& other) {
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

template<typename T>
Graph<T>::~Graph() {
    for (auto subgraph : subGraphs) {
        delete subgraph;
    }
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
typename Graph<T>::Node* Graph<T>::addNode(const T& data) {
    int id = generateNodeId();
    auto node = std::make_shared<Node>(this, id, data);
    nodes[id] = node;
    return node.get();
}

template<typename T>
typename Graph<T>::Node* Graph<T>::addNodeWithId(int id, const T& data) {
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
bool Graph<T>::removeNode(Node* node) {
    if (!node || node->getOwnerGraph() != this) {
        return false;
    }
    return removeNode(node->getId());
}

template<typename T>
typename Graph<T>::Node* Graph<T>::getNode(int nodeId) {
    auto it = nodes.find(nodeId);
    return (it != nodes.end()) ? it->second.get() : nullptr;
}

template<typename T>
const typename Graph<T>::Node* Graph<T>::getNode(int nodeId) const {
    auto it = nodes.find(nodeId);
    return (it != nodes.end()) ? it->second.get() : nullptr;
}

template<typename T>
bool Graph<T>::containsNode(int nodeId) const {
    return nodes.find(nodeId) != nodes.end();
}

template<typename T>
typename Graph<T>::Edge* Graph<T>::addEdge(int fromNodeId, int toNodeId, double weight) {
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

template<typename T>
typename Graph<T>::Edge* Graph<T>::addEdge(Node* fromNode, Node* toNode, double weight) {
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
bool Graph<T>::removeEdge(Edge* edge) {
    if (!edge || edge->getOwnerGraph() != this) {
        return false;
    }
    return removeEdge(edge->getId());
}

template<typename T>
typename Graph<T>::Edge* Graph<T>::getEdge(int edgeId) {
    auto it = edges.find(edgeId);
    return (it != edges.end()) ? it->second.get() : nullptr;
}

template<typename T>
const typename Graph<T>::Edge* Graph<T>::getEdge(int edgeId) const {
    auto it = edges.find(edgeId);
    return (it != edges.end()) ? it->second.get() : nullptr;
}

template<typename T>
bool Graph<T>::containsEdge(int edgeId) const {
    return edges.find(edgeId) != edges.end();
}

template<typename T>
typename Graph<T>::Edge* Graph<T>::findEdge(int fromNodeId, int toNodeId) const {
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

template<typename T>
typename Graph<T>::Edge* Graph<T>::findEdge(const Node* fromNode, const Node* toNode) const {
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
bool Graph<T>::hasEdge(const Node* fromNode, const Node* toNode) const {
    return findEdge(fromNode, toNode) != nullptr;
}

template<typename T>
std::vector<typename Graph<T>::Node*> Graph<T>::getNeighbors(int nodeId) const {
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

template<typename T>
std::vector<typename Graph<T>::Node*> Graph<T>::getNeighbors(const Node* node) const {
    if (!node) {
        return std::vector<Node*>();
    }
    return getNeighbors(node->getId());
}

template<typename T>
std::vector<int> Graph<T>::getNeighborIds(int nodeId) const {
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

template<typename T>
std::vector<typename Graph<T>::Node*> Graph<T>::getAllNodes() const {
    std::vector<Node*> result;
    for (const auto& pair : nodes) {
        result.push_back(pair.second.get());
    }
    return result;
}

template<typename T>
std::vector<typename Graph<T>::Edge*> Graph<T>::getAllEdges() const {
    std::vector<Edge*> result;
    for (const auto& pair : edges) {
        result.push_back(pair.second.get());
    }
    return result;
}

template<typename T>
std::vector<typename Graph<T>::Edge*> Graph<T>::getEdgesFrom(int nodeId) const {
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

template<typename T>
std::vector<typename Graph<T>::Edge*> Graph<T>::getEdgesTo(int nodeId) const {
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

template<typename T>
Graph<T>* Graph<T>::createSubGraph(const std::string& subGraphName) {
    std::string actualName = subGraphName;
    if (actualName.empty()) {
        actualName = name + "_sub_" + std::to_string(subGraphs.size());
    }
    
    Graph* subGraph = new Graph(actualName, isDirected);
    subGraph->parentGraph = this;
    subGraphs.push_back(subGraph);
    return subGraph;
}

template<typename T>
Graph<T>* Graph<T>::getParentGraph() const {
    return parentGraph;
}

template<typename T>
std::vector<Graph<T>*> Graph<T>::getSubGraphs() const {
    return subGraphs;
}

template<typename T>
Graph<T>* Graph<T>::getSubGraph(const std::string& subGraphName) const {
    for (Graph* sub : subGraphs) {
        if (sub->getName() == subGraphName) {
            return sub;
        }
    }
    return nullptr;
}

template<typename T>
Graph<T>* Graph<T>::getSubGraphAt(size_t index) const {
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
bool Graph<T>::removeSubGraph(Graph* subGraph) {
    auto it = std::find(subGraphs.begin(), subGraphs.end(), subGraph);
    if (it == subGraphs.end()) {
        return false;
    }
    subGraphs.erase(it);
    delete subGraph;
    return true;
}

template<typename T>
bool Graph<T>::removeSubGraph(const std::string& subGraphName) {
    Graph* subGraph = getSubGraph(subGraphName);
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
    Graph* subGraph = subGraphs[index];
    subGraphs.erase(subGraphs.begin() + index);
    delete subGraph;
    return true;
}

template<typename T>
int Graph<T>::getDepth() const {
    int depth = 0;
    Graph* current = parentGraph;
    while (current) {
        depth++;
        current = current->parentGraph;
    }
    return depth;
}

template<typename T>
Graph<T>* Graph<T>::getRootGraph() {
    Graph* current = this;
    while (current->parentGraph) {
        current = current->parentGraph;
    }
    return current;
}

template<typename T>
const Graph<T>* Graph<T>::getRootGraph() const {
    const Graph* current = this;
    while (current->parentGraph) {
        current = current->parentGraph;
    }
    return current;
}

template<typename T>
bool Graph<T>::isDescendantOf(const Graph* ancestor) const {
    const Graph* current = parentGraph;
    while (current) {
        if (current == ancestor) {
            return true;
        }
        current = current->parentGraph;
    }
    return false;
}

template<typename T>
bool Graph<T>::isAncestorOf(const Graph* descendant) const {
    return descendant->isDescendantOf(this);
}

template<typename T>
bool Graph<T>::isRoot() const {
    return parentGraph == nullptr;
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
    for (auto sub : subGraphs) {
        delete sub;
    }
    subGraphs.clear();
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

template<typename T>
void Graph<T>::printHierarchy(std::ostream& os, int indent) const {
    std::string indentStr(indent * 2, ' ');
    os << indentStr << "Graph: " << name 
       << " (Nodes: " << nodes.size() << ", Edges: " << edges.size() 
       << ", Depth: " << getDepth() << ")" << std::endl;
    
    for (const Graph* sub : subGraphs) {
        sub->printHierarchy(os, indent + 1);
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

template class Graph<int>;
template class Graph<std::string>;

template std::ostream& operator<<(std::ostream& os, const Graph<int>& g);
template std::ostream& operator<<(std::ostream& os, const Graph<std::string>& g);

}
