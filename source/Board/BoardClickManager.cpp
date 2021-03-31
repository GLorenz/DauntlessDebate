// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Board/BoardClickManager.h"
#include "Foundation/InputManager.h"
#include "Board/BoardComponent.h"
#include "Foundation/Helpers.h"
#include <Foundation\ResourceManager.h>

BoardClickManager::BoardClickManager(BoardComponent& board)
	: m_board(board)
	, m_inputActionName("mouseLeftClick")
	, m_observers()
{
	registerToInput();
}


void BoardClickManager::update()
{
	if (InputManager::getInstance().isPressed(m_inputActionName, 0))
	{
		onClick();
	}
}

void BoardClickManager::onClick()
{
	Event e;
	e.level = Event::MOUSE_LEVEL;
	e.object = nullptr;

	auto mousePos = InputManager::getInstance().getMousePos();
	auto squarePtr = m_board.getSquareAt(mousePos);

	if (squarePtr != nullptr) {
		e.level = Event::SQUARE_LEVEL;
		e.object = squarePtr;

		auto meep = m_board.getMeepleOnSquare(squarePtr);

		if (meep != nullptr) {
			e.level = Event::MEEPLE_LEVEL;
			e.object = meep;
		}
	}
	// TODO: remove this
	//std::cout << "Event: Level " << e.level << std::endl;
	notify(e);
}

void BoardClickManager::registerToInput()
{
	InputManager::getInstance().bind(m_inputActionName, sf::Mouse::Left, 0);
}

BoardClickManager::~BoardClickManager()
{
	// TODO: figure out why this crashes
	//InputManager::getInstance().unbind(m_inputActionName, 0);
}

void BoardClickManager::addObserver(IObserver<Event> & observer)
{
	Helpers::addUnique(m_observers, &observer);
}
void BoardClickManager::removeObserver(IObserver<Event> & observer)
{
	Helpers::remove(m_observers, &observer);
}
void BoardClickManager::notify(Event msg)
{
	for (auto* obs : m_observers) {
		obs->observerUpdate(msg);
	}
}