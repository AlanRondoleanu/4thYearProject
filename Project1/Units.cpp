#include "Units.h"

Units::Units(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement, AStarMovement t_astarMovement) :
	pos(t_startPosition), flowfieldMovement(t_flowfieldMovement), astarMovement(t_astarMovement)
{
	setPos(pos);
}
Units::~Units() {}

void Units::update(float t_deltaTime)
{
	if (getAlive() == true)
	{
		body.move(velocity * stats.speed);
		pos = body.getPosition();

		// Health Set
		healthBar.setHealth(stats.health, stats.max_health);
		healthBar.setPosition(pos, getRadius());

		// Update attack timer
		if (attackCooldown > 0.0f) {
			attackCooldown -= t_deltaTime;
		}

		// Kill unit
		if (stats.health <= 0)
		{
			setAlive(false);
		}
	}
}

void Units::draw(sf::RenderWindow& t_window)
{
	if (getAlive() == true)
	{
		t_window.draw(body);
	}
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

bool Units::canAttack()
{
	return attackCooldown <= 0.0f;
}

void Units::resetAttackTimer()
{
	attackCooldown = 1.0f / stats.attackSpeed;
}

std::string Units::stateToString()
{
	switch (state)
	{
	case UnitState::Moving:        return "Moving";
	case UnitState::Following:     return "Following";
	case UnitState::AttackMove:    return "AttackMove";
	case UnitState::AttackFollow:  return "AttackFollow";
	case UnitState::Attacking:     return "Attacking";
	case UnitState::Idle:          return "Idle";
	default:                       return "Unknown";
	}
	
}

void Units::setCellID()
{
	int gridX = static_cast<int>(pos.x / FlowField::GRID_WIDTH);
	int gridY = static_cast<int>(pos.y / FlowField::GRID_HEIGHT);

	Cell cell = flowfieldMovement.getFlowfield()->Grid[gridY][gridX];
	cellID = cell.getID();
}