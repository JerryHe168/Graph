#include <iostream>
#include <cassert>
#include <unordered_set>
#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif
#include "graph.hpp"
#include "algorithms.hpp"

using namespace graph::algo;

using namespace graph;

void testBasicNodeOperations() {
    std::cout << "===== 测试基本节点操作 =====" << std::endl;
    
    Graph<int> g("TestGraph", false);
    
    assert(g.isEmpty() == true);
    assert(g.getNodeCount() == 0);
    
    Node<int>* n1 = g.addNode(10);
    Node<int>* n2 = g.addNode(20);
    
    assert(g.isEmpty() == false);
    assert(g.getNodeCount() == 2);
    assert(n1->getId() == 0);
    assert(n2->getId() == 1);
    assert(n1->getData() == 10);
    assert(n2->getData() == 20);
    
    Node<int>* n3 = g.addNodeWithId(100, 100);
    assert(n3 != nullptr);
    assert(n3->getId() == 100);
    assert(n3->getData() == 100);
    assert(g.getNodeCount() == 3);
    
    Node<int>* duplicate = g.addNodeWithId(0, 999);
    assert(duplicate == nullptr);
    
    assert(g.containsNode(0) == true);
    assert(g.containsNode(999) == false);
    
    Node<int>* found = g.getNode(0);
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
    
    Node<std::string>* n1 = g.addNode("A");
    Node<std::string>* n2 = g.addNode("B");
    Node<std::string>* n3 = g.addNode("C");
    
    assert(g.getEdgeCount() == 0);
    
    Edge<std::string>* e1 = g.addEdge(n1, n2, 2.5);
    Edge<std::string>* e2 = g.addEdge(n2->getId(), n3->getId(), 3.0);
    
    assert(g.getEdgeCount() == 2);
    assert(e1->getWeight() == 2.5);
    assert(e2->getWeight() == 3.0);
    assert(e1->getFromNodeId() == n1->getId());
    assert(e1->getToNodeId() == n2->getId());
    
    assert(g.containsEdge(e1->getId()) == true);
    assert(g.containsEdge(999) == false);
    
    Edge<std::string>* found = g.getEdge(e1->getId());
    assert(found != nullptr);
    assert(found->getWeight() == 2.5);
    
    found->setWeight(5.0);
    assert(found->getWeight() == 5.0);
    
    assert(g.hasEdge(n1, n2) == true);
    assert(g.hasEdge(n1, n3) == false);
    
    Edge<std::string>* foundEdge = g.findEdge(n1, n2);
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
    
    Node<int>* n1 = g.addNode(1);
    Node<int>* n2 = g.addNode(2);
    Node<int>* n3 = g.addNode(3);
    
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
    
    std::vector<Node<int>*> neighbors = g.getNeighbors(n1);
    assert(neighbors.size() == 2);
    
    neighbors = g.getNeighbors(n2);
    assert(neighbors.size() == 1);
    
    std::cout << "有向图测试通过!" << std::endl;
}

void testUndirectedGraph() {
    std::cout << "\n===== 测试无向图 =====" << std::endl;
    
    Graph<int> g("UndirectedGraph", false);
    
    Node<int>* n1 = g.addNode(1);
    Node<int>* n2 = g.addNode(2);
    
    g.addEdge(n1, n2);
    
    assert(n1->getOutDegree() == 1);
    assert(n1->getInDegree() == 1);
    assert(n2->getOutDegree() == 1);
    assert(n2->getInDegree() == 1);
    
    assert(n1->getDegree() == 1);
    assert(n2->getDegree() == 1);
    
    assert(g.hasEdge(n1, n2) == true);
    assert(g.hasEdge(n2, n1) == true);
    
    Edge<int>* e = g.findEdge(n2, n1);
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
    
    Node<int>* n1 = g.addNode(1);
    Node<int>* n2 = g.addNode(2);
    Node<int>* n3 = g.addNode(3);
    
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
    
    Node<std::string>* n1 = g.addNode("A");
    Node<std::string>* n2 = g.addNode("B");
    Node<std::string>* n3 = g.addNode("C");
    
    g.addEdge(n1, n2);
    g.addEdge(n1, n3);
    g.addEdge(n2, n3);
    
    std::vector<Edge<std::string>*> outEdges = g.getEdgesFrom(n1->getId());
    assert(outEdges.size() == 2);
    
    std::vector<Edge<std::string>*> inEdges = g.getEdgesTo(n3->getId());
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
    Node<std::string>* n1 = original.addNode("A");
    Node<std::string>* n2 = original.addNode("B");
    original.addEdge(n1, n2, 3.5);
    original.createSubGraph("Sub");
    
    Graph<std::string> copy(original);
    
    assert(copy.getName() == "Original");
    assert(copy.isDirectedGraph() == true);
    assert(copy.getNodeCount() == 2);
    assert(copy.getEdgeCount() == 1);
    assert(copy.getSubGraphCount() == 1);
    
    Node<std::string>* copyN1 = copy.getNode(0);
    Node<std::string>* copyN2 = copy.getNode(1);
    assert(copyN1->getData() == "A");
    assert(copyN2->getData() == "B");
    
    Edge<std::string>* copyE = copy.findEdge(copyN1, copyN2);
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
    
    Node<int>* n0 = g.addNode(0);
    Node<int>* n1 = g.addNode(1);
    Node<int>* n2 = g.addNode(2);
    Node<int>* n3 = g.addNode(3);
    
    g.addEdge(n0, n1);
    g.addEdge(n0, n2);
    g.addEdge(n1, n3);
    g.addEdge(n2, n3);
    
    std::vector<int> neighbors = g.getNeighborIds(0);
    assert(neighbors.size() == 2);
    
    std::vector<Node<int>*> neighborNodes = g.getNeighbors(n0);
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
    
    Node<int>* n1 = g.addNode(1);
    Node<int>* n2 = g.addNode(2);
    
    assert(g.addEdge(nullptr, n2) == nullptr);
    assert(g.addEdge(n1, nullptr) == nullptr);
    
    Graph<int> other("Other", true);
    Node<int>* otherNode = other.addNode(999);
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

void testBFSAlgorithm() {
    std::cout << "\n===== 测试BFS广度优先搜索算法 =====" << std::endl;
    
    Graph<int> g("BFSTest", false);
    
    Node<int>* n0 = g.addNode(0);
    Node<int>* n1 = g.addNode(1);
    Node<int>* n2 = g.addNode(2);
    Node<int>* n3 = g.addNode(3);
    Node<int>* n4 = g.addNode(4);
    Node<int>* n5 = g.addNode(5);
    
    g.addEdge(n0, n1);
    g.addEdge(n0, n2);
    g.addEdge(n1, n3);
    g.addEdge(n1, n4);
    g.addEdge(n2, n5);
    
    std::vector<Node<int>*> bfsResult = bfs(g, n0);
    assert(bfsResult.size() == 6);
    
    assert(bfsResult[0]->getId() == 0);
    assert(bfsResult[1]->getId() == 1 || bfsResult[1]->getId() == 2);
    assert(bfsResult[2]->getId() == 1 || bfsResult[2]->getId() == 2);
    
    if (bfsResult[1]->getId() == 1) {
        assert(bfsResult[3]->getId() == 3 || bfsResult[3]->getId() == 4);
        assert(bfsResult[4]->getId() == 3 || bfsResult[4]->getId() == 4);
        assert(bfsResult[5]->getId() == 5);
    } else {
        assert(bfsResult[3]->getId() == 5);
        assert(bfsResult[4]->getId() == 3 || bfsResult[4]->getId() == 4);
        assert(bfsResult[5]->getId() == 3 || bfsResult[5]->getId() == 4);
    }
    
    std::vector<int> bfsIdsVec = bfsIds(g, 0);
    assert(bfsIdsVec.size() == 6);
    assert(bfsIdsVec[0] == 0);
    
    std::vector<Node<int>*> emptyResult = bfs(g, 999);
    assert(emptyResult.empty());
    
    std::vector<Node<int>*> nullResult = bfs(g, static_cast<Node<int>*>(nullptr));
    assert(nullResult.empty());
    
    Graph<int> directedG("DirectedBFSTest", true);
    Node<int>* d0 = directedG.addNode(0);
    Node<int>* d1 = directedG.addNode(1);
    Node<int>* d2 = directedG.addNode(2);
    directedG.addEdge(d0, d1);
    directedG.addEdge(d1, d2);
    
    std::vector<Node<int>*> directedBFS = bfs(directedG, d0);
    assert(directedBFS.size() == 3);
    assert(directedBFS[0]->getId() == 0);
    assert(directedBFS[1]->getId() == 1);
    assert(directedBFS[2]->getId() == 2);
    
    std::vector<Node<int>*> fromD2 = bfs(directedG, d2);
    assert(fromD2.size() == 1);
    assert(fromD2[0]->getId() == 2);
    
    std::cout << "BFS算法测试通过!" << std::endl;
}

void testDFSAlgorithm() {
    std::cout << "\n===== 测试DFS深度优先搜索算法 =====" << std::endl;
    
    Graph<int> g("DFSTest", false);
    
    Node<int>* n0 = g.addNode(0);
    Node<int>* n1 = g.addNode(1);
    Node<int>* n2 = g.addNode(2);
    Node<int>* n3 = g.addNode(3);
    Node<int>* n4 = g.addNode(4);
    Node<int>* n5 = g.addNode(5);
    
    g.addEdge(n0, n1);
    g.addEdge(n0, n2);
    g.addEdge(n1, n3);
    g.addEdge(n1, n4);
    g.addEdge(n2, n5);
    
    std::vector<Node<int>*> dfsResult = dfs(g, n0);
    assert(dfsResult.size() == 6);
    assert(dfsResult[0]->getId() == 0);
    
    std::vector<int> dfsIdsVec = dfsIds(g, 0);
    assert(dfsIdsVec.size() == 6);
    assert(dfsIdsVec[0] == 0);
    
    std::vector<Node<int>*> emptyResult = dfs(g, 999);
    assert(emptyResult.empty());
    
    std::vector<Node<int>*> nullResult = dfs(g, static_cast<Node<int>*>(nullptr));
    assert(nullResult.empty());
    
    Graph<int> directedG("DirectedDFSTest", true);
    Node<int>* d0 = directedG.addNode(0);
    Node<int>* d1 = directedG.addNode(1);
    Node<int>* d2 = directedG.addNode(2);
    Node<int>* d3 = directedG.addNode(3);
    directedG.addEdge(d0, d1);
    directedG.addEdge(d0, d2);
    directedG.addEdge(d1, d3);
    directedG.addEdge(d2, d3);
    
    std::vector<Node<int>*> directedDFS = dfs(directedG, d0);
    assert(directedDFS.size() == 4);
    assert(directedDFS[0]->getId() == 0);
    
    std::cout << "DFS算法(迭代版)测试通过!" << std::endl;
}

void testDFSRecursiveAlgorithm() {
    std::cout << "\n===== 测试DFS递归深度优先搜索算法 =====" << std::endl;
    
    Graph<int> g("DFSRecursiveTest", false);
    
    Node<int>* n0 = g.addNode(0);
    Node<int>* n1 = g.addNode(1);
    Node<int>* n2 = g.addNode(2);
    Node<int>* n3 = g.addNode(3);
    
    g.addEdge(n0, n1);
    g.addEdge(n0, n2);
    g.addEdge(n1, n3);
    
    std::vector<Node<int>*> dfsResult = dfsRecursive(g, n0);
    assert(dfsResult.size() == 4);
    assert(dfsResult[0]->getId() == 0);
    
    std::vector<Node<int>*> emptyResult = dfsRecursive(g, 999);
    assert(emptyResult.empty());
    
    std::vector<Node<int>*> nullResult = dfsRecursive(g, static_cast<Node<int>*>(nullptr));
    assert(nullResult.empty());
    
    Graph<int> linearG("LinearDFS", true);
    Node<int>* l0 = linearG.addNode(0);
    Node<int>* l1 = linearG.addNode(1);
    Node<int>* l2 = linearG.addNode(2);
    Node<int>* l3 = linearG.addNode(3);
    linearG.addEdge(l0, l1);
    linearG.addEdge(l1, l2);
    linearG.addEdge(l2, l3);
    
    std::vector<Node<int>*> linearDFS = dfsRecursive(linearG, l0);
    assert(linearDFS.size() == 4);
    assert(linearDFS[0]->getId() == 0);
    assert(linearDFS[1]->getId() == 1);
    assert(linearDFS[2]->getId() == 2);
    assert(linearDFS[3]->getId() == 3);
    
    std::cout << "DFS算法(递归版)测试通过!" << std::endl;
}

void testGraphTraversalEdgeCases() {
    std::cout << "\n===== 测试图遍历边界情况 =====" << std::endl;
    
    Graph<int> emptyG("EmptyGraph", false);
    assert(bfs(emptyG, 0).empty());
    assert(dfs(emptyG, 0).empty());
    assert(dfsRecursive(emptyG, 0).empty());
    
    Graph<int> singleNodeG("SingleNode", false);
    Node<int>* n = singleNodeG.addNode(42);
    
    std::vector<Node<int>*> bfsSingle = bfs(singleNodeG, n);
    assert(bfsSingle.size() == 1);
    assert(bfsSingle[0]->getId() == 0);
    
    std::vector<Node<int>*> dfsSingle = dfs(singleNodeG, n);
    assert(dfsSingle.size() == 1);
    assert(dfsSingle[0]->getId() == 0);
    
    std::vector<Node<int>*> dfsRecSingle = dfsRecursive(singleNodeG, n);
    assert(dfsRecSingle.size() == 1);
    assert(dfsRecSingle[0]->getId() == 0);
    
    Graph<int> disconnectedG("Disconnected", false);
    Node<int>* n0 = disconnectedG.addNode(0);
    Node<int>* n1 = disconnectedG.addNode(1);
    Node<int>* n2 = disconnectedG.addNode(2);
    disconnectedG.addEdge(n0, n1);
    
    std::vector<Node<int>*> bfsDisconnected = bfs(disconnectedG, n0);
    assert(bfsDisconnected.size() == 2);
    
    std::vector<Node<int>*> bfsFromN2 = bfs(disconnectedG, n2);
    assert(bfsFromN2.size() == 1);
    assert(bfsFromN2[0]->getId() == 2);
    
    std::cout << "图遍历边界情况测试通过!" << std::endl;
}

void testGraphTraversalOptimized() {
    std::cout << "\n===== 测试图遍历优化接口 =====" << std::endl;
    
    Graph<int> g("OptimizedTest", false);
    Node<int>* n0 = g.addNode(0);
    Node<int>* n1 = g.addNode(1);
    Node<int>* n2 = g.addNode(2);
    Node<int>* n3 = g.addNode(3);
    Node<int>* n4 = g.addNode(4);
    g.addEdge(n0, n1);
    g.addEdge(n0, n2);
    g.addEdge(n1, n3);
    g.addEdge(n2, n4);
    
    std::unordered_set<int> visited;
    
    std::vector<Node<int>*> bfsResult = bfs(g, n0, visited);
    assert(bfsResult.size() == 5);
    assert(visited.size() == 5);
    
    visited.clear();
    std::vector<Node<int>*> dfsResult = dfs(g, n0, visited);
    assert(dfsResult.size() == 5);
    assert(visited.size() == 5);
    
    visited.clear();
    std::vector<Node<int>*> dfsRecResult = dfsRecursive(g, n0, visited);
    assert(dfsRecResult.size() == 5);
    assert(visited.size() == 5);
    
    visited.clear();
    bfs(g, n1, visited);
    assert(visited.size() == 5);
    
    visited.clear();
    bfs(g, n2, visited);
    assert(visited.size() == 5);
    
    std::cout << "图遍历优化接口测试通过!" << std::endl;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_BINARY);
    _setmode(_fileno(stderr), _O_BINARY);
#endif
    
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
        testBFSAlgorithm();
        testDFSAlgorithm();
        testDFSRecursiveAlgorithm();
        testGraphTraversalEdgeCases();
        testGraphTraversalOptimized();
        
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
