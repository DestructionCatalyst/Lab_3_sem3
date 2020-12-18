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
};