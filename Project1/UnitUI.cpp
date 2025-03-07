#include "UnitUI.h"

UnitUI::UnitUI(sf::RenderWindow& window)
{
    font.loadFromFile("ariblk.ttf");

    // Get window size at game start
    sf::Vector2u windowSize = window.getSize();
    uiScale.x = windowSize.x / 1920.0f;
    uiScale.y = windowSize.y / 1080.0f;

    // Set UI panel size and position
    sf::Vector2f panelSize{ 400, 300 };

    panel.setSize(sf::Vector2f(panelSize.x * uiScale.x, panelSize.y * uiScale.y));
    panel.setPosition(windowSize.x - panel.getSize().x - 20, windowSize.y - panel.getSize().y - 20);
    panel.setFillColor(sf::Color(50, 50, 50));

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
        if (selectedUnit->GetUnitType() == "Soldier") {
            buttonLabels[0].setString("Attack");
            buttonLabels[1].setString("Move");
            buttonLabels[2].setString("Hold");
        }
    }
}

void UnitUI::Render(sf::RenderWindow& window, sf::View& uiView) const
{
    sf::View originalView = window.getView();
    window.setView(uiView); // Switch to UI view

    window.draw(panel);
    if (selectedUnit != nullptr)
    {
        for (auto& button : buttons) window.draw(button);
        for (auto& label : buttonLabels) window.draw(label);
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
