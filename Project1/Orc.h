#ifndef ORC_H
#define ORC_H
#include <SFML/Graphics.hpp>
#include "Units.h"

class Orc : public Units
{
public:
	Orc(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement, AStarMovement t_astarMovement);

	std::string GetUnitType() const override {
		return "Orc";
	}

private:

};
#endif

