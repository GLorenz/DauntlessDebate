// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/GameStateManager.h"
#include <functional>

class PlayingState;

class IPlayingSubState {
public:
	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void init(std::function<void(IPlayingSubState*)> onSubStateFinished) = 0;
	virtual void abort() = 0;

	virtual ~IPlayingSubState() = default;
};