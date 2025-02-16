#ifndef SOLDIER_H
#define SOLDIER_H
#include <SFML/Graphics.hpp>
#include "Units.h"

class Soldier : public Units
{
public:
	Soldier(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement, AStarMovement t_astarMovement);
	
private:
	void initialize();
};
#endif

