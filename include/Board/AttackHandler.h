// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "Board/BoardComponent.h"
#include "Foundation/AttackDir.h"

class Meeple;

class AttackHandler
{
public:
	static BoardComponent::Squares getAttackOptions(std::shared_ptr<Meeple> attacker, AttackDir attackDir);
	static void attack(std::shared_ptr<Meeple> attacker, BoardComponent::Squares affectedSquares);
private:
	static std::vector<std::vector<int>> transpose(std::vector<std::vector<int>> matrix);
	static std::vector<std::vector<int>> reverseCols(std::vector<std::vector<int>> matrix);
	static std::vector<std::vector<int>> reverseRows(std::vector<std::vector<int>> matrix);

	static std::vector<std::vector<int>> createEmpty(int rows, int cols);
	const static int standardDamage = 2;
	const static int rangedAttack = 4;
};