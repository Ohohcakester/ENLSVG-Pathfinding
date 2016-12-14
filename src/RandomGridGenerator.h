#ifndef _RANDOM_GRID_GENERATOR_H_
#define _RANDOM_GRID_GENERATOR_H_

class Grid;

class RandomGridGenerator {
public:
    static void generateRandomGrid(Grid& grid, const float percentBlocked);
    static void generateAutomataGrid(Grid& grid, const float percentBlocked, const int iterations, const float resolutionMultiplier);

private:
    static void runAutomataIterationBlockedBorders(const int resolution, Grid& grid, std::vector<std::vector<int>>& count);
};

#endif