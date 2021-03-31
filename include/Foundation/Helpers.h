// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include <stdarg.h>

namespace Helpers
{
	static constexpr float PI = 3.1415926536f;
	static constexpr float Rad2Deg = (360.f / (PI * 2));

	sf::Vector2f getDirection(float rotation);
	float getLength(sf::Vector2f vec);
	sf::Vector2f normalize(sf::Vector2f vec);
	float dotProduct(sf::Vector2f left, sf::Vector2f right);
	float getAngle(sf::Vector2f vec1, sf::Vector2f vec2);

	float lerp(float a, float b, float f);

	template <class C, class T>
	bool contains(C&& container, T&& object) {
		auto found = std::find(container.begin(), container.end(), object);
		return (found != container.end());
	}

	// convert some enumerator to underlying type
	// used for sorting by Layer in RenderManager (Layer enum -> numerical value)
	// copied from Effective Modern C++ by Scott Meyers. P73, Section 11 Scoped Enums
	template <typename E>
	constexpr auto toUType(E enumerator) noexcept
	{
		return static_cast<std::underlying_type_t<E>>(enumerator);
	}

	template <class C, class T>
	void addUnique(C&& container, T&& object) {
		auto found = std::find(container.begin(), container.end(), object);
		if (found == container.end())
			container.push_back(object);
	}

	template <class C, class T>
	void remove(C&& container, T&& object) {
		auto found = std::find(container.begin(), container.end(), object);
		if (found != container.end())
			container.erase(found);
	}
};