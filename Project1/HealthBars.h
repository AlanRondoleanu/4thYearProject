#ifndef HEALTHBARS_H
#define HEALTHBARS_H

#include <SFML/Graphics.hpp>

class HealthBars 
{
public:
    HealthBars();

    void setPosition(sf::Vector2f position, float radius);
    void setHealth(float current, float max);

    void render(sf::RenderWindow& t_window);

private:
    sf::RectangleShape backgroundBar;
    sf::RectangleShape healthBar;

    float barWidth = 40;
    float barHeight = 5;

    float currentHealth;
    float maxHealth;
};

#endif