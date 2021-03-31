#pragma once
#include "pch.h"
#include <SFML/System/Vector2.hpp>

struct TransformPack {
	TransformPack(const sf::Vector2f& _position, const sf::Vector2f& _scale, const sf::Vector2f& _origin, float _angle) : position(_position), scale(_scale), origin(_origin), angle(_angle) {}
	const sf::Vector2f& position; 
	const sf::Vector2f& scale; 
	const sf::Vector2f& origin;
	float angle; 
};