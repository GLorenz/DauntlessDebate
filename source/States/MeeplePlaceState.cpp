// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "States/MeeplePlaceState.h"
#include "Board/Meeple.h"
#include "GameManager.h"
#include "Board/BoardComponent.h"
#include "States/PlayingState.h"
#include "Foundation/ResourceManager.h"
#include "Foundation/Helpers.h"
#include <algorithm>
#include <vector>
#include <random>

void MeeplePlaceState::init(std::function<void(IPlayingSubState*)> onSubStateFinished)
{
	m_finishedCallback = onSubStateFinished;
	m_p1ValidSquares = std::vector<std::shared_ptr<SquareComponent>>();
	m_p2ValidSquares = std::vector<std::shared_ptr<SquareComponent>>();

	// first player starts by placing one only
	m_placingsLeft = 1;

	splitPlayerSquaresHorizontal();
	m_curValidSqaures = &m_p1ValidSquares;
	highlightCurValidSquares();

	createAllMeeples();
	createNextPlacingDisplay();

	startObserveBoardClicks();
}

void MeeplePlaceState::observerUpdate(Event message)
{
	if (message.level == Event::SQUARE_LEVEL) {
		auto squareClicked = std::static_pointer_cast<SquareComponent>(message.object);
		
		// ignore if clicked square is not valid
		if (!Helpers::contains(*m_curValidSqaures, squareClicked))
			return;

		// placing pops meeple out of queue
		placeNextMeeple(squareClicked);
		--m_placingsLeft;

		// if all meeples are set, finish
		if (m_p1Meeples.empty() && m_p2Meeples.empty()) {
			finish();
		}
		else {
			// only switch player if no placings left
			if(m_placingsLeft <= 0)
				switchPlayers();

			updatePlacingDisplay();
		}
	}
}

void MeeplePlaceState::createAllMeeples()
{
	int maxHealth = ConfigHandler::getInstance().getMeepleMaxHealth();
	auto meepConfigs = ConfigHandler::getInstance().getMeepleConfigs();
	m_maxMeeps = meepConfigs.size() * 2;

	std::vector<std::shared_ptr<Meeple>> p1MeepleVector, p2MeepleVector;

	bool isPlayerOneMeep = true;
	for (size_t i = 0; i < m_maxMeeps; i++) {
		std::shared_ptr<GameObject> newMeep = std::make_shared<GameObject>("Meeple");

		size_t configIdx = i / 2;
		// Add meeple Components
		auto meepComp = newMeep->addComponent<Meeple>(
			std::get<0>(meepConfigs[configIdx]), // attack type
			std::get<1>(meepConfigs[configIdx]), // meeple type
			maxHealth,
			isPlayerOneMeep
			);

		// Add render Components
		std::string texName = std::get<2>(meepConfigs[configIdx]); // texture name
		auto spritePtr = ResourceManager::getInstance().getSprite(texName);
		m_meepSpriteBounds = spritePtr->getLocalBounds(); // TODO: do this right, bounds are needed to scale when placing

		newMeep->addComponent<RenderComponent>(spritePtr, Layer::Foreground);
		newMeep->setOrigin(spritePtr->getLocalBounds().width / 2, spritePtr->getLocalBounds().height / 2);
		auto circle = std::make_shared<sf::CircleShape>(sf::CircleShape(spritePtr->getLocalBounds().width / 2));
		sf::Vector2f spriteMid(circle->getLocalBounds().width / 2, circle->getLocalBounds().height / 2);
		circle->setOrigin(spriteMid);
		circle->move(spriteMid);
		circle->scale(0.7f, 0.7f);
		circle->setFillColor(isPlayerOneMeep ? ConfigHandler::getInstance().getP1Color() : ConfigHandler::getInstance().getP2Color());
		circle->setOutlineColor(sf::Color::Transparent);
		circle->setOutlineThickness(40.f);
		meepComp->setCircle(circle);
		newMeep->addComponent<RenderComponent>(circle, Layer::Objects);

		auto rectangle = std::make_shared<sf::RectangleShape>(sf::Vector2f((float)spritePtr->getLocalBounds().width, (float)spritePtr->getLocalBounds().height));
		rectangle->setOrigin(spriteMid);
		rectangle->move(spriteMid);
		meepComp->setDisabledRect(rectangle);
		meepComp->enableAttacking();
		newMeep->addComponent<RenderComponent>(rectangle, Layer::UI);

		auto hpCircle = std::make_shared<sf::CircleShape>(spritePtr->getLocalBounds().width / 2);
		hpCircle->scale(0.4f, 0.4f);
		hpCircle->setFillColor(sf::Color::White);
		newMeep->addComponent<RenderComponent>(hpCircle, Layer::Foreground);

		auto hpText = std::make_shared<sf::Text>("8", *ResourceManager::getInstance().getFont("ptsans_regular"), 100u);
		
		hpText->setFillColor(sf::Color::Black);
		newMeep->addComponent<RenderComponent>(hpText, Layer::UI);
		sf::Vector2f textMid(hpText->getLocalBounds().width, hpText->getLocalBounds().height);
		sf::Vector2f circleSize(hpCircle->getGlobalBounds().width, hpCircle->getGlobalBounds().height);
		hpText->setPosition(circleSize.x / 3 - textMid.x / 2, circleSize.y / 3 - textMid.y / 2);
		meepComp->setHpText(hpText);

		newMeep->setActive(false);
		GameManager::getInstance().getBoard()->addMeeple(newMeep);

		if (isPlayerOneMeep)
			p1MeepleVector.push_back(meepComp);
		else
			p2MeepleVector.push_back(meepComp);

		isPlayerOneMeep = !isPlayerOneMeep;
	}

	std::random_device rd;
	std::shuffle(p1MeepleVector.begin(), p1MeepleVector.end(), rd);
	std::shuffle(p2MeepleVector.begin(), p2MeepleVector.end(), rd);

	for (auto m1 : p1MeepleVector)
		m_p1Meeples.push(m1);
	for (auto m2 : p2MeepleVector)
		m_p2Meeples.push(m2);
}

void MeeplePlaceState::placeNextMeeple(std::shared_ptr<SquareComponent> square)
{
	auto meep = m_p1sTurn ? m_p1Meeples.front() : m_p2Meeples.front();

	ResourceManager::getInstance().getSound("button")->play();
	meep->move(square);
	meep->scaleToCurrentSquare(m_meepSpriteBounds);
	meep->getGameObject().setActive(true);

	if (m_p1sTurn)
		m_p1Meeples.pop();
	else
		m_p2Meeples.pop();
}

void MeeplePlaceState::switchPlayers()
{
	if (m_curValidSqaures == &m_p1ValidSquares) {
		m_curValidSqaures = &m_p2ValidSquares;
	}
	else {
		m_curValidSqaures = &m_p1ValidSquares;
	}

	m_p1sTurn = !m_p1sTurn;
	m_placingsLeft = 2;

	highlightCurValidSquares();
}

void MeeplePlaceState::createNextPlacingDisplay()
{
	m_uiObj = std::make_shared<GameObject>("ui items");
	m_playerUI = std::make_shared<sf::Text>("", *ResourceManager::getInstance().getFont("ptsans_regular"), 40u);
	m_playerUI->setOutlineColor(sf::Color::Black);
	m_playerUI->setOutlineThickness(4.0);

	m_placeUI = std::make_shared<sf::Text>("", *ResourceManager::getInstance().getFont("ptsans_regular"), 20u);
	m_placeUI->setOutlineColor(sf::Color::Black);
	m_placeUI->setOutlineThickness(4.0);
	m_uiObj->addComponent<RenderComponent>(m_playerUI, Layer::UI);
	m_uiObj->addComponent<RenderComponent>(m_placeUI, Layer::UI);

	auto screenSize = GameManager::getInstance().getWindowSize();
	m_nextMeepPosition = sf::Vector2f(screenSize.x * 21 / 24.f, screenSize.y / 2.f);
	m_nextMeepScale = sf::Vector2f(.75f, .75f);

	updatePlacingDisplay();
}

void MeeplePlaceState::updatePlacingDisplay()
{
	sf::IntRect boardRect = GameManager::getInstance().getBoard()->getBoardRect();
	sf::Vector2u screenSize = GameManager::getInstance().getWindowSize();

	m_playerUI->setString(m_p1sTurn ? "Player One" : "Player Two");
	m_playerUI->setFillColor(m_p1sTurn ? ConfigHandler::getInstance().getP1Color() : ConfigHandler::getInstance().getP2Color());

	m_playerUI->setPosition(sf::Vector2f((boardRect.left + boardRect.width) +
		(screenSize.x - (boardRect.left + boardRect.width)) / 2.f - m_playerUI->getLocalBounds().width / 2.f,
		boardRect.top + screenSize.y / 22.f));

	m_placeUI->setString("Place your next debater!");
	m_placeUI->setPosition(sf::Vector2f((boardRect.left + boardRect.width) +
		(screenSize.x - (boardRect.left + boardRect.width)) / 2.f - m_placeUI->getLocalBounds().width / 2.f,
		boardRect.top + screenSize.y / 10.f));
	m_placeUI->setFillColor(m_p1sTurn ? ConfigHandler::getInstance().getP1Color() : ConfigHandler::getInstance().getP2Color());

	auto meep = m_p1sTurn ? m_p1Meeples.front() : m_p2Meeples.front();

	meep->getGameObject().setPosition(m_nextMeepPosition);
	meep->getGameObject().setScale(m_nextMeepScale);
	meep->getGameObject().setActive(true);
}

void MeeplePlaceState::startObserveBoardClicks()
{
	GameManager::getInstance().getBoard()->getClickManager().addObserver(*this);
}

void MeeplePlaceState::stopObserveBoardClicks()
{
	GameManager::getInstance().getBoard()->getClickManager().removeObserver(*this);
}

void MeeplePlaceState::splitPlayerSquaresHorizontal()
{
	auto board = GameManager::getInstance().getBoard();
	int boardSize = board->getSize();
	int splittingCol = boardSize / 2; // using int division

	for (int col = 0; col < splittingCol; ++col) {
		for (int row = 0; row < boardSize; ++row) {
			m_p1ValidSquares.emplace_back(board->getSquareAt(col, row));
		}
	}

	for (int col = boardSize - 1; col >= boardSize - splittingCol; --col) {
		for (int row = 0; row < boardSize; ++row) {
			m_p2ValidSquares.emplace_back(board->getSquareAt(col, row));
		}
	}
}

void MeeplePlaceState::highlightCurValidSquares()
{
	GameManager::getInstance().getSquareHighlighter().highlight(*m_curValidSqaures);
}

void MeeplePlaceState::update()
{
}

void MeeplePlaceState::draw(sf::RenderWindow& window)
{
	// display some info on how to place meeples
}

void MeeplePlaceState::abort()
{
	GameManager::getInstance().getHighlighter().toDefault();
	GameManager::getInstance().getSquareHighlighter().toDefault();
	stopObserveBoardClicks();
}

void MeeplePlaceState::finish()
{
	// release everything, then call regular ending of state
	abort();
	m_finishedCallback(this);
}
