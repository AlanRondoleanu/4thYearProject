#ifndef UNITUI_H
#define UNITUI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <unordered_set>
#include "Units.h"
#include <functional>

class UnitUI 
{
public:
    UnitUI(sf::RenderWindow& window);
    void SetSelectedUnit(std::shared_ptr<Targetable> unit);
    void Render(sf::RenderWindow& window, sf::View& uiView) const;
    void HandleClick(sf::Vector2f mousePos);
    bool isInsideUI(sf::Vector2f mousePos);
    void SetButtonAction(size_t index, const std::function<void()>& action);
    void SetDefaultButtonAction(size_t index, const std::function<void()>& action);
    void RestoreDefaultButtons();

    bool insideUI = false;

private:
    // Portrait Handling
    void loadPortrait(const std::string& unitName, const std::string& filePath);
    const sf::Texture& getPortrait(const std::string& unitName);

    sf::RectangleShape panel;
    sf::RectangleShape portraitPanel;
    sf::Sprite portraitSprite;
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonLabels;
    std::vector<std::function<void()>> buttonCallbacks;
    std::vector<std::function<void()>> defaultButtonCallbacks;
    sf::Font font;
    std::shared_ptr<Targetable> selectedUnit;
    sf::Vector2f uiScale;

    std::unordered_map<std::string, sf::Texture> portraits;
};

#endif