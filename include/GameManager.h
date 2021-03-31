// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/Rounds/RoundManager.h"
#include "Board/SquareHighlighter.h"
#include "Foundation/Highlighter.h"
#include <TGUI/TGUI.hpp>

class GameManager {
public:
	static GameManager& getInstance();

	SquareHighlighter& getSquareHighlighter();
	Highlighter& getHighlighter();

	std::shared_ptr<BoardComponent> getBoard();
	void setBoard(std::shared_ptr<BoardComponent> board);
	void resetBoard();

	sf::Vector2u getWindowSize();
	void setWindowSize(sf::Vector2u);

	void setPlayerScores(sf::Vector2i scores);
	sf::Vector2i getPlayerScores();

	void setGui(std::shared_ptr<tgui::Gui> gui);
	std::shared_ptr<tgui::Gui> getGui();

	void setWindow(std::shared_ptr<sf::RenderWindow> window);
	std::shared_ptr<sf::RenderWindow> getWindow();

private:
	GameManager();

	sf::Vector2i m_playerScores;
	sf::Vector2u m_windowSize;
	std::shared_ptr<tgui::Gui> m_gui;
	std::shared_ptr<sf::RenderWindow> m_window;
	std::shared_ptr<BoardComponent> m_board;
	SquareHighlighter m_squareHighlighter;
	Highlighter m_highlighter;
};