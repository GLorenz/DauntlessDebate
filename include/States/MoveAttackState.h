// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/IPlayingSubState.h"
#include "GameObjects/GameObject.h"
#include "States/Rounds/RoundManager.h"

class MoveAttackState : public IPlayingSubState {
public:
	virtual void update() override;
	virtual void draw(sf::RenderWindow& window) override;
	virtual void init(std::function<void(IPlayingSubState*)> onSubStateFinished) override;
	virtual void abort() override;

	virtual ~MoveAttackState() override = default;

	void startNextRound();
	void onRoundFinished();
	void updateUI();

private:
	void finish();

	RoundManager m_roundManager;
	std::function<void(IPlayingSubState*)> m_finishedCallback;

	int m_maxRounds;
	int m_roundCounter;
	std::shared_ptr<GameObject> m_roundObj;
	std::shared_ptr<sf::Text> m_roundText;
};