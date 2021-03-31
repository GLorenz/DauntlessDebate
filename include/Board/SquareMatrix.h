// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "GameObjects/Components/SquareComponent.h"

class SquareMatrix {
public:
	using Square = std::shared_ptr<SquareComponent>;
	using Squares = std::vector<Square>;

	SquareMatrix(size_t size, Squares squares);

	Square& at(int column, int row);
	const Squares& getSquares();
	void pushBack(Square square);

private:
	size_t m_size;
	Squares m_squares;
};