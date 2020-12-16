#include <iostream>

#include "dependencies/TestEnvironment.h"

#include "dependencies/HashMap.h"

#include "AdjacencyList.h"
#include "Graph.h"
#include "GraphFactory.h"

using namespace dictionary;

void testAdjacencyList()
{
    AdjacencyList<int>* adjacent = new AdjacencyList<int>();

    adjacent->SetAdjacent(1, 2);
    adjacent->SetAdjacent(2, 4);
    adjacent->SetAdjacent(3, 8);
    adjacent->SetAdjacent(4, 16);
    adjacent->SetAdjacent(5, 32);

    ASSERT_EQUALS(adjacent->EdgeLength(3), 8);

    adjacent->SetAdjacent(4, 8);

    ASSERT_EQUALS(adjacent->EdgeLength(4), 8);

    adjacent->RemoveAdjacent(2);

    ASSERT_THROWS(adjacent->EdgeLength(2), vertex_not_found);

    ASSERT_EQUALS(adjacent->SequenceSize(), 4);
}

void basicGraphTest()
{
    Graph<int>* g = new Graph<int>([](int a, int b)->int {return a % b;});

    ASSERT_THROWS(g->EdgeLength(1, 2), vertex_not_found);

    g->AddVertex(1);
    g->AddVertex(2);
    g->AddVertex(3);

    g->SetBidirectionalEdge(1, 2, 4);
    g->SetAdjacent(2, 3, 8);
    g->SetAdjacent(1, 3, 20);

    ASSERT_EQUALS(g->AdjacentCount(2), 2);
    ASSERT_EQUALS(g->EdgeLength(1, 3), 20);
    TestEnvironment::Assert(g->AreConnected(1, 3));

    g->RemoveAdjacent(2, 3);
    ASSERT_THROWS(g->EdgeLength(2, 3), vertex_not_found);
    TestEnvironment::Assert(!g->AreConnected(2, 3));

    g->RemoveVertex(3);

    //std::cout << *dynamic_cast<HashMap<int, AdjacencyList<int>*>*>(g->vertices) << std::endl;
    ASSERT_THROWS(g->EdgeLength(1, 3), vertex_not_found);

    ASSERT_EQUALS(g->EdgeLength(1, 1), 0);


}

void topologyGenerationTest()
{
    Graph<int>* k7 = IntegerGraphFactory::Complete(7);

    for(int i = 0; i < 7; i++)
        for(int j = 0; j < 7; j++)
            if (i != j) 
                TestEnvironment::Assert(k7->AreConnected(i, j));
            
    Graph<int>* p10 = IntegerGraphFactory::Chain(10, 1, Direction::BACKWARDS);

    
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (i == j + 1)
                TestEnvironment::Assert(p10->AreConnected(i, j));
            else
                TestEnvironment::Assert(!p10->AreConnected(i, j));
                
    Graph<int>* c10 = IntegerGraphFactory::Cycle(10, 1, Direction::CLOCKWISE);

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if ((j == i + 1) || (i == 9 && j == 0))
                TestEnvironment::Assert(c10->AreConnected(i, j));
            else
                TestEnvironment::Assert(!c10->AreConnected(i, j));
    
    Graph<int>* w5 = IntegerGraphFactory::Wheel(5, 1, Direction::BIDIRECTIONAL, Direction::TO_CENTER);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if ((j == i + 1) || (i == 3 && j == 0)) 
            {
                TestEnvironment::Assert(w5->AreConnected(i, j));
                TestEnvironment::Assert(w5->AreConnected(j, i));
            }
            
    for(int i = 0; i < 4; i++)
        TestEnvironment::Assert(w5->AreConnected(i, 4));
}

int main()
{
    TestEnvironment* env = new TestEnvironment();

    ADD_NEW_TEST(*env, "AdjacencyList test", testAdjacencyList);
    ADD_NEW_TEST(*env, "Basic graph test", basicGraphTest);
    ADD_NEW_TEST(*env, "Topology generation test", topologyGenerationTest);

    env->RunAll();

}

