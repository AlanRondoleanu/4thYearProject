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

    void addUnit(Targetable* t_unit);
    void removeUnit(Targetable* t_unit);
    void targetUnit(std::weak_ptr<Targetable> t_unit);

    const std::vector<Targetable*>& getUnits() const;
    bool containsUnit(const Units* unit) const;
    std::weak_ptr<Targetable> getTarget() { return targetedUnit; }
    int getTargetLastCellID() { return targetLastCellID; }
    void refreshLastCellID() { targetLastCellID = targetedUnit.lock()->getCellID(); }

private:
    std::vector<Targetable*> units;
    std::weak_ptr<Targetable> targetedUnit;
    int targetLastCellID;
};

#endif