// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "Board/Meeple.h"

class ConfigHandler
{
public:
	using Pattern = std::vector<std::vector<int>>;
	using MeepleConfig = std::tuple<Meeple::Attack, Meeple::Type, std::string>;
	static ConfigHandler& getInstance();

	void setup();

	int getRandomVecSize();
	void registerMeleePattern(Pattern pattern);
	void registerRangedPattern(Pattern pattern);
	void registerRandomPattern(Pattern pattern);
	void registerMeepleConfig(MeepleConfig meepleConfig);

	Pattern getMeleePattern();
	Pattern getRangedPattern();
	Pattern getRandomPattern(int idx);
	std::vector<MeepleConfig> getMeepleConfigs();
	int getMeepleMaxHealth();

	int getMaxRounds();
	int getMaxMoves();
	sf::Color getP1Color();
	sf::Color getP2Color();
	sf::Color getSelectedColorMove();
	sf::Color getSelectedColorDamage();

private:
	ConfigHandler();
	~ConfigHandler() = default;
	ConfigHandler(const ConfigHandler & p) = delete;
	ConfigHandler& operator=(ConfigHandler const&) = delete;

	int m_maxRounds;
	sf::Color m_p1Color, m_p2Color, m_selectedColorMove, m_selectedColorDamage;

	Pattern m_meleePattern;
	Pattern m_rangedPattern;
	std::vector<Pattern> m_randomPattern;
	std::vector<MeepleConfig> m_meepleConfigs;
	int m_meepleMaxHealth;
	int m_maxMoves;
};