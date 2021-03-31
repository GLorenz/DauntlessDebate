// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "Interfaces/IObservable.h"

template <typename T>
class IObserver {
public:
	virtual void observerUpdate(T message) = 0;
};