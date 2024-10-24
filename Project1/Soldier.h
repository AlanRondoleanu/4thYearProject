#ifndef SOLDIER_H
#define SOLDIER_H
#include <SFML/Graphics.hpp>
#include "Units.h"

class Soldier : public Units
{
public:
	Soldier();
	
private:
	void initialize();
};
#endif

