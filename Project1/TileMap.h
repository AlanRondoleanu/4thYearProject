#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>

class TileMap 
{
public:
    TileMap(int gridWidth, int gridHeight, int cellSize);

    void loadTileTexture(const std::string& filePath);

    // Draw the map to the window
    void draw(sf::RenderWindow& window);

private:
    int gridWidth;
    int gridHeight;
    int cellSize;

    sf::Texture mapTexture;
    sf::Sprite mapSprite;
};

#endif