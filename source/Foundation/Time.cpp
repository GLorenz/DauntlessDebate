// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Foundation/Time.h"

float Time::m_deltaTime = 0.f;

void Time::onNewFrame() {
	static sf::Clock clock;
	sf::Time deltaTime = clock.restart();
	m_deltaTime = deltaTime.asSeconds();
}

const float Time::delta() {
	return m_deltaTime;
}