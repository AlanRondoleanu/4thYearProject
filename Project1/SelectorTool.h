#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Mouse.h"
#include "UnitHandler.h"

class SelectorTool
{
public:
    SelectorTool();
    void render(sf::RenderWindow& t_window);
    void handleEvent(const sf::Event& event);
    void selectUnits();

private:

    bool isSelecting{ false };
    sf::Vector2f startPos;
    sf::Vector2f endPos;
    sf::RectangleShape selectionBox;

    void updateSelectionBox();
    void resetBounds();
};

