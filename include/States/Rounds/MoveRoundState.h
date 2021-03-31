// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/Rounds/IRoundState.h"
#include "Board/BoardClickManager.h"
#include "Board/Meeple.h"
#include "GameObjects/Components/SquareComponent.h"

class MoveRoundState : public IRoundState, IObserver<Event> {
public:
	virtual void start(bool playerOneActive, finishCallback callBack) override;
	virtual void abort() override;
	virtual void update() override;

	enum class Status { Waiting, MeeplePickedUp };

	virtual ~MoveRoundState() override = default;

protected:
	finishCallback m_finishCallback;

private:
	void finishState();

	void resetSelectedMeeple();
	void setSelectedMeeple(std::shared_ptr<Meeple> meeple);
	void updateCurMoveOptions();
	void highlightCurMoveOptions();

	void startObserveBoardClicks();
	void stopObserveBoardClicks();

	void resetMoveCount();
	void moveSelectedMeeple(std::shared_ptr<SquareComponent> square);

	virtual void observerUpdate(Event message) override;

	void updateUI();

	std::shared_ptr<Meeple> m_selectedMeeple;
	std::vector<std::shared_ptr<SquareComponent>> m_curMoveOptions;

	size_t m_curMoveCount;
	Status m_status;
	bool m_isPlayerOneActive;
	std::shared_ptr<GameObject> m_uiObj;
	std::shared_ptr<sf::Text> m_playerUI;
	std::shared_ptr<sf::Text> m_movesUI;
};