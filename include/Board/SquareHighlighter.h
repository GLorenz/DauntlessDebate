// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "Board/BoardComponent.h"

class SquareHighlighter {
public:
	using Squares = std::vector<BoardComponent::Square>;
	using SquaresRef = Squares&;

	SquareHighlighter();

	void toDefault();
	void toDefault(const SquaresRef squares);
	void highlight(const SquaresRef squares);
	void damage(const SquaresRef squares);

private:
	Squares m_curHighlights;
};