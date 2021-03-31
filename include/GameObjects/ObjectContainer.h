// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "Foundation/Helpers.h"

template <typename T>
class ObjectContainer {
public:
	ObjectContainer() : m_objects(std::vector<T>()) {}

	virtual void add(T object) {
		Helpers::addUnique(m_objects, object);
	}

	virtual void add(int count, ...) {
		va_list args;
		va_start(args, count);
		for (int i = 0; i < count; i++) {
			auto newEl = va_arg(args, T);
			add(newEl);
		}
		va_end(args);
	}

	virtual void remove(T object) {
		Helpers::remove(m_objects, object);
	}

protected:
	std::vector<T> m_objects;
};