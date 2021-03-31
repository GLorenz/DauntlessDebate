// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Board/SquareHighlighter.h"

SquareHighlighter::SquareHighlighter() : m_curHighlights() { }

void SquareHighlighter::toDefault()
{
	toDefault(m_curHighlights);
}

void SquareHighlighter::toDefault(const SquaresRef squares)
{
	for (auto square : squares) {
		square->setStatus(SquareComponent::Status::Default);
	}
	m_curHighlights.clear();
}

void SquareHighlighter::highlight(const SquaresRef squares)
{
	if (m_curHighlights.size() > 0) {
		toDefault(m_curHighlights);
	}

	for (auto square : squares) {
		square->setStatus(SquareComponent::Status::MoveHighlight);
		m_curHighlights.push_back(square);
	}
}

void SquareHighlighter::damage(const SquaresRef squares)
{
	if (m_curHighlights.size() > 0) {
		toDefault(m_curHighlights);
	}

	for (auto square : squares) {
		square->setStatus(SquareComponent::Status::DamageHighlight);
		m_curHighlights.push_back(square);
	}
}