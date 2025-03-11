#include "HealthBars.h"

HealthBars::HealthBars()
    : currentHealth(100), maxHealth(100)
{
    // Background bar
    backgroundBar.setSize(sf::Vector2f(barWidth, barHeight));
    backgroundBar.setFillColor(sf::Color::Red);

    // Foreground health bar
    healthBar.setSize(sf::Vector2f(barWidth, barHeight));
    healthBar.setFillColor(sf::Color::Green);
}

void HealthBars::setPosition(sf::Vector2f position, float radius)
{
    float x = position.x - (barWidth / 2.f);
    float y = position.y - radius - 10.f;

    backgroundBar.setPosition(x, y);
    healthBar.setPosition(x, y);
}

void HealthBars::setHealth(float current, float max)
{
    currentHealth = current;
    maxHealth = max;

    float healthPercent = currentHealth / maxHealth;
    healthPercent = std::max(0.f, std::min(1.f, healthPercent)); // Clamp

    healthBar.setSize(sf::Vector2f(barWidth * healthPercent, barHeight));
}

void HealthBars::render(sf::RenderWindow& t_window)
{
    t_window.draw(backgroundBar);
    t_window.draw(healthBar);
}