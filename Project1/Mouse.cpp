#include "Mouse.h"

void Mouse::updateHovering(const std::vector<std::shared_ptr<Units>>& t_enemies, const std::vector<std::shared_ptr<Units>>& t_player)
{
	for (const auto& unit : t_enemies)
	{
		if (unit->body.getGlobalBounds().contains(mousePosition))
		{
			hoveredUnit = unit.get();
			hoveredState = MouseHover::Enemy;
			return;
		}
	}

	for (const auto& unit : t_player)
	{
		if (unit->body.getGlobalBounds().contains(mousePosition))
		{
			hoveredUnit = unit.get();
			hoveredState = MouseHover::Player;
			return;
		}
	}

	hoveredUnit = nullptr;
	hoveredState = MouseHover::Nothing;
}

Units* Mouse::getHoveredUnit()
{
	if (hoveredUnit != nullptr)
	{
		return hoveredUnit;
	}
	else {
		return nullptr;
	}
}

