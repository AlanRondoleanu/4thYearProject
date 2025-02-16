#include "Units.h"

Units::Units(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement, AStarMovement t_astarMovement) :
	pos(t_startPosition), flowfieldMovement(t_flowfieldMovement), astarMovement(t_astarMovement)
{
	setPos(pos);
}
Units::~Units() {}

void Units::update()
{
	if (getAlive() == true)
	{
		pos = body.getPosition();
		body.move(velocity * stats.speed);
	}
}

void Units::draw(sf::RenderWindow& t_window)
{
	t_window.draw(body);
}

void Units::setFlowField(FlowField t_field, UnitState t_state)
{
	flowfieldMovement.SetFlowfield(t_field);
	state = t_state;
}

void Units::select()
{
	body.setFillColor(sf::Color::Green);
}

void Units::deselect()
{
	body.setFillColor(sf::Color::Blue);
}

bool Units::isInsideSelection(const sf::FloatRect& selection) const
{
	return selection.intersects(body.getGlobalBounds());
}

void Units::push(sf::Vector2f t_direction)
{
	body.move(t_direction);
}

void Units::setCellID()
{
	int gridX = static_cast<int>(pos.x / FlowField::GRID_WIDTH);
	int gridY = static_cast<int>(pos.y / FlowField::GRID_HEIGHT);

	Cell cell = flowfieldMovement.getFlowfield()->Grid[gridY][gridX];
	cellID = cell.getID();
}
