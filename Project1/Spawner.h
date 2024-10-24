#ifndef SPAWNER_H
#define SPAWNER_H

#include "Buildings.h"

class Spawner : public Buildings
{
public:
	Spawner();
	void update();

private:
	void initialize();
};
#endif
