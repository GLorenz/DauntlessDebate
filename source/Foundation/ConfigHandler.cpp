// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Foundation/ConfigHandler.h"

ConfigHandler& ConfigHandler::getInstance()
{
	static ConfigHandler m_instance;
	return m_instance;
}

// we're planning to read this config from a file
// but had not time for it yet

ConfigHandler::ConfigHandler()
	: m_maxRounds(8)
	, m_p1Color(224, 142, 69, 255)
	, m_p2Color(56, 184, 148, 255)
	, m_selectedColorDamage(107, 64, 80, 255)
	, m_selectedColorMove(72, 84, 170, 255)
	, m_meepleMaxHealth(10)
	, m_maxMoves(4)
{

}
void ConfigHandler::setup()
{
	m_meepleConfigs =
	{
		{Meeple::Attack::Ranged, Meeple::Type::A, "rangedA"},
		{Meeple::Attack::Ranged, Meeple::Type::B, "rangedB"},
		{Meeple::Attack::Ranged, Meeple::Type::C, "rangedC"},
		{Meeple::Attack::Melee, Meeple::Type::A, "meleeA"},
		{Meeple::Attack::Melee, Meeple::Type::B, "meleeB"},
		{Meeple::Attack::Melee, Meeple::Type::C, "meleeC"},
		{Meeple::Attack::Random, Meeple::Type::A, "randomA"},
		{Meeple::Attack::Random, Meeple::Type::B, "randomB"},
		{Meeple::Attack::Random, Meeple::Type::C, "randomC"}
	};

	// changed all patterns to be column major
	// can also do it row major here and rotate once before using

	m_rangedPattern =
	{
		{1, 1, 1, 1, 0, 0, 2}
	};

	m_meleePattern =
	{
		{1,1,1},
		{1,2,1},
		{1,1,1}
	};

	m_randomPattern.push_back(
		{
			{1,1,1,0,0},
			{1,0,0,0,0},
			{1,0,2,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		}
	);

	m_randomPattern.push_back(
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{1,1,2,1,1},
			{0,0,0,0,0},
			{0,0,0,0,0}
		}
	);

	m_randomPattern.push_back(
		{
			{1,0,0,0,1},
			{0,0,0,0,0},
			{0,0,2,0,0},
			{0,0,0,0,0},
			{1,0,0,0,1}
		}
	);

	m_randomPattern.push_back(
		{
			{0,0,0,0,0},
			{0,1,0,0,0},
			{1,1,2,0,0},
			{0,1,0,0,0},
			{0,0,0,0,0}
		}
	);

	m_randomPattern.push_back(
		{
			{1,0,0,0,0},
			{0,1,0,0,0},
			{0,0,1,2,0},
			{0,1,0,0,0},
			{1,0,0,0,0}
		}
	);

	m_randomPattern.push_back(
		{
			{0,0,0,0,0},
			{0,1,1,0,0},
			{0,0,0,2,0},
			{0,1,1,0,0},
			{0,0,0,0,0}
		}
	);
}

int ConfigHandler::getRandomVecSize()
{
	return m_randomPattern.size();
}

void ConfigHandler::registerMeleePattern(Pattern pattern)
{
	m_meleePattern = pattern;
}

void ConfigHandler::registerRangedPattern(Pattern pattern)
{
	m_rangedPattern = pattern;
}

void ConfigHandler::registerRandomPattern(Pattern pattern)
{
	m_randomPattern.push_back(pattern);
}

void ConfigHandler::registerMeepleConfig(MeepleConfig meepleConfig)
{
	m_meepleConfigs.push_back(meepleConfig);
}

ConfigHandler::Pattern ConfigHandler::getMeleePattern()
{
	return m_meleePattern;
}

ConfigHandler::Pattern ConfigHandler::getRangedPattern()
{
	return m_rangedPattern;
}

ConfigHandler::Pattern ConfigHandler::getRandomPattern(int idx)
{
	return m_randomPattern[idx];
}

std::vector<ConfigHandler::MeepleConfig> ConfigHandler::getMeepleConfigs()
{
	return m_meepleConfigs;
}

int ConfigHandler::getMeepleMaxHealth()
{
	return m_meepleMaxHealth;
}

int ConfigHandler::getMaxRounds()
{
	return m_maxRounds;
}

int ConfigHandler::getMaxMoves()
{
	return m_maxMoves;
}

sf::Color ConfigHandler::getP1Color()
{
	return m_p1Color;
}

sf::Color ConfigHandler::getP2Color()
{
	return m_p2Color;
}

sf::Color ConfigHandler::getSelectedColorMove()
{
	return m_selectedColorMove;
}

sf::Color ConfigHandler::getSelectedColorDamage()
{
	return m_selectedColorDamage;
}

