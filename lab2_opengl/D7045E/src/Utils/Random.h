#pragma once
#include <random>


namespace D7045E
{
	class Random
	{
	public:
		static float RandomFloat(float begin, float end)
		{
			std::random_device rd;
			std::mt19937 mt(rd());

			std::uniform_real_distribution<float> dist(begin, end);

			return dist(mt);
		}
	};
}
