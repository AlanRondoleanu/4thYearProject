#include "SelectorTool.h"



SelectorTool::SelectorTool()
{
    selectionBox.setFillColor(sf::Color(0, 0, 255, 50));
    selectionBox.setOutlineColor(sf::Color::Blue);
    selectionBox.setOutlineThickness(1.0f);
}

void SelectorTool::render(sf::RenderWindow& t_window)
{
    if (isSelecting)
        t_window.draw(selectionBox);
}

void SelectorTool::updateSelectionBox()
{
    sf::Vector2f size = endPos - startPos;
    selectionBox.setSize({ std::abs(size.x), std::abs(size.y) });
    selectionBox.setPosition(
        std::min(startPos.x, endPos.x),
        std::min(startPos.y, endPos.y)
    );
}

void SelectorTool::resetBounds()
{
    selectionBox.setSize({ 0,0 });
}

void SelectorTool::handleEvent(const sf::Event& event, bool insideUI)
{
    if (!insideUI)
    {
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            isSelecting = true;
            resetBounds();
            startPos = Mouse::getInstance().getPosition();

            // Deselect Units
            for (auto& unit : UnitHandler::getInstance().selectedUnits)
            {
                unit->deselect();
            }
            UnitHandler::getInstance().selectedUnits.clear();
        }
        else if (event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            isSelecting = false;
            selectUnits();
        }
        else if (event.type == sf::Event::MouseMoved && isSelecting)
        {
            endPos = Mouse::getInstance().getPosition();
            updateSelectionBox();
        }
    }
}

void SelectorTool::selectUnits()
{
    UnitHandler::getInstance().selectedUnits.clear();
    for (auto& unit : UnitHandler::getInstance().playerUnits)
    {
        std::shared_ptr<Units> currentUnit = std::dynamic_pointer_cast<Units>(unit);
        bool inside = currentUnit->isInsideSelection(selectionBox.getGlobalBounds());
        if (inside == true)
        {
            UnitHandler::getInstance().selectedUnits.insert(currentUnit.get());
            currentUnit->select();
        }
    }
}
