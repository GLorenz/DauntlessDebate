// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/Rounds/IRoundState.h"
#include "Board/BoardClickManager.h"
#include "Board/Meeple.h"
#include "Foundation/AttackDir.h"

class AttackRoundState : public IRoundState, IObserver<Event> {
public:
	using MeeplePTR = std::shared_ptr<Meeple>;
	using Meeples = std::vector<MeeplePTR>;

	enum class Status { Waiting, MeeplePickedUp };

	AttackRoundState();
	virtual ~AttackRoundState() override = default;

	virtual void start(bool playerOneActive, finishCallback callBack) override;
	virtual void abort() override;
	virtual void update() override;

	virtual void observerUpdate(Event message) override;

protected:
	finishCallback m_finishCallback;

private:
	void finish();

	void startObserveBoardClicks();
	void stopObserveBoardClicks();

	void resetSelectedMeeple();
	void setSelectedMeeple(std::shared_ptr<Meeple> meeple);

	void attackWithSelected();
	AttackDir getAttackDirOfAngle(float angle);

	void updateUI();

	Meeples m_alreadyAttackedMeeps;
	MeeplePTR m_selectedMeeple;
	Status m_status;

	const sf::Vector2f down = sf::Vector2f(0.f, 1.f);
	AttackDir m_curAttackDir;
	Meeples m_playerMeeples;
	std::vector<std::shared_ptr<SquareComponent>> m_curAttackOptions;

	bool m_isPlayerOneActive;
	std::shared_ptr<GameObject> m_uiObj;
	std::shared_ptr<sf::Text> m_playerUI;
	std::shared_ptr<sf::Text> m_attackUI;
};