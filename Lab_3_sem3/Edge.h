#pragma once

template<class T>
class Edge {
private:
	T endVertex;

	int length;

public:
	Edge(T endVertex, int length):
		endVertex(endVertex), length(length)
	{}

	T GetEnd()
	{
		return endVertex;
	}

	int GetLength()
	{
		return length;
	}

	void setLength(int newLength)
	{
		length = newLength;
	}
};