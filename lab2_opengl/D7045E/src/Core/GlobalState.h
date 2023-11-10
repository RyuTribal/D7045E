#pragma once
#include "pch.h"

namespace D7045E
{
    enum PointSet
    {
	    
    };

    class GlobalState
    {
    private:
        GlobalState() {
            std::copy(std::begin(DEFAULT_COLOR), std::end(DEFAULT_COLOR), std::begin(point_color));
        }

        GlobalState(const GlobalState&) = delete;
        GlobalState& operator=(const GlobalState&) = delete;

    public:
        static constexpr int BG_COLOR[3] = { 128, 128, 128 };
        static constexpr int DEFAULT_COLOR[3] = { 128, 128, 128 };
        static constexpr int EDGE_DEFAULT_COLOR[3] = { 0, 0, 0 };

        static constexpr int INITIAL_POINT_COUNT = 10;
        static constexpr int MAX_DELTA_TIME = 60;
        static constexpr int SPEED_FACTOR = 50;

    	std::vector<int> point_sets = {10, 50, 100};

		int choice = 0;

        int POINT_COUNT = INITIAL_POINT_COUNT; 
        int point_color[3];
        int previousTimestamp = 0; // For frame timing
        std::vector<int> triangleCoords;
        bool shouldDrawTriangles = false;

        static GlobalState& Instance()
        {
            static GlobalState instance; 
            return instance;
        }
	};
}