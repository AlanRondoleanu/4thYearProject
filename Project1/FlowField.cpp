#include "FlowField.h"


FlowField::FlowField() 
{
	loadFont();

	float x = 0;
	float y = 0;
	int cellNumber = 0;
	Cell cell = Cell();

	Grid.resize(CELL_AMOUNT, std::vector<Cell>(CELL_AMOUNT));

	for (size_t o = 0; o < CELL_AMOUNT; o++)
	{
		for (size_t i = 0; i < CELL_AMOUNT; i++)
		{
			sf::Vector2f newPosition = { x, y };
			cell.setPosition(newPosition);
			cell.setID(cellNumber);
			cell.setSize({ CELL_WIDTH, CELL_HEIGHT });
			cell.initialize();
			cell.setFont(font);

			Grid[o][i] = cell;

			x += CELL_WIDTH;
			cellNumber++;
		}
		y += CELL_HEIGHT;
		x = 0;
	}

	setNeighbors();
}

void FlowField::setNeighbors()
{
	for (int o = 0; o < CELL_AMOUNT; o++)
	{
		for (int i = 0; i < CELL_AMOUNT; i++)
		{
			Cell* currentCell = &Grid[o][i];

			// Up
			if (o - 1 >= 0)
			{
				currentCell->neighbouringCells.push_back(&Grid[o - 1][i]);
			}
			// Left
			if (i - 1 >= 0)
			{
				currentCell->neighbouringCells.push_back(&Grid[o][i - 1]);
			}
			// Right
			if (i + 1 < CELL_AMOUNT)
			{
				currentCell->neighbouringCells.push_back(&Grid[o][i + 1]);
			}
			// Down
			if (o + 1 < CELL_AMOUNT)
			{
				currentCell->neighbouringCells.push_back(&Grid[o + 1][i]);
			}

			// DIAGONAL CELLS

			// Up-Left
			if (o > 0 && i > 0) {
				currentCell->diagonalCells.push_back(&Grid[o - 1][i - 1]);
			}
			// Up-Right
			if (o > 0 && i + 1 < CELL_AMOUNT) {
				currentCell->diagonalCells.push_back(&Grid[o - 1][i + 1]);
			}
			// Down-Left
			if (o + 1 < CELL_AMOUNT && i > 0) {
				currentCell->diagonalCells.push_back(&Grid[o + 1][i - 1]);
			}
			// Down-Right
			if (o + 1 < CELL_AMOUNT && i + 1 < CELL_AMOUNT) {
				currentCell->diagonalCells.push_back(&Grid[o + 1][i + 1]);
			}
		}
	}
}

void FlowField::setDestination(Cell* t_cell)
{
	destination = t_cell;
}

void FlowField::createFlowField()
{
	for (auto& row : Grid)
	{
		for (Cell& cell : row)
		{
			std::vector<Cell*> neighbors = cell.neighbouringCells;
			neighbors.insert(neighbors.end(), cell.diagonalCells.begin(), cell.diagonalCells.end());
			
			int bestCost = cell.bestCost;

			for (Cell* neighbor : neighbors)
			{
				if (neighbor->bestCost < bestCost)
				{
					bestCost = neighbor->bestCost;
					sf::Vector2f direction = directionBetweenTwoPoints(neighbor->getPostion(), cell.getPostion());
					cell.setDirection(direction);
				}
			}
		}
	}
}

void FlowField::render(sf::RenderWindow& t_window)
{
	for (auto& row : Grid)
	{
		for (Cell& cell : row)
		{
			cell.render(t_window);
		}
	}
}

void FlowField::resetField(std::vector<Buildings*> t_buildings)
{
	for (auto& row : Grid)
	{
		for (Cell& cell : row)
		{
			cell.setDirection({ 0, 0 });
			cell.setCost(1);
			cell.setBestCost(99999);

			for (Buildings* building : t_buildings)
			{
				if (building->body.getGlobalBounds().intersects(cell.shape.getGlobalBounds()))
				{
					cell.setCost(99999);
				}
			}
		}
	}
}

sf::Vector2f FlowField::directionBetweenTwoPoints(sf::Vector2f t_1, sf::Vector2f t_2)
{
	sf::Vector2f direction = t_1 - t_2;
	float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude != 0)
	{
		direction /= magnitude;  // Normalize the direction vector
	}
	return direction;
}

void FlowField::createIntegrationField()
{
	std::queue<Cell*> cellsToCheck;

	// Current Destination Cell
	destination->cost = 0;
	destination->bestCost = 0;
	cellsToCheck.push(destination);

	while (!cellsToCheck.empty())
	{
		Cell* currentCell = cellsToCheck.front();
		cellsToCheck.pop();

		std::vector<Cell*> neightbors = currentCell->getNeighbors();
		for (Cell* neighbor : neightbors)
		{
			// Ignore impassable cells
			if (neighbor->cost == neighbor->MAX_COST)
				continue;

			if (neighbor->cost + currentCell->bestCost < neighbor->bestCost)
			{
				neighbor->setBestCost(neighbor->cost + currentCell->bestCost);
				cellsToCheck.push(neighbor);
			}
		}
	}
}

void FlowField::loadFont()
{
	//Font Loading
	if (!font.loadFromFile("ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
}