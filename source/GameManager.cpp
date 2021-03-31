// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "GameManager.h"

GameManager::GameManager()
    : m_squareHighlighter()
    , m_playerScores()
{
}

GameManager& GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

SquareHighlighter& GameManager::getSquareHighlighter()
{
    return m_squareHighlighter;
}

Highlighter& GameManager::getHighlighter()
{
    return m_highlighter;
}

std::shared_ptr<BoardComponent> GameManager::getBoard()
{
    return m_board;
}

void GameManager::setBoard(std::shared_ptr<BoardComponent> board)
{
    m_board = board;
}

void GameManager::resetBoard()
{
    m_board.reset();
}

sf::Vector2u GameManager::getWindowSize()
{
    return m_windowSize;
}

void GameManager::setWindowSize(sf::Vector2u size)
{
    m_windowSize = size;
}

void GameManager::setPlayerScores(sf::Vector2i scores)
{
    m_playerScores = scores;
}

sf::Vector2i GameManager::getPlayerScores()
{
    return m_playerScores;
}

void GameManager::setGui(std::shared_ptr<tgui::Gui> gui)
{
    m_gui = gui;
}

std::shared_ptr<tgui::Gui> GameManager::getGui()
{
    return m_gui;
}

void GameManager::setWindow(std::shared_ptr<sf::RenderWindow> window)
{
    m_window = window;
}

std::shared_ptr<sf::RenderWindow> GameManager::getWindow()
{
    return m_window;
}
