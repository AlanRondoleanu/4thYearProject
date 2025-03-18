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
		setPos(body.getPosition());

		// Health Set
		healthBar.setHealth(stats.health, stats.max_health);
		healthBar.setPosition(pos, getRadius());

		// Update attack timer
		if (attackCooldown > 0.0f) 
		{
			attackCooldown -= t_deltaTime;
		}

		// Kill unit
		if (stats.health <= 0)
		{
			setAlive(false);
		}

		if (velocity.y < 0 && currentFacing != UnitFacing::Back) 
		{
			swapTexture(0);
			currentFacing = UnitFacing::Back;
		}
		else if (velocity.y > 0 && currentFacing != UnitFacing::Front) 
		{

			swapTexture(1);
			currentFacing = UnitFacing::Front;
		}
	}
}

void Units::loadTexture(const std::string& filePath)
{
	sf::Texture t_texture;

	if (!t_texture.loadFromFile(filePath))
	{
		std::cerr << "Failed to load texture from " << filePath << std::endl;
		return;
	}

	textures.push_back(t_texture);
	sprite.setPosition(body.getPosition());
}

void Units::swapTexture(int t_value)
{
	sprite.setTexture(textures[t_value]);
	sf::Vector2u textureSize = textures[t_value].getSize();
	sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
}

void Units::draw(sf::RenderWindow& t_window)
{
	if (getAlive() == true)
	{
		t_window.draw(body);
		t_window.draw(sprite);
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

void Units::takeDamage(float amount)
{
	stats.health -= amount;
}

std::shared_ptr<Targetable> Units::getTarget()
{
	return currentTarget;
}

UnitState Units::getState()
{
	return state;
}

UnitStats Units::getUnitStats() const
{
	return stats;
}

int Units::getCellID() const
{
	return cellID;
}

void Units::setPos(sf::Vector2f t_position)
{
	pos = t_position;
	body.setPosition(pos); 
	sprite.setPosition(pos);
}

void Units::setCellID()
{
	int gridX = static_cast<int>(pos.x / FlowField::CELL_WIDTH);
	int gridY = static_cast<int>(pos.y / FlowField::CELL_HEIGHT);

	Cell cell = flowfieldMovement.getFlowfield()->Grid[gridY][gridX];
	cellID = cell.getID();
}

void Units::setState(UnitState t_state)
{
	state = t_state;
}

void Units::setTarget(std::shared_ptr<Targetable> t_target)
{
	currentTarget = std::move(t_target);
}
