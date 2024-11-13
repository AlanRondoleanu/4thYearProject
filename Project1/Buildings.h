#ifndef BUILDINGS_H
#define BUILDINGS_H
#include <SFML/Graphics.hpp>

class Buildings
{
public:
	Buildings();
	~Buildings();

	static int PLAYER_BUILDING_AMOUNT;
	static int ENEMY_BUILDING_AMOUNT;

	virtual void update() = 0;
	void draw(sf::RenderWindow& t_window);
	void placementCollision(std::vector<Buildings*> t_buildings);

	sf::RectangleShape body;
	bool placed{ false };
	sf::Vector2f spawnPoint{ 0,0 };
	
	//Getters-Setters
	sf::Vector2f getPos() { return body.getPosition(); }
	bool getBlocked() { return blocked; }
	sf::Vector2f getSpawnPoint() { return spawnPoint; }

	void setPos(sf::Vector2f t_position);
	void setEnemy(bool t_enemy);

private:
	virtual void initialize() = 0;
	bool enemy{ false };
	bool blocked{ false };
};
#endif
