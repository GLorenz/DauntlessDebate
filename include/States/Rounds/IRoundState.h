// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include <functional>

class IRoundState {
public:
	using finishCallback = std::function<void(IRoundState*)>;

	virtual void start(bool playerOneActive, finishCallback callBack) = 0;
	virtual void abort() = 0;

	virtual void update() = 0;

	virtual ~IRoundState() = default;
};