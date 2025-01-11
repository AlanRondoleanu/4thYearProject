#ifndef UNITGROUP_H
#define UNITGROUP_H

#include <vector>
#include <memory>
#include <algorithm>
#include "Units.h"

class UnitGroup 
{
public:
    UnitGroup() = default;

    void addUnit(Units* t_unit);
    void removeUnit(Units* t_unit);
    void targetUnit(Units* t_unit);

    const std::vector<Units*>& getUnits() const;
    bool containsUnit(const Units* unit) const;
    Units* getTarget() { return targetedUnit; }
    int getTargetLastCellID() { return targetLastCellID; }
    void refreshLastCellID() { targetLastCellID = targetedUnit->cellID; }

private:
    std::vector<Units*> units;
    Units* targetedUnit;
    int targetLastCellID;
};

#endif