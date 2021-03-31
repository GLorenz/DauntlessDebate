// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include <vector>

template <typename T>
class IObserver;

template <typename T>
class IObservable {
public:
	virtual void addObserver(IObserver<T>& observer) = 0;
	virtual void removeObserver(IObserver<T>& observer) = 0;
	virtual void notify(T msg) = 0;
};