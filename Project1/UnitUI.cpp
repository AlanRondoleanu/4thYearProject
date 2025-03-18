#include "UnitUI.h"

UnitUI::UnitUI(sf::RenderWindow& window)
{
    font.loadFromFile("ariblk.ttf");

    sf::Vector2u windowSize = window.getSize();
    uiScale.x = windowSize.x / 1920.0f;
    uiScale.y = windowSize.y / 1080.0f;

    // Set UI panel
    sf::Vector2f panelSize{ 425, 300 };

    panel.setSize(sf::Vector2f(panelSize.x * uiScale.x, panelSize.y * uiScale.y));
    panel.setPosition(windowSize.x - panel.getSize().x, windowSize.y - panel.getSize().y - 20);
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
    loadPortrait("Soldier", "Assets/Textures/Sci-fi/Soldier_Portrait.png");
    loadPortrait("Orc", "Assets/Textures/Fantasy/Orc_Portrait.png");
    loadPortrait("NightElf", "Assets/Textures/Fantasy/NightElf_Portrait.png");

    // Create 8 buttons in a 2x4 grid
    const int numButtons = 8;
    const int columns = 4;
    const int rows = 2;

    sf::Vector2f buttonSize{ 90, 60 };
    float buttonSpacingX = 10.0f;
    float buttonSpacingY = 10.0f;

    float startX = panel.getPosition().x + 10 * uiScale.x;
    float startY = panel.getPosition().y + 10 * uiScale.y;

    for (int i = 0; i < numButtons; i++)
    {
        sf::RectangleShape button(sf::Vector2f(buttonSize.x * uiScale.x, buttonSize.y * uiScale.y));
        button.setFillColor(sf::Color(100, 100, 100));

        int col = i % columns;
        int row = i / columns;

        float posX = startX + col * ((buttonSize.x + buttonSpacingX) * uiScale.x);
        float posY = startY + row * ((buttonSize.y + buttonSpacingY) * uiScale.y);

        button.setPosition(posX, posY);

        buttons.push_back(button);

        sf::Text label;
        label.setFont(font);
        label.setCharacterSize(static_cast<unsigned int>(14 * uiScale.y));
        label.setPosition(posX + 10, posY + 5);
        buttonLabels.push_back(label);
    }

    // Resizing callbacks to be same as button size
    buttonCallbacks.resize(buttons.size());

    RestoreDefaultButtons();
}

void UnitUI::SetSelectedUnit(std::shared_ptr<Targetable> unit)
{
    if (selectedUnit == unit)
        return;

    selectedUnit = unit;

    // Clear existing button labels and callbacks
    for (auto& label : buttonLabels)
        label.setString("");

    buttonCallbacks.clear(); // Reset the button callbacks

    if (selectedUnit)
    {
        if (selectedUnit->GetUnitType() == "Soldier")
        {
            portraitSprite.setTexture(getPortrait("Soldier"));

            buttonLabels[0].setString("Attack");
            SetButtonAction(0, [this]() {
                std::cout << "Attack Command for Soldier\n";
                });

            buttonLabels[1].setString("Move");
            SetButtonAction(1, [this]() {
                std::cout << "Move Command for Soldier\n";
                });

            buttonLabels[2].setString("Hold");
            SetButtonAction(2, [this]() {
                std::cout << "Hold Position for Soldier\n";
                });
        }
        else if (selectedUnit->GetUnitType() == "Orc")
        {
            portraitSprite.setTexture(getPortrait("Orc"));

            buttonLabels[0].setString("Attack");
            SetButtonAction(0, [this]() {
                std::cout << "Attack Command for Orc\n";
                });

            buttonLabels[1].setString("Move");
            SetButtonAction(1, [this]() {
                std::cout << "Move Command for Orc\n";
                });

            buttonLabels[2].setString("Hold");
            SetButtonAction(2, [this]() {
                std::cout << "Hold Position for Orc\n";
                });
        }
        else if (selectedUnit->GetUnitType() == "NightElf")
        {
            portraitSprite.setTexture(getPortrait("NightElf"));

            buttonLabels[0].setString("Attack");
            SetButtonAction(0, [this]() {
                std::cout << "Attack Command for NightElf\n";
                });

            buttonLabels[1].setString("Move");
            SetButtonAction(1, [this]() {
                std::cout << "Move Command for NightElf\n";
                });

            buttonLabels[2].setString("Hold");
            SetButtonAction(2, [this]() {
                std::cout << "Hold Position for NightElf\n";
                });
        }

        // Handle other unit types here...
    }
    else
    {
        RestoreDefaultButtons();
    }
}
void UnitUI::Render(sf::RenderWindow& window, sf::View& uiView) const
{
    sf::View originalView = window.getView();
    window.setView(uiView); // Switch to UI view

    window.draw(panel);
    window.draw(portraitPanel);

    if (true)
    {
        for (auto& button : buttons) window.draw(button);
        for (auto& label : buttonLabels) window.draw(label);

        if (selectedUnit)
            window.draw(portraitSprite);
    }

    window.setView(originalView);
}

bool UnitUI::isInsideUI(sf::Vector2f mousePos)
{
    if (panel.getGlobalBounds().contains(mousePos) ||
        portraitPanel.getGlobalBounds().contains(mousePos))
    {
        insideUI = true;
        return true;
    }
    insideUI = false;
    return false;
}

void UnitUI::SetButtonAction(size_t index, const std::function<void()>& action)
{
    // Increase vector size
    if (index >= buttonCallbacks.size()) {
        buttonCallbacks.resize(index + 1);
    }

    buttonCallbacks[index] = action;
}

void UnitUI::SetDefaultButtonAction(size_t index, const std::function<void()>& action)
{
    if (index >= defaultButtonCallbacks.size())
        defaultButtonCallbacks.resize(index + 1);

    defaultButtonCallbacks[index] = action;

    // If there is no unit selected, use default callbacks immediately
    if (!selectedUnit) 
    {
        if (index >= buttonCallbacks.size())
            buttonCallbacks.resize(index + 1);

        buttonCallbacks[index] = action;
    }
}

void UnitUI::RestoreDefaultButtons()
{
    // Restore default callbacks
    buttonCallbacks = defaultButtonCallbacks;

    for (size_t i = 0; i < buttonLabels.size(); ++i)
    {
        buttonLabels[i].setString("");

        if (i == 0) buttonLabels[i].setString("Orc");
        if (i == 1) buttonLabels[i].setString("Soldier");
        if (i == 2) buttonLabels[i].setString("Night Elf");
    }
}

void UnitUI::HandleClick(sf::Vector2f mousePos)
{
    for (size_t i = 0; i < buttons.size(); i++)
    {
        if (buttons[i].getGlobalBounds().contains(mousePos))
        {
            if (buttonCallbacks[i]) 
            {
                buttonCallbacks[i]();
            }
            else {
                std::cout << "No action assigned to this button.\n";
            }
        }
    }
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
