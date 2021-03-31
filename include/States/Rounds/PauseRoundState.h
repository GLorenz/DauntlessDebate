// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/Rounds/IRoundState.h"

class PauseRoundState : public IRoundState {
public:
	virtual void start(bool playerOneActive, finishCallback callBack) override;
	virtual void abort() override;
	virtual void update() override;
};