#pragma once
#include <SFML/Graphics.hpp>
#include "Units.h"

enum class MouseHover {
    Nothing,
    Enemy,
    Player
};

class Mouse
{
public:
    // Delete copy constructor and assignment operator to enforce singleton
    Mouse(const Mouse&) = delete;
    Mouse& operator=(const Mouse&) = delete;

    // Get the singleton instance
    static Mouse& getInstance() 
    {
        static Mouse instance;
        return instance;
    }

    sf::Vector2f getPosition() const 
    {
        return mousePosition;
    }

    sf::Vector2f getPositionOnScreen() const { return mousePositionOnScreen; }

    sf::Vector2f getPositionWithGrid()
    {
        sf::Vector2f mousePosition_Gridded = mousePosition;
        mousePosition_Gridded.x = (static_cast<int>(mousePosition_Gridded.x) / 50) * 50 + 25;
        mousePosition_Gridded.y = (static_cast<int>(mousePosition_Gridded.y) / 50) * 50 + 25;
        return mousePosition_Gridded;
    }

    void UpdateMousePostion(const sf::RenderWindow& t_window, sf::View t_camera) 
    {
        sf::Vector2i mouseTemp = sf::Mouse::getPosition(t_window);
        mousePositionOnScreen = sf::Vector2f(static_cast<float>(mouseTemp.x), static_cast<float>(mouseTemp.y));
        mousePosition = t_window.mapPixelToCoords(mouseTemp, t_camera);
    }
    void updateHovering(const std::vector<std::shared_ptr<Targetable>>& t_enemies, const std::vector<std::shared_ptr<Targetable>>& t_player);

    Units* getHoveredUnit();
    MouseHover hoveredState{ MouseHover::Nothing };

private:
    sf::Vector2f mousePosition;
    sf::Vector2f mousePositionOnScreen;
    Units* hoveredUnit;

    Mouse() = default;
};

