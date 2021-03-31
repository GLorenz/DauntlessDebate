// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Board/AttackHandler.h"
#include "Foundation/ConfigHandler.h"
#include "GameManager.h"

BoardComponent::Squares AttackHandler::getAttackOptions(std::shared_ptr<Meeple> attacker, AttackDir attackDir)
{
	auto board = GameManager::getInstance().getBoard();
	auto coord = attacker->getCurrentSquare()->getBoardCoordinates();

	// Find affectedSquares of Attack Range
	BoardComponent::Squares squares;
	
	std::vector<std::vector<int>> pattern;
	if (attacker->getAttack() == Meeple::Attack::Melee) pattern = ConfigHandler::getInstance().getMeleePattern();
	else if (attacker->getAttack() == Meeple::Attack::Ranged) pattern = ConfigHandler::getInstance().getRangedPattern();
	else pattern = ConfigHandler::getInstance().getRandomPattern(attacker->getRandomIdx());

	switch (attackDir)
	{
		case AttackDir::Right:
			pattern = transpose(reverseRows(pattern));
			break;
		case AttackDir::Left:
			pattern = transpose(reverseCols(pattern));
			break;
		case AttackDir::Down:
			pattern = reverseRows(reverseCols(pattern));
			break;
	};


	int cols = pattern.size();
	int rows = pattern[0].size();
	
	int attackerCol = 0;
	int attackerRow = 0;
	for (int row = 0; row < rows; row++)
	{
		for(int col = 0; col < cols; col++)
		{
			if (pattern[col][row] == 2) {
				attackerCol = col;
				attackerRow = row;
			}
		}
	}

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (pattern[col][row] == 1)
			{
				sf::Vector2i distance = sf::Vector2i(col - attackerCol, row - attackerRow);

				// x is always col, y is always row
				auto targetSquare = board->getSquareAt(coord.x + distance.x, coord.y + distance.y);
				if(targetSquare != nullptr)
					squares.push_back(targetSquare);
			}
		}  
	}

	return squares;
}

void AttackHandler::attack(std::shared_ptr<Meeple> attacker, BoardComponent::Squares affectedSquares)
{
	// Calculate effectiveness and damage enemy
	for (auto square : affectedSquares)
	{	
		auto meeple = GameManager::getInstance().getBoard()->getMeepleOnSquare(square);
		if (!meeple) continue;

		if (meeple->isPlayerOne() == attacker->isPlayerOne()) continue;

		Meeple::Type enemyType = meeple->getType();
		Meeple::Type attackerType = attacker->getType();
		if (enemyType == attackerType) meeple->receiveDamage(standardDamage);
		else
		{
			if (attackerType == Meeple::Type::A)
			{
				if(enemyType == Meeple::Type::B) meeple->receiveDamage(standardDamage * 2);
				else if (enemyType == Meeple::Type::C) meeple->receiveDamage(standardDamage / 2);
			}
			else if (attackerType == Meeple::Type::B)
			{
				if (enemyType == Meeple::Type::C) meeple->receiveDamage(standardDamage * 2);
				else if (enemyType == Meeple::Type::A) meeple->receiveDamage(standardDamage / 2);
			}
			else if (attackerType == Meeple::Type::C)
			{
				if (enemyType == Meeple::Type::A) meeple->receiveDamage(standardDamage * 2);
				else if (enemyType == Meeple::Type::B) meeple->receiveDamage(standardDamage / 2);
			}
		}
	}
}

std::vector<std::vector<int>> AttackHandler::transpose(std::vector<std::vector<int>> matrix)
{
	// column major means size of matrix is cols
	int cols = matrix.size();
	int rows = matrix[0].size();

	auto output = createEmpty(cols, rows);

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			output[row][col] = matrix[col][row];
		}
	}

	return output;
}

std::vector<std::vector<int>> AttackHandler::reverseCols(std::vector<std::vector<int>> matrix)
{
	int cols = matrix.size();
	int rows = matrix[0].size();

	auto output = createEmpty(rows, cols);

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			output[col][row] = matrix[cols - col - 1][row];
		}
	}

	return output;
}

std::vector<std::vector<int>> AttackHandler::reverseRows(std::vector<std::vector<int>> matrix)
{
	int cols = matrix.size();
	int rows = matrix[0].size();

	auto output = createEmpty(rows, cols);

	for (int col = 0; col < cols; col++)
	{
		for (int row = 0; row < rows; row++)
		{
			output[col][row] = matrix[col][rows - row - 1];
		}
	}

	return output;
}

std::vector<std::vector<int>> AttackHandler::createEmpty(int rows, int cols)
{
	// 2d vector is column major
	std::vector<std::vector<int>> result(cols);
	for (size_t i = 0; i < result.size(); i++) {
		result[i] = std::vector<int>(rows);
	}
	return result;
}