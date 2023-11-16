#include "pch.h"
#include "Algorithms.h"

namespace Engine
{
	void Algorithms::CreateConvexHull(Ref<std::vector<Ref<Vertex>>> hull, Ref<std::vector<Ref<Vertex>>> vertices_in_hull, Ref<std::vector<Ref<Vertex>>> vertices, bool sort)
	{
		// Merge sort has the complexity O(n log n)

		CORE_ASSERT(vertices->size() >= 3, "Convex hull has to have at least 3 vertices!!");

		std::vector<Ref<Vertex>> upper_hull;
		std::vector<Ref<Vertex>> lower_hull;

		if (sort) {
			Sort::MergeSortArray(vertices->begin(), vertices->end(), [](Engine::Ref<Engine::Vertex> a, Engine::Ref<Engine::Vertex> b) {
				return a->x == b->x ? a->y < b->y : a->x < b->x;
				});
		}

		upper_hull.push_back(vertices->at(0));
		upper_hull.push_back(vertices->at(1));

		for(int i = 2; i < vertices->size(); i++)
		{
			upper_hull.push_back(vertices->at(i));
			while(upper_hull.size() > 2 && Cross(upper_hull[upper_hull.size() - 3].get(), upper_hull[upper_hull.size() - 2].get(), upper_hull[upper_hull.size() - 1].get()) <= 0)
			{
				if(vertices_in_hull)
				{
					vertices_in_hull->push_back(upper_hull[upper_hull.size() - 2]);
				}
				upper_hull.erase(upper_hull.end() - 2);
			}
		}

		lower_hull.push_back(vertices->at(vertices->size() - 1));
		lower_hull.push_back(vertices->at(vertices->size() - 2));

		for (int i = vertices->size() - 3; i >= 0; i--)
		{
			lower_hull.push_back(vertices->at(i));
			while (lower_hull.size() > 2 && Cross(lower_hull[lower_hull.size() - 3].get(), lower_hull[lower_hull.size() - 2].get(), lower_hull[lower_hull.size() - 1].get()) <= 0)
			{
				if (vertices_in_hull)
				{
					vertices_in_hull->push_back(lower_hull[lower_hull.size() - 2]);
				}
				lower_hull.erase(lower_hull.end() - 2);
			}
		}

		// So there are no duplicate vertices during combination of the lower and upper hull
		lower_hull.erase(lower_hull.begin());
		lower_hull.pop_back();
		for(auto vertex : upper_hull)
		{
			hull->push_back(vertex);
		}
		for (auto vertex : lower_hull)
		{
			hull->push_back(vertex);
		}
	}

}