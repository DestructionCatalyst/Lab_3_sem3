#include <iostream>

#include "dependencies/TestEnvironment.h"
#include "dependencies/SequenceAssertions.h"
#include "dependencies/HashMap.h"


#include "AdjacencyList.h"
#include "Graph.h"
#include "GraphFactory.h"
#include "GraphPathfinder.h"
#include "MaxStreamFinder.h"

using namespace dictionary;

using std::cin;
using std::cout;
using std::endl;

#define INPUT_INCORRECT_IF(CONDITION) if(CONDITION) command = -1; break;

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
    
    Graph<int>* w5 = IntegerGraphFactory::Wheel(5, 1, 1, Direction::BIDIRECTIONAL, Direction::TO_CENTER);

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

void testDijkstra() {
    Graph<int>* g = IntegerGraphFactory::Cycle(10);

    DijkstraPathfinder<int> p = DijkstraPathfinder<int>(g, 0);

    p.Dijkstra();

    p.GetPath(4);

    Graph<int>* g1 = IntegerGraphFactory::Empty(11);

    g1->SetBidirectionalEdge(0, 1, 1);
    g1->SetBidirectionalEdge(2, 8, 3);
    g1->SetBidirectionalEdge(2, 3, 10);
    g1->SetBidirectionalEdge(2, 7, 16);
    g1->SetBidirectionalEdge(3, 5, 7);
    g1->SetBidirectionalEdge(5, 10, 5);
    g1->SetBidirectionalEdge(4, 5, 6);
    g1->SetBidirectionalEdge(4, 6, 1);
    g1->SetBidirectionalEdge(5, 6, 9);
    g1->SetBidirectionalEdge(7, 10, 2);

    g1->SetAdjacent(0, 8, 3);
    g1->SetAdjacent(1, 9, 7);
    g1->SetAdjacent(1, 3, 10);
    g1->SetAdjacent(9, 10, 11);
    g1->SetAdjacent(3, 10, 9);
    g1->SetAdjacent(2, 6, 5);
    g1->SetAdjacent(6, 7, 1);

    DijkstraPathfinder<int>* p1 = new DijkstraPathfinder<int>(g1, 0);

    AssertSequenceEquals({ 0, 8, 2, 6, 7, 10 }, p1->GetPath(10));
    ASSERT_EQUALS(14, p1->GetDistance(10))

}

void testEdmondsKarp()
{
    Graph<int>* g = IntegerGraphFactory::Wheel(6, 2, 1, Direction::CLOCKWISE);

    EdmondsKarpStreamFinder<int>* f = new EdmondsKarpStreamFinder<int>(g, 0, 3);

    ASSERT_EQUALS(f->FindStream(), 3);

    Graph<int>* g1 = IntegerGraphFactory::Empty(9);

    g1->SetBidirectionalEdge(2, 3, 1);
    g1->SetBidirectionalEdge(3, 4, 3);
    g1->SetBidirectionalEdge(4, 5, 2);
    g1->SetBidirectionalEdge(4, 6, 9);
    g1->SetBidirectionalEdge(5, 6, 5);

    g1->SetAdjacent(0, 1, 5);
    g1->SetAdjacent(0, 5, 11);
    g1->SetAdjacent(1, 2, 2);
    g1->SetAdjacent(1, 3, 1);
    g1->SetAdjacent(2, 7, 4);
    g1->SetAdjacent(3, 7, 3);
    g1->SetAdjacent(6, 8, 4);
    g1->SetAdjacent(7, 8, 12);

    EdmondsKarpStreamFinder<int>* f1 = new EdmondsKarpStreamFinder<int>(g1, 0, 8);

    ASSERT_EQUALS(f1->FindStream(), 10);
}

void inputNumber(int* number)
{
    try
    {
        std::cin >> *number;
    }
    catch (...)
    {
        cin.clear();
        cin.ignore(32767, '\n');
        cout << "Incorrect input!" << endl;
        *number = -1;
    }
}

int inputNumberInRange(int lowerBound = 0, int upperBound = INT_MAX)
{
    int number;

    do
    {
        inputNumber(&number);

        if (number < lowerBound || number > upperBound)
            std::cout << "Incorrect input, try again\n";
    } while (number < lowerBound || number > upperBound);

    return number;
}

int inputEdgeWeight(const char* message = "Input edge weight")
{
    cout << message << '\n';

    return inputNumberInRange();
}

Direction inputEdgeDirection(
    const char* message1 = "forwards",
    const char* message2 = "backwards",
    const char* message = "Input edges direction"
    )
{
    cout << message << '\n';
    cout << "1 - " << message1 << '\n';
    cout << "2 - " << message2 << '\n';
    cout << "3 - bidirectional" << '\n';

    int dir = inputNumberInRange(1, 3);

    return static_cast<Direction>(dir - 1);
}

Graph<int>* createGraphWithTopology(int topology, int size)
{
    int vertexCount = 0;
    int weight = 0;

    Direction direction = Direction::BIDIRECTIONAL;

    int sideWeight = 0;
    int radiusWeight = 0;

    Direction sideDirection = Direction::BIDIRECTIONAL;
    Direction radiusDirection = Direction::BIDIRECTIONAL;

    switch (topology)
    {
    case 1:
        cout << "Input vertex count:\n";

        vertexCount = inputNumberInRange();

        return IntegerGraphFactory::Empty(vertexCount);
    case 2:
        cout << "Input vertex count:\n";

        vertexCount = inputNumberInRange(1);

        weight = inputEdgeWeight();

        return IntegerGraphFactory::Complete(vertexCount, weight);
    case 3:
        cout << "Input vertex count:\n";

        vertexCount = inputNumberInRange(2);

        weight = inputEdgeWeight();

        direction = inputEdgeDirection();

        return IntegerGraphFactory::Chain(vertexCount, weight, direction);
    case 4:
        cout << "Input vertex count:\n";

        vertexCount = inputNumberInRange(3);

        weight = inputEdgeWeight();

        direction = inputEdgeDirection("clockwise", "counter-clockwise");

        return IntegerGraphFactory::Cycle(vertexCount, weight, direction);
    case 5:
        cout << "Input vertex count:\n";

        vertexCount = inputNumberInRange(4);

        sideWeight = inputEdgeWeight("Input side weight");
        radiusWeight = inputEdgeWeight("Input radius weight");

        sideDirection = inputEdgeDirection("clockwise", "counter-clockwise", "Input sides direction");
        radiusDirection = inputEdgeDirection("to center", "from center", "Input radius direction");

        return IntegerGraphFactory::Wheel(vertexCount, sideWeight, radiusWeight, sideDirection, radiusDirection);
    default:
        return nullptr;
        break;
    }
}

int main()
{
    int command = -1;
    Graph<int>* graph = nullptr;

    while (command != 0)
    {
        //header();
        int vertexCount = 0;
        int topology = 0;

        int edgeStart = 0;
        int edgeEnd = 0;

        inputNumber(&command);

        switch (command)
        {
        case 1:
            if (graph != nullptr) {
                delete(graph);
                graph = nullptr;
            }

            cout << "Select topology:\n";
            cout << "1 - empty graph\n";
            cout << "2 - complete graph\n";
            cout << "3 - chain graph\n";
            cout << "4 - cycle graph\n";
            cout << "5 - wheel graph\n";

            topology = inputNumberInRange(1, 5);

            graph = createGraphWithTopology(topology, vertexCount);

            cout << "Graph successfully created\n";
            break;
        case 2:
            if (graph != nullptr) {
                graph->AddVertex(graph->VertexCount());
                cout << "Added vertex with index " << graph->VertexCount() - 1 << '\n';
            }
            else
                cout << "Create graph first!\n";

            break;
        case 3:
            if (graph != nullptr) {
                cout << "Input edge start\n";
                edgeStart = inputNumberInRange(0, graph->VertexCount() - 1);

                cout << "Input edge end\n";
                edgeEnd = inputNumberInRange(0, graph->VertexCount() - 1);

                cout << 
            }
            else
                cout << "Create graph first!\n";
            break;
        case 9:
            if (graph != nullptr)
                cout << *graph << endl;
            else
                cout << "Create graph first!\n";

            break;
        default:
            cin.clear();
            cin.ignore(32767, '\n');
            //cout << "Incorrect input!" << endl;
        }
    }

    TestEnvironment* env = new TestEnvironment();

    ADD_NEW_TEST(*env, "AdjacencyList test", testAdjacencyList);
    ADD_NEW_TEST(*env, "Basic graph test", basicGraphTest);
    ADD_NEW_TEST(*env, "Topology generation test", topologyGenerationTest);
    ADD_NEW_TEST(*env, "Dijkstra test", testDijkstra);
    ADD_NEW_TEST(*env, "Edmonds-Karp algorithm test", testEdmondsKarp);

    env->RunAll();

}

