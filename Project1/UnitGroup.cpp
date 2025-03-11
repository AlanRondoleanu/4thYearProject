#include "UnitGroup.h"

void UnitGroup::addUnit(Units* t_unit)
{
    units.push_back(t_unit);
}

void UnitGroup::removeUnit(Units* t_unit)
{
    auto it = std::remove(units.begin(), units.end(), t_unit);
    if (it != units.end()) 
    {
        units.erase(it);
    }
}

void UnitGroup::targetUnit(std::weak_ptr<Units> t_unit)
{
    targetedUnit = t_unit;
    targetLastCellID = t_unit.lock()->cellID;
}

const std::vector<Units*>& UnitGroup::getUnits() const
{
    return units;
}

bool UnitGroup::containsUnit(const Units* unit) const 
{
    return std::find(units.begin(), units.end(), unit) != units.end();
}