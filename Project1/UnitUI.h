#ifndef UNITUI_H
#define UNITUI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <unordered_set>
#include "Units.h"

class UnitUI 
{
public:
    UnitUI(sf::RenderWindow& window);
    void SetSelectedUnit(std::shared_ptr<Units> unit);
    void Render(sf::RenderWindow& window, sf::View& uiView) const;
    void HandleClick(sf::Vector2f mousePos);
    bool isInsideUI(sf::Vector2f mousePos);

private:
    sf::RectangleShape panel;
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonLabels;
    sf::Font font;
    std::shared_ptr<Units> selectedUnit;
    sf::Vector2f uiScale;
};

#endif