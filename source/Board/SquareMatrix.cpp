// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Board/SquareMatrix.h"

SquareMatrix::SquareMatrix(size_t size, Squares squares)
	: m_size(size)
	, m_squares(squares)
{
}

SquareMatrix::Square& SquareMatrix::at(int column, int row)
{
	return m_squares[column + m_size * row];
}

const SquareMatrix::Squares& SquareMatrix::getSquares()
{
	return m_squares;
}

void SquareMatrix::pushBack(Square square)
{
	m_squares.push_back(square);
}
