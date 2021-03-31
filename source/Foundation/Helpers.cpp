// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Foundation/Helpers.h"

namespace Helpers
{
	float getLength(sf::Vector2f vec)
	{
		return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
	}

	sf::Vector2f getDirection(float rotation)
	{
		sf::Vector2f direction =
		{
			(float)sin(rotation * PI / 180),
			(float)-cos(rotation * PI / 180)
		};
		return normalize(direction);
	}

	sf::Vector2f normalize(sf::Vector2f vec)
	{
		float length = getLength(vec);
		return sf::Vector2f(vec.x / length, vec.y / length);
	}
	float dotProduct(sf::Vector2f left, sf::Vector2f right)
	{
		return left.x * right.x + left.y * right.y;
	}
	float getAngle(sf::Vector2f vec1, sf::Vector2f vec2)
	{
		float dot = dotProduct(vec1, vec2);
		float det = vec1.x * vec2.y - vec1.y * vec2.x; // determinant
		return abs(atan2(det, dot) - PI); // # atan2(y, x) or atan2(sin, cos)
		//return acosf(dotProduct(vec1, vec2) / (getLength(vec1) * getLength(vec2)));
	}
	float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
}
