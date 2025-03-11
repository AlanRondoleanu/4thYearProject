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
    void targetUnit(std::weak_ptr<Units> t_unit);

    const std::vector<Units*>& getUnits() const;
    bool containsUnit(const Units* unit) const;
    std::weak_ptr<Units> getTarget() { return targetedUnit; }
    int getTargetLastCellID() { return targetLastCellID; }
    void refreshLastCellID() { targetLastCellID = targetedUnit.lock()->cellID; }

private:
    std::vector<Units*> units;
    std::weak_ptr<Units> targetedUnit;
    int targetLastCellID;
};

#endif