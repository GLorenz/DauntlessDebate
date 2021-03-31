// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/GameStateManager.h"
#include "GameObjects/GameObject.h"

class ScoreState : public GameState {
public:
	ScoreState();

	void update() override;
	void draw(sf::RenderWindow& window) override;
	void init(GameStateManager& manager) override;
	void exit() override;

private:
	std::unique_ptr<GameObject> createScoreText(bool forPlayerOne);
	std::unique_ptr<GameObject> createWinnerText();
	std::unique_ptr<GameObject> createBackground();

	GameStateManager* m_manager;
	std::string m_buttonName;
	int m_p1Score;
	int m_p2Score;
	std::unique_ptr<GameObject> m_p1TextObj, m_p2TextObj, m_winnerTextObj, m_background;
};