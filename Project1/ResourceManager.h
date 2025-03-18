#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>

class ResourceManager
{
public:
    ResourceManager(sf::RenderWindow& window);
    void Render(sf::RenderWindow& window, sf::View& uiView) const;

    void setGoldString(int t_int);
    void addResource(const std::string& type, int amount);
    bool checkCost(const std::string& type, int amount);
    void spendResource(const std::string& type, int amount);
    int getResource(const std::string& type) const;
    int getCost(std::string t_typename);

    // Resource display UI elements
    sf::RectangleShape resourcePanel;
    sf::Text goldText;
    sf::Font font;

private:
    std::unordered_map<std::string, int> resources;


};

#endif