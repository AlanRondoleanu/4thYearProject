#ifndef NIGHTELF_H
#define NIGHTELF_H
#include <SFML/Graphics.hpp>
#include "Units.h"

class NightElf : public Units
{
public:
	NightElf(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement, AStarMovement t_astarMovement);

	std::string GetUnitType() const override {
		return "NightElf";
	}

private:

};
#endif

