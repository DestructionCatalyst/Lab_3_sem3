#pragma once

template<class T>
class Edge {
private:
	T endVertex;

	int weight;

public:
	Edge(T endVertex, int length):
		endVertex(endVertex), weight(length)
	{}

	T GetEnd()
	{
		return endVertex;
	}

	int GetWeight()
	{
		return weight;
	}

	void SetWeight(int newLength)
	{
		weight = newLength;
	}

	template<class T1>
	friend std::ostream& operator<< (std::ostream& stream, Edge<T1>& graph);
};

template<class T1>
std::ostream& operator<<(std::ostream& stream, Edge<T1>& edge)
{
	if(edge.GetWeight() != 0)
		stream << "-[" << edge.GetWeight() << "]->(" << edge.GetEnd() << ")";

	return stream;
}
