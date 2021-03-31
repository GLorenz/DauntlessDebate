// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "Interfaces/IObservable.h"
#include "Interfaces/IObserver.h"
#include <unordered_map>
#include <bitset>

class BoardComponent;
class SquareComponent;

struct Event {
	static const size_t MOUSE_LEVEL = 0;
	static const size_t SQUARE_LEVEL = 1;
	static const size_t MEEPLE_LEVEL = 2;

	size_t level;
	std::shared_ptr<void> object; //using void pointer for now (can be anything)
};

class BoardClickManager : IObservable<Event> {
public:
	using Square = std::shared_ptr<SquareComponent>;
	using Squares = std::vector<Square>;

	BoardClickManager(BoardComponent& board);
	~BoardClickManager();
	
	BoardClickManager(const BoardClickManager& rhv) = delete;
	BoardClickManager& operator=(const BoardClickManager& rhv) = delete;

	void update();

	virtual void addObserver(IObserver<Event>& observer) override;
	virtual void removeObserver(IObserver<Event>& observer) override;
	virtual void notify(Event msg) override;

private:
	void registerToInput();
	void onClick();

	BoardComponent& m_board;
	std::string m_inputActionName;

	std::vector<IObserver<Event>*> m_observers;
};