#pragma once

#include <utility>
#include <stdexcept>

#include "dependencies/Sequence.h"
#include "dependencies/ListSequence.h"

#include "Edge.h"

using namespace sequences;

using std::pair;

class vertex_not_found: public std::exception {
public:
	vertex_not_found(): std::exception()
	{}

	vertex_not_found(const char* message) : std::exception(message)
	{}
};

template<class T>
class AdjacencyList
{
public:
	typedef iterators::ListIterator<std::pair<T, int>> AdjacentEdgesIterator;
private:
	Sequence<std::pair<T, int>>* adjacent = new ListSequence<std::pair<T, int>>();
public:
	int SequenceSize() const
	{
		return adjacent->GetLength();
	}
	Edge GetEdge()
	{
		
	}
	int EdgeLength(T vertex) const
	{
		int vertexIndex = Find(vertex);

		if (vertexIndex == -1)
			throw vertex_not_found("No connection or vertex does not exist");

		return adjacent->Get(vertexIndex).second;
	}
	void SetAdjacent(T vertex, int distance)
	{
		int vertexIndex = Find(vertex);

		if (vertexIndex == -1)
			adjacent->Append(std::make_pair(vertex, distance));
		else
			adjacent->Set(std::make_pair(vertex, distance), vertexIndex);
	}
	void RemoveAdjacent(T vertex)
	{
		int vertexIndex = Find(vertex);

		if (vertexIndex == -1)
			throw vertex_not_found("No connection or vertex does not exist");

		dynamic_cast<ListSequence<std::pair<T, int>>*>(adjacent)->Remove(vertexIndex);
	}
	
	AdjacentEdgesIterator begin()
	{
		return *dynamic_cast<AdjacentEdgesIterator*>(adjacent->begin());
	}
	AdjacentEdgesIterator end()
	{
		return *dynamic_cast<AdjacentEdgesIterator*>(adjacent->end());
	}

private:
	int Find(T vertex)
	{
		AdjacentEdgesIterator iter = begin();

		for (int i = 0; (iter != end()) && (i < SequenceSize()); ++iter, ++i)
		{
			if ((*iter).first == vertex)
			{
				return i;
			}
		}
		return -1;

	}
};

