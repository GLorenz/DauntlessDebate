// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include <string>
#include <map>
#include "States/GameState.h"

class GameStateManager {
public:
	GameStateManager();
	~GameStateManager();
	GameStateManager(const GameStateManager& m) = delete;
	GameStateManager& operator=(const GameStateManager& rhv) = delete;

	void update();
	void draw(sf::RenderWindow& window); // not const RenderWindow since draw() is not const
	void setState(const std::string stateName);
	void addState(const std::string stateName, std::shared_ptr<GameState> state);

private:
	std::map<std::string, std::shared_ptr<GameState>> states; // map of all available states
	std::shared_ptr<GameState> currentState; // holds the current state
	void clearStates();
};