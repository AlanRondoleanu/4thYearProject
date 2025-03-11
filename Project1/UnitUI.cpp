#include "UnitUI.h"

UnitUI::UnitUI(sf::RenderWindow& window)
{
    font.loadFromFile("ariblk.ttf");

    sf::Vector2u windowSize = window.getSize();
    uiScale.x = windowSize.x / 1920.0f;
    uiScale.y = windowSize.y / 1080.0f;

    // Set UI panel
    sf::Vector2f panelSize{ 400, 300 };

    panel.setSize(sf::Vector2f(panelSize.x * uiScale.x, panelSize.y * uiScale.y));
    panel.setPosition(windowSize.x - panel.getSize().x - 20, windowSize.y - panel.getSize().y - 20);
    panel.setFillColor(sf::Color(50, 50, 50));

    // Portrait Panel
    sf::Vector2f protraitPanelSize{ 250, 300 };

    portraitPanel.setSize(sf::Vector2f(protraitPanelSize.x * uiScale.x, protraitPanelSize.y * uiScale.y));
    portraitPanel.setPosition(windowSize.x - portraitPanel.getSize().x - 350, windowSize.y - portraitPanel.getSize().y - 20);
    portraitPanel.setFillColor(sf::Color(0, 0, 0));

    // Portrait Sprite
    sf::Vector2f protraitSpriteSize{ 250, 300 };

    portraitSprite.setPosition(windowSize.x - portraitPanel.getSize().x - 340, windowSize.y - portraitPanel.getSize().y - 10);
    portraitSprite.setScale(0.4,0.4);

    // Portrait loading
    loadPortrait("Soldier", "Assets/Portraits/Soldier.png");

    // Create buttons
    for (int i = 0; i < 3; i++) 
    {
        sf::Vector2f buttonSize{ 100, 75 };
        sf::RectangleShape button(sf::Vector2f(buttonSize.x * uiScale.x, buttonSize.y * uiScale.y));
        button.setFillColor(sf::Color(100, 100, 100));

        float buttonXOffset = 10;
        float buttonYOffset = 10;
        float buttonSpacing = 125;
        button.setPosition(panel.getPosition().x + buttonXOffset + (i * (buttonSpacing * uiScale.x)), panel.getPosition().y + buttonYOffset);
        buttons.push_back(button);

        sf::Text label;
        label.setFont(font);
        label.setCharacterSize(static_cast<unsigned int>(14 * uiScale.y));
        label.setPosition(button.getPosition().x + 10, button.getPosition().y + 5);
        buttonLabels.push_back(label);
    }
}

void UnitUI::SetSelectedUnit(std::shared_ptr<Units> unit)
{
    selectedUnit = unit;

    // Update button labels based on unit type
    if (selectedUnit) 
    {
        if (selectedUnit->GetUnitType() == "Soldier") 
        {
            buttonLabels[0].setString("Attack");
            buttonLabels[1].setString("Move");
            buttonLabels[2].setString("Hold");

            portraitSprite.setTexture(getPortrait("Soldier"));
        }
    }


}

void UnitUI::Render(sf::RenderWindow& window, sf::View& uiView) const
{
    sf::View originalView = window.getView();
    window.setView(uiView); // Switch to UI view

    window.draw(panel);
    window.draw(portraitPanel);

    if (selectedUnit != nullptr)
    {
        for (auto& button : buttons) window.draw(button);
        for (auto& label : buttonLabels) window.draw(label);

        window.draw(portraitSprite);
    }

    window.setView(originalView);
}

void UnitUI::HandleClick(sf::Vector2f mousePos)
{
    if (!selectedUnit) return;

    for (size_t i = 0; i < buttons.size(); i++) 
    {
        if (buttons[i].getGlobalBounds().contains(mousePos)) 
        {
            if (selectedUnit->GetUnitType() == "Soldier") {
                if (i == 0) std::cout << "Attack Command\n";
                if (i == 1) std::cout << "Move Command\n";
                if (i == 2) std::cout << "Hold Position\n";
            }
        }
    }
}

bool UnitUI::isInsideUI(sf::Vector2f mousePos)
{
    if (panel.getGlobalBounds().contains(mousePos))
    {
        return true;
    }
    
    return false;
}

void UnitUI::loadPortrait(const std::string& unitName, const std::string& filePath)
{
    sf::Texture texture;
    if (texture.loadFromFile(filePath)) 
    {
        portraits[unitName] = texture;
    }
    else {
        std::cout << "Failed to load portrait for " << unitName << std::endl;
    }
}

const sf::Texture& UnitUI::getPortrait(const std::string& unitName)
{
    return portraits.at(unitName);
}
