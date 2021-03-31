// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"

enum class Layer : char {
	Background,
	BackgroundDetails,
	Objects,
	Foreground,
	UI,
	SelectionBackground,
	SelectionObject,
	SelectionForeground,
	SelectionUI,

	Default = Objects
};