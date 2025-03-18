#include "ResourceManager.h"

ResourceManager::ResourceManager(sf::RenderWindow& window)
{
    // Initialize starting resources
    resources["Gold"] = 500;
    setGoldString(resources["Gold"]);

    // Load the font (use your font path)
    if (!font.loadFromFile("ariblk.ttf"))
    {
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Create the resource panel box
    resourcePanel.setSize(sf::Vector2f(200.f, 50.f));  // Width x Height
    resourcePanel.setFillColor(sf::Color(50, 50, 50, 200));  // Semi-transparent dark gray

    // Position it in the bottom right corner
    sf::Vector2u windowSize = window.getSize();
    resourcePanel.setPosition(windowSize.x - resourcePanel.getSize().x, windowSize.y - resourcePanel.getSize().y - 350);


    // Set up the text
    goldText.setFont(font);
    goldText.setCharacterSize(18);
    goldText.setFillColor(sf::Color::White);

    // Position it inside the panel
    goldText.setPosition(
        resourcePanel.getPosition().x + 10.f,
        resourcePanel.getPosition().y + 10.f
    );

}

void ResourceManager::Render(sf::RenderWindow& window, sf::View& uiView) const
{
    sf::View originalView = window.getView();
    window.setView(uiView);

    window.draw(resourcePanel);
    window.draw(goldText);

    window.setView(originalView);
}

void ResourceManager::setGoldString(int t_int)
{
    std::string resource = "Gold: ";
    resource += std::to_string(t_int);

    goldText.setString(resource);
}

void ResourceManager::addResource(const std::string& type, int amount)
{
    resources[type] += amount;
    setGoldString(resources[type]);
}

bool ResourceManager::checkCost(const std::string& type, int amount)
{
    if (resources[type] >= amount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ResourceManager::spendResource(const std::string& type, int amount)
{
    resources[type] -= amount;
    setGoldString(resources[type]);
}

int ResourceManager::getResource(const std::string& type) const
{
    auto it = resources.find(type);
    if (it != resources.end())
        return it->second;
    return 0;
}

int ResourceManager::getCost(std::string t_typename)
{
    if (t_typename == "Soldier_Building")
    {
        return 100;
    }
    else if(t_typename == "Orc_Building")
    { 
        return 100;
    }
    else if (t_typename == "NightElf_Building")
    {
        return 100;
    }
    else
    {
        return 0;
    }
}
