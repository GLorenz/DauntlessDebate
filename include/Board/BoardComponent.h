// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "Board/Meeple.h"
#include "GameObjects/Components/SquareComponent.h"
#include "GameObjects/GameObject.h"
#include "Board/BoardClickManager.h"
#include "Board/Meeple.h"

class BoardClickManager;

class BoardComponent : public GameComponent {
public:
	using Square = std::shared_ptr<SquareComponent>;
	using Squares = std::vector<Square>;
	using MeeplePTR = std::shared_ptr<Meeple>;

	BoardComponent(GameObject& gameObject, int size, sf::IntRect boardBounds);

	void update() override;

	int getSize();

	Square getSquareAt(int column, int row);
	Square getSquareAt(sf::Vector2i absolutePosition);

	Squares getNeighbourSquares(Square square);
	Squares getFreeNeighbourSquares(Square square);
	
	std::vector<MeeplePTR> getMeeplesOfPlayer(bool ofPlayerOne);
	std::vector<MeeplePTR>& getAllMeeples();
	MeeplePTR getMeepleOnSquare(Square square);
	void addMeeple(std::shared_ptr<GameObject> meepleObject);
	void countMeeples(int& meeplesP1, int& meeplesP2);

	BoardClickManager& getClickManager();
	void setMeeplesActive(bool toActive);

	bool bothHaveMeeples();

	sf::IntRect getBoardRect();

private:
	void addMeeple(MeeplePTR meeple);
	int getVectorIdxFromRowCol(int column, int row);

	BoardClickManager m_clickManager;
	Squares m_squares;
	std::vector<MeeplePTR> m_meeples;
	std::vector<std::shared_ptr<GameObject>> m_meepleObjects;
	int m_size; // int because of easier comparison to other ints
	sf::IntRect m_boardArea;
};