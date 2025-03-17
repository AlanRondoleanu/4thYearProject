#include "Buildings.h"
#include "Units.h"

Buildings::Buildings(const sf::Vector2f& position, bool isEnemy)
{
	setPos(position);
	body.setPosition(position);
	body.setFillColor(sf::Color::Black);
	placed = true;
	enemy = isEnemy;
}

void Buildings::draw(sf::RenderWindow& t_window)
{
	//t_window.draw(body);
    t_window.draw(sprite);
    healthBar.render(t_window);
}

void Buildings::resetAttackTimer()
{
    attackCooldown = 1.0f / stats.attackSpeed;
}

void Buildings::loadTileTexture(const std::string& filePath)
{
    if (!texture.loadFromFile(filePath))
    {
        std::cerr << "Failed to load texture from " << filePath << std::endl;
        return;
    }

    //float scaleFactor = 50.0f / 64.0f;
    //sprite.setScale(scaleFactor, scaleFactor);
    sprite.setTexture(texture);

    sf::Vector2u textureSize = texture.getSize();
    sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
    sprite.setPosition(body.getPosition());  
}

sf::Vector2f Buildings::getSpawnPoint()
{
    sf::Vector2f position = getPos();
    sf::Vector2f size = body.getSize();

    const float spawnOffset = 25.f;

    if (enemy) 
    {
        // Spawn below the building
        position.y += size.y + spawnOffset;
    }
    else {
        // Spawn above the building
        position.y -= size.y + spawnOffset;
    }

    return position;
}

bool Buildings::canAttack()
{
    if (aggressive)
    {
        return attackCooldown <= 0.0f;
    }
    else {
        return false;
    }
}

void Buildings::setPos(sf::Vector2f t_position)
{
	body.setPosition(t_position);
    sprite.setPosition(t_position);
}

