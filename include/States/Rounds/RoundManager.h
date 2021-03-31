// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/Rounds/IRoundState.h"
#include "GameObjects/GameObject.h"
#include "PatternRenderer.h"

class RoundManager {
public:
	using Action = std::function<void()>;

	RoundManager();
	virtual ~RoundManager();

	void update();

	void startNewRound(Action onRoundFinishedCB);
	void onStateFinished(IRoundState* finishedState);

private:
	void startNextState();
	void onRoundFinished();

	void createTransitionText();
	void startTransition(bool isMovingState);
	void onTransitionComplete();
	bool m_inTransition;
	float m_curTransScale;
	float m_targetScale;
	float m_transSpeed;

	bool m_player1IsActive;

	Action m_onRoundFinishedCB;

	std::unique_ptr<IRoundState> m_stateAfterTransition;
	std::unique_ptr<IRoundState> m_state;

	std::unique_ptr<GameObject> m_transitionTextObj;
	std::shared_ptr<sf::Text> m_transitionText;

	PatternRenderer m_patternRend;
	PatternRenderer m_patternRendNext;
	int m_currRandomPatternIdx;
	int m_nextRandomPatternIdx;
	tgui::Group::Ptr m_gui;
};