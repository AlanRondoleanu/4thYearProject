#include "Mouse.h"

void Mouse::updateHovering(const std::vector<std::shared_ptr<Targetable>>& t_enemies, const std::vector<std::shared_ptr<Targetable>>& t_player)
{
	for (const auto& enemy : t_enemies)
	{
		std::shared_ptr<Units> unit = std::dynamic_pointer_cast<Units>(enemy);
		if (unit->body.getGlobalBounds().contains(mousePosition))
		{
			hoveredUnit = unit.get();
			hoveredState = MouseHover::Enemy;
			return;
		}
	}

	for (const auto& player : t_player)
	{
		std::shared_ptr<Units> unit = std::dynamic_pointer_cast<Units>(player);
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

