#include "PartitionGrid.h"

PartitionGrid::PartitionGrid(int width, int height)
    : gridWidth(width), gridHeight(height)
{
    // Initialize offsets: same as before
    neighborOffsets =
    {
        -gridWidth - 1, -gridWidth, -gridWidth + 1,
        -1, 0, 1,
        gridWidth - 1, gridWidth, gridWidth + 1
    };
}

void PartitionGrid::clear()
{
    partitionMap.clear();
}

void PartitionGrid::registerUnit(Targetable* unit, int cellID)
{
    partitionMap[cellID].push_back(unit);
}

std::vector<Targetable*> PartitionGrid::getUnitsInCellAndNeighbors(int cellID) const
{
    std::vector<Targetable*> result;

    for (int offset : neighborOffsets)
    {
        int neighborCellID = cellID + offset;

        auto it = partitionMap.find(neighborCellID);
        if (it != partitionMap.end())
        {
            result.insert(result.end(), it->second.begin(), it->second.end());
        }
    }

    return result;
}
