#ifndef PARTITIONGRID_H
#define PARTITIONGRID_H

#include <unordered_map>
#include <vector>
#include "Targetable.h"

class PartitionGrid
{
public:
    PartitionGrid(int gridWidth, int gridHeight);

    void clear(); // Clears all partition cells (typically per frame)

    void registerUnit(Targetable* unit, int cellID);

    std::vector<Targetable*> getUnitsInCellAndNeighbors(int cellID) const;

private:
    int gridWidth;
    int gridHeight;

    std::unordered_map<int, std::vector<Targetable*>> partitionMap;

    std::vector<int> neighborOffsets; // Precomputed offsets for neighbor queries
};

#endif