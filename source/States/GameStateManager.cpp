// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include <iostream>
#include "States/GameStateManager.h"

using namespace std;

void GameStateManager::setState(const string stateName) {
	
	if (states.find(stateName) == states.end()) {
		cerr << "No suitable GameState found for " << stateName << endl;
		return;
	}
	
	std::shared_ptr<GameState> state = states[stateName];

	if (state != currentState) {
		if (currentState != nullptr) {
			currentState->exit();
		}
		currentState = state;
		currentState->init(*this);
	}
}

void GameStateManager::update() { currentState->update(); }

void GameStateManager::draw(sf::RenderWindow& window) { currentState->draw(window); }

void GameStateManager::addState(const std::string stateName, std::shared_ptr<GameState> state) {
	states.insert(pair<string, std::shared_ptr<GameState>>(stateName, state));
}
GameStateManager::GameStateManager() : currentState(nullptr) { }

GameStateManager::~GameStateManager() {
	clearStates();
}

void GameStateManager::clearStates() {
	states.clear();
	currentState = nullptr;
}