#pragma once
#include <algorithm>
#include <vector>

class GlobalState
{
private:
    GlobalState() {
        
    }

    GlobalState(const GlobalState&) = delete;
    GlobalState& operator=(const GlobalState&) = delete;

public:
    static constexpr int BG_COLOR[3] = { 128, 128, 128 };
    static constexpr int EDGE_DEFAULT_COLOR[3] = { 0, 0, 0 };

    static constexpr int INITIAL_POINT_COUNT = 3;
    static constexpr int MAX_DELTA_TIME = 60;
    static constexpr int SPEED_FACTOR = 50;

    std::vector<int> point_sets = { 10, 50, 100 };

    int choice = 0;
    int color_type_choice = 0;
    float uniform_color[3] = { 141.f/255.f, 85.f/255.f, 36.f/255 };

    int POINT_COUNT = INITIAL_POINT_COUNT;
    std::vector<int> triangleCoords;
    bool shouldDrawTriangles = false;

    static GlobalState& Instance()
    {
        static GlobalState instance;
        return instance;
    }
};