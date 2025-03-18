#include "UnitGroup.h"

void UnitGroup::addUnit(Targetable* t_unit)
{
    units.push_back(t_unit);
}

void UnitGroup::removeUnit(Targetable* t_unit)
{
    auto it = std::remove(units.begin(), units.end(), t_unit);
    if (it != units.end()) 
    {
        units.erase(it);
    }
}

void UnitGroup::targetUnit(std::weak_ptr<Targetable> t_unit)
{
    targetedUnit = t_unit;
    targetLastCellID = t_unit.lock()->getCellID();
}

const std::vector<Targetable*>& UnitGroup::getUnits() const
{
    return units;
}

bool UnitGroup::containsUnit(const Units* unit) const 
{
    return std::find(units.begin(), units.end(), unit) != units.end();
}