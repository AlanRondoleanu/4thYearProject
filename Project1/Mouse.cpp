#include "Mouse.h"

bool Mouse::isHoveringEnemy(const std::vector<std::shared_ptr<Units>>& t_enemies)
{
	for (const auto& unit : t_enemies)
	{
		if (unit->body.getGlobalBounds().contains(mousePosition))
		{
			hoveredUnit = unit.get();
			return true;
		}
		else {
			return false;
		}
	}
}

