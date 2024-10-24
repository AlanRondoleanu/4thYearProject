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
	void place(sf::Vector2f t_position);
	void placementCollision(Buildings* t_buildings[]);

	sf::RectangleShape body;
	bool placed{ false };
	
	

	//Getters-Setters
	sf::Vector2f getPos() { return pos; }
	bool getBlocked() { return blocked; }

	void setPos(sf::Vector2f t_position) { pos = t_position; }
	void setEnemy(bool t_enemy);

private:
	virtual void initialize() = 0;
	bool enemy{ false };
	bool blocked{ false };

	sf::Vector2f pos{ -100, -100 };
	
};
#endif
