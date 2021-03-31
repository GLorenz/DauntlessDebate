// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"

class Time {
public:
	Time() = delete;
	static void onNewFrame();
	static const float delta(); // use extra delta function to make it const
private:
	static float m_deltaTime;
};