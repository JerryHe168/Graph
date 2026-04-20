#include <iostream>
#include <cassert>
#include "graph.h"

using namespace graph;

void testBasicNodeOperations() {
    std::cout << "===== 测试基本节点操作 =====" << std::endl;
    
    Graph<int> g("TestGraph", false);
    
    assert(g.isEmpty() == true);
    assert(g.getNodeCount() == 0);
    
    Graph<int>::Node* n1 = g.addNode(10);
    Graph<int>::Node* n2 = g.addNode(20);
    
    assert(g.isEmpty() == false);
    assert(g.getNodeCount() == 2);
    assert(n1->getId() == 0);
    assert(n2->getId() == 1);
    assert(n1->getData() == 10);
    assert(n2->getData() == 20);
    
    Graph<int>::Node* n3 = g.addNodeWithId(100, 100);
    assert(n3 != nullptr);
    assert(n3->getId() == 100);
    assert(n3->getData() == 100);
    assert(g.getNodeCount() == 3);
    
    Graph<int>::Node* duplicate = g.addNodeWithId(0, 999);
    assert(duplicate == nullptr);
    
    assert(g.containsNode(0) == true);
    assert(g.containsNode(999) == false);
    
    Graph<int>::Node* found = g.getNode(0);
    assert(found != nullptr);
    assert(found->getId() == 0);
    
    found->setData(999);
    assert(found->getData() == 999);
    
    assert(g.removeNode(0) == true);
    assert(g.getNodeCount() == 2);
    assert(g.containsNode(0) == false);
    
    assert(g.removeNode(999) == false);
    
    std::cout << "基本节点操作测试通过!" << std::endl;
}

void testBasicEdgeOperations() {
    std::cout << "\n===== 测试基本边操作 =====" << std::endl;
    
    Graph<std::string> g("EdgeTest", false);
    
    Graph<std::string>::Node* n1 = g.addNode("A");
    Graph<std::string>::Node* n2 = g.addNode("B");
    Graph<std::string>::Node* n3 = g.addNode("C");
    
    assert(g.getEdgeCount() == 0);
    
    Graph<std::string>::Edge* e1 = g.addEdge(n1, n2, 2.5);
    Graph<std::string>::Edge* e2 = g.addEdge(n2->getId(), n3->getId(), 3.0);
    
    assert(g.getEdgeCount() == 2);
    assert(e1->getWeight() == 2.5);
    assert(e2->getWeight() == 3.0);
    assert(e1->getFromNodeId() == n1->getId());
    assert(e1->getToNodeId() == n2->getId());
    
    assert(g.containsEdge(e1->getId()) == true);
    assert(g.containsEdge(999) == false);
    
    Graph<std::string>::Edge* found = g.getEdge(e1->getId());
    assert(found != nullptr);
    assert(found->getWeight() == 2.5);
    
    found->setWeight(5.0);
    assert(found->getWeight() == 5.0);
    
    assert(g.hasEdge(n1, n2) == true);
    assert(g.hasEdge(n1, n3) == false);
    
    Graph<std::string>::Edge* foundEdge = g.findEdge(n1, n2);
    assert(foundEdge != nullptr);
    assert(foundEdge->getId() == e1->getId());
    
    assert(g.removeEdge(e1->getId()) == true);
    assert(g.getEdgeCount() == 1);
    assert(g.containsEdge(e1->getId()) == false);
    
    assert(g.removeEdge(999) == false);
    
    std::cout << "基本边操作测试通过!" << std::endl;
}

void testDirectedGraph() {
    std::cout << "\n===== 测试有向图 =====" << std::endl;
    
    Graph<int> g("DirectedGraph", true);
    
    Graph<int>::Node* n1 = g.addNode(1);
    Graph<int>::Node* n2 = g.addNode(2);
    Graph<int>::Node* n3 = g.addNode(3);
    
    g.addEdge(n1, n2);
    g.addEdge(n2, n3);
    g.addEdge(n1, n3);
    
    assert(n1->getOutDegree() == 2);
    assert(n1->getInDegree() == 0);
    assert(n2->getOutDegree() == 1);
    assert(n2->getInDegree() == 1);
    assert(n3->getOutDegree() == 0);
    assert(n3->getInDegree() == 2);
    
    assert(g.hasEdge(n1, n2) == true);
    assert(g.hasEdge(n2, n1) == false);
    
    std::vector<Graph<int>::Node*> neighbors = g.getNeighbors(n1);
    assert(neighbors.size() == 2);
    
    neighbors = g.getNeighbors(n2);
    assert(neighbors.size() == 1);
    
    std::cout << "有向图测试通过!" << std::endl;
}

void testUndirectedGraph() {
    std::cout << "\n===== 测试无向图 =====" << std::endl;
    
    Graph<int> g("UndirectedGraph", false);
    
    Graph<int>::Node* n1 = g.addNode(1);
    Graph<int>::Node* n2 = g.addNode(2);
    
    g.addEdge(n1, n2);
    
    assert(n1->getOutDegree() == 1);
    assert(n1->getInDegree() == 1);
    assert(n2->getOutDegree() == 1);
    assert(n2->getInDegree() == 1);
    
    assert(n1->getDegree() == 1);
    assert(n2->getDegree() == 1);
    
    assert(g.hasEdge(n1, n2) == true);
    assert(g.hasEdge(n2, n1) == true);
    
    Graph<int>::Edge* e = g.findEdge(n2, n1);
    assert(e != nullptr);
    
    std::cout << "无向图测试通过!" << std::endl;
}

void testHierarchicalStructure() {
    std::cout << "\n===== 测试层次结构和父子图关系 =====" << std::endl;
    
    Graph<int> root("Root", false);
    
    assert(root.isRoot() == true);
    assert(root.isLeaf() == true);
    assert(root.getDepth() == 0);
    assert(root.getParentGraph() == nullptr);
    assert(root.getSubGraphCount() == 0);
    
    Graph<int>* sub1 = root.createSubGraph("SubGraph1");
    Graph<int>* sub2 = root.createSubGraph("SubGraph2");
    
    assert(root.getSubGraphCount() == 2);
    assert(root.isLeaf() == false);
    assert(sub1->isRoot() == false);
    assert(sub1->getDepth() == 1);
    assert(sub1->getParentGraph() == &root);
    
    Graph<int>* sub1_1 = sub1->createSubGraph("SubGraph1_1");
    
    assert(sub1_1->getDepth() == 2);
    assert(sub1_1->getRootGraph() == &root);
    assert(root.getRootGraph() == &root);
    
    assert(sub1_1->isDescendantOf(&root) == true);
    assert(root.isAncestorOf(sub1_1) == true);
    assert(sub1_1->isDescendantOf(sub2) == false);
    
    Graph<int>* found = root.getSubGraph("SubGraph1");
    assert(found == sub1);
    
    found = root.getSubGraphAt(1);
    assert(found == sub2);
    
    assert(root.removeSubGraph("SubGraph2") == true);
    assert(root.getSubGraphCount() == 1);
    
    std::cout << "层次结构测试通过!" << std::endl;
}

void testNodeRemovalCleansEdges() {
    std::cout << "\n===== 测试节点删除时边的清理 =====" << std::endl;
    
    Graph<int> g("CleanupTest", false);
    
    Graph<int>::Node* n1 = g.addNode(1);
    Graph<int>::Node* n2 = g.addNode(2);
    Graph<int>::Node* n3 = g.addNode(3);
    
    g.addEdge(n1, n2);
    g.addEdge(n2, n3);
    g.addEdge(n1, n3);
    
    assert(g.getNodeCount() == 3);
    assert(g.getEdgeCount() == 3);
    
    g.removeNode(n2);
    
    assert(g.getNodeCount() == 2);
    assert(g.getEdgeCount() == 1);
    
    assert(n1->getDegree() == 1);
    assert(n3->getDegree() == 1);
    
    std::cout << "边清理测试通过!" << std::endl;
}

void testGetEdgesFromTo() {
    std::cout << "\n===== 测试获取出边和入边 =====" << std::endl;
    
    Graph<std::string> g("EdgeQueryTest", true);
    
    Graph<std::string>::Node* n1 = g.addNode("A");
    Graph<std::string>::Node* n2 = g.addNode("B");
    Graph<std::string>::Node* n3 = g.addNode("C");
    
    g.addEdge(n1, n2);
    g.addEdge(n1, n3);
    g.addEdge(n2, n3);
    
    std::vector<Graph<std::string>::Edge*> outEdges = g.getEdgesFrom(n1->getId());
    assert(outEdges.size() == 2);
    
    std::vector<Graph<std::string>::Edge*> inEdges = g.getEdgesTo(n3->getId());
    assert(inEdges.size() == 2);
    
    inEdges = g.getEdgesTo(n1->getId());
    assert(inEdges.size() == 0);
    
    std::cout << "出边入边查询测试通过!" << std::endl;
}

void testClearOperations() {
    std::cout << "\n===== 测试清空操作 =====" << std::endl;
    
    Graph<int> g("ClearTest", false);
    
    g.addNode(1);
    g.addNode(2);
    g.addEdge(0, 1);
    g.createSubGraph("Sub1");
    
    assert(g.getNodeCount() == 2);
    assert(g.getEdgeCount() == 1);
    assert(g.getSubGraphCount() == 1);
    
    g.clear();
    
    assert(g.getNodeCount() == 0);
    assert(g.getEdgeCount() == 0);
    assert(g.getSubGraphCount() == 1);
    
    g.clearAll();
    
    assert(g.getNodeCount() == 0);
    assert(g.getEdgeCount() == 0);
    assert(g.getSubGraphCount() == 0);
    
    std::cout << "清空操作测试通过!" << std::endl;
}

void testCopyOperations() {
    std::cout << "\n===== 测试拷贝操作 =====" << std::endl;
    
    Graph<std::string> original("Original", true);
    Graph<std::string>::Node* n1 = original.addNode("A");
    Graph<std::string>::Node* n2 = original.addNode("B");
    original.addEdge(n1, n2, 3.5);
    original.createSubGraph("Sub");
    
    Graph<std::string> copy(original);
    
    assert(copy.getName() == "Original");
    assert(copy.isDirectedGraph() == true);
    assert(copy.getNodeCount() == 2);
    assert(copy.getEdgeCount() == 1);
    assert(copy.getSubGraphCount() == 1);
    
    Graph<std::string>::Node* copyN1 = copy.getNode(0);
    Graph<std::string>::Node* copyN2 = copy.getNode(1);
    assert(copyN1->getData() == "A");
    assert(copyN2->getData() == "B");
    
    Graph<std::string>::Edge* copyE = copy.findEdge(copyN1, copyN2);
    assert(copyE != nullptr);
    assert(copyE->getWeight() == 3.5);
    
    Graph<std::string> assigned("Assigned", false);
    assigned = original;
    
    assert(assigned.getName() == "Original");
    assert(assigned.isDirectedGraph() == true);
    assert(assigned.getNodeCount() == 2);
    assert(assigned.getEdgeCount() == 1);
    
    std::cout << "拷贝操作测试通过!" << std::endl;
}

void testPrintFunctions() {
    std::cout << "\n===== 测试打印函数 =====" << std::endl;
    
    Graph<int> g("PrintTest", false);
    g.addNode(10);
    g.addNode(20);
    g.addEdge(0, 1);
    
    Graph<int>* sub = g.createSubGraph("SubPrint");
    sub->addNode(100);
    
    std::cout << "\n--- print() ---" << std::endl;
    g.print();
    
    std::cout << "\n--- operator<< ---" << std::endl;
    std::cout << g;
    
    std::cout << "\n--- printHierarchy() ---" << std::endl;
    g.printHierarchy();
    
    std::cout << "打印函数测试通过!" << std::endl;
}

void testNeighborQueries() {
    std::cout << "\n===== 测试邻居查询 =====" << std::endl;
    
    Graph<int> g("NeighborTest", false);
    
    Graph<int>::Node* n0 = g.addNode(0);
    Graph<int>::Node* n1 = g.addNode(1);
    Graph<int>::Node* n2 = g.addNode(2);
    Graph<int>::Node* n3 = g.addNode(3);
    
    g.addEdge(n0, n1);
    g.addEdge(n0, n2);
    g.addEdge(n1, n3);
    g.addEdge(n2, n3);
    
    std::vector<int> neighbors = g.getNeighborIds(0);
    assert(neighbors.size() == 2);
    
    std::vector<Graph<int>::Node*> neighborNodes = g.getNeighbors(n0);
    assert(neighborNodes.size() == 2);
    
    neighbors = g.getNeighborIds(999);
    assert(neighbors.size() == 0);
    
    neighborNodes = g.getNeighbors(nullptr);
    assert(neighborNodes.size() == 0);
    
    std::cout << "邻居查询测试通过!" << std::endl;
}

void testEdgeCases() {
    std::cout << "\n===== 测试边界情况 =====" << std::endl;
    
    Graph<int> g("EdgeCaseTest", true);
    
    assert(g.addEdge(0, 1) == nullptr);
    
    Graph<int>::Node* n1 = g.addNode(1);
    Graph<int>::Node* n2 = g.addNode(2);
    
    assert(g.addEdge(nullptr, n2) == nullptr);
    assert(g.addEdge(n1, nullptr) == nullptr);
    
    Graph<int> other("Other", true);
    Graph<int>::Node* otherNode = other.addNode(999);
    assert(g.addEdge(otherNode, n1) == nullptr);
    
    assert(g.removeNode(nullptr) == false);
    assert(g.removeNode(otherNode) == false);
    
    assert(g.findEdge(nullptr, n1) == nullptr);
    assert(g.hasEdge(nullptr, n1) == false);
    
    Graph<int>* sub = g.createSubGraph();
    assert(sub != nullptr);
    assert(!sub->getName().empty());
    
    assert(g.getSubGraphAt(999) == nullptr);
    assert(g.removeSubGraphAt(999) == false);
    
    std::cout << "边界情况测试通过!" << std::endl;
}

int main() {
    std::cout << "======================================" << std::endl;
    std::cout << "   C++ 图论数据结构库测试程序" << std::endl;
    std::cout << "======================================" << std::endl;
    
    try {
        testBasicNodeOperations();
        testBasicEdgeOperations();
        testDirectedGraph();
        testUndirectedGraph();
        testHierarchicalStructure();
        testNodeRemovalCleansEdges();
        testGetEdgesFromTo();
        testClearOperations();
        testCopyOperations();
        testPrintFunctions();
        testNeighborQueries();
        testEdgeCases();
        
        std::cout << "\n======================================" << std::endl;
        std::cout << "   所有测试通过!" << std::endl;
        std::cout << "======================================" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n测试失败: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n测试失败: 未知异常" << std::endl;
        return 1;
    }
}
