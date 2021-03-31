// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"

class GameStateManager;

class GameState {
public:
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void init(GameStateManager& manager) = 0;
	virtual void exit() = 0;
};