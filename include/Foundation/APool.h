// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include <queue>

template <class T>
class APool {
public:
	APool(int size, size_t enlargeSize = 4)
		: m_Queue(std::deque<T>()) // 0 bc children fill queue in constructor with their createElement()
		, m_enlargeSize(enlargeSize)
	{
	}

	T borrowElement() {
		if (m_Queue.size() == 0) {
			enlarge();
		}

		// move since unique_ptr cannot be copied
		auto retPtr = std::move(m_Queue.front());
		// remove first elemet from queue
		m_Queue.pop();
		return retPtr;
	}

	// not sure if i can rely on clients to return sector after using
	void returnElement(T element) {
		// reset sector
		onReturnElement(element);
		m_Queue.push(std::move(element));
	}

	virtual ~APool() = default;

protected:
	virtual T createElement() = 0;
	virtual void onReturnElement(T& el) = 0;

	virtual void enlarge() {
		for (size_t i = 0; i < m_enlargeSize; ++i) {
			m_Queue.push(std::move(createElement()));
		}

#ifdef _DEBUG
		std::cout << "increased pool size" << std::endl;
#endif
	}

	size_t m_enlargeSize;
	std::queue<T> m_Queue;
};