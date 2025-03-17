#include "TileMap.h"
#include <iostream>

TileMap::TileMap(int gridWidth, int gridHeight, int cellSize)
    : gridWidth(gridWidth), gridHeight(gridHeight), cellSize(cellSize)
{

    loadTileTexture("Assets/Textures/map.png");
    float scaleFactor = 50.0f / 64.0f;
    mapSprite.setScale(scaleFactor, scaleFactor);
}

void TileMap::loadTileTexture(const std::string& filePath)
{
    if (!mapTexture.loadFromFile(filePath))
    {
        std::cerr << "Failed to load texture from " << filePath << std::endl;
        return;
    }

    mapSprite.setTexture(mapTexture);
}


void TileMap::draw(sf::RenderWindow& window)
{
    window.draw(mapSprite);
}