#ifndef SPAWNER_H
#define SPAWNER_H

#include "Buildings.h"

class Spawner : public Buildings
{
public:
	Spawner(const sf::Vector2f& position, bool isEnemy);
	void update(float t_deltaTime) override;
	std::string GetUnitType() const override;
private:
};
#endif
