#include <iostream>

#include "dependencies/TestEnvironment.h"

#include "dependencies/HashMap.h"

#include "AdjacencyList.h"
#include "Graph.h"

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

    g->RemoveAdjacent(2, 3);
    ASSERT_THROWS(g->EdgeLength(2, 3), vertex_not_found);

    g->RemoveVertex(3);

    //std::cout << *dynamic_cast<HashMap<int, AdjacencyList<int>*>*>(g->vertices) << std::endl;
    ASSERT_THROWS(g->EdgeLength(1, 3), vertex_not_found);

    ASSERT_EQUALS(g->EdgeLength(1, 1), 0);


}

int main()
{
    TestEnvironment* env = new TestEnvironment();

    ADD_NEW_TEST(*env, "AdjacencyList test", testAdjacencyList);
    ADD_NEW_TEST(*env, "Basic graph test", basicGraphTest);

    env->RunAll();

}

