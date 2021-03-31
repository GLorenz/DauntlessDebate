// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/IPlayingSubState.h"
#include "GameObjects/GameObject.h"
#include "Foundation/ConfigHandler.h"
#include "Board/BoardClickManager.h"
#include "GameObjects/Components/SquareComponent.h"
#include "GameObjects/Components/RenderComponent.h"
#include <queue>

class MeeplePlaceState : public IPlayingSubState, IObserver<Event> {
public:
	virtual void update() override;
	virtual void draw(sf::RenderWindow& window) override;
	virtual void init(std::function<void(IPlayingSubState*)> onSubStateFinished) override;
	virtual void abort() override;

	virtual ~MeeplePlaceState() override = default;

	virtual void observerUpdate(Event message) override;
private:
	void finish();

	void createAllMeeples();
	void placeNextMeeple(std::shared_ptr<SquareComponent> square);
	void switchPlayers();

	void startObserveBoardClicks();
	void stopObserveBoardClicks();

	void splitPlayerSquaresHorizontal();
	void highlightCurValidSquares();

	void createNextPlacingDisplay();
	void updatePlacingDisplay();

	std::vector<std::shared_ptr<SquareComponent>>* m_curValidSqaures;
	std::vector<std::shared_ptr<SquareComponent>> m_p1ValidSquares;
	std::vector<std::shared_ptr<SquareComponent>> m_p2ValidSquares;

	sf::Vector2f m_nextMeepPosition;
	sf::Vector2f m_nextMeepScale;

	std::queue<std::shared_ptr<Meeple>> m_p1Meeples;
	std::queue<std::shared_ptr<Meeple>> m_p2Meeples;
	int m_placingsLeft;
	bool m_p1sTurn;

	sf::FloatRect m_meepSpriteBounds;
	size_t m_maxMeeps;

	std::shared_ptr<GameObject> m_uiObj;
	std::shared_ptr<sf::Text> m_playerUI;
	std::shared_ptr<sf::Text> m_placeUI;
	bool m_initiated = false;

	std::function<void(IPlayingSubState*)> m_finishedCallback;
};