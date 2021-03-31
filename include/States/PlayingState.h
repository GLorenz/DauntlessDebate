// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/GameStateManager.h"
#include "States/IPlayingSubState.h"
#include "GameObjects/GameObject.h"
#include "Board/BoardComponent.h"

class PlayingState : public GameState {
public:
	PlayingState();

	void update() override;
	void draw(sf::RenderWindow& window) override;
	void init(GameStateManager& manager) override;
	void exit() override;

	void onSubStateFinished(IPlayingSubState* subState);

private:
	void startState(std::unique_ptr<IPlayingSubState> state);

	std::unique_ptr<IPlayingSubState> m_subState;

	GameStateManager* m_manager;
	std::shared_ptr<GameObject> m_camera;
	std::shared_ptr<GameObject> m_board;
	std::shared_ptr<BoardComponent> m_boardComp;
	std::shared_ptr<GameObject> m_backgroundObj;
	std::shared_ptr<GameObject> m_effectivnessObj;
	std::shared_ptr<GameObject> m_rulesObj;
	std::shared_ptr<GameObject> m_signObj;
};