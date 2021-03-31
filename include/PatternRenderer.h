#pragma once
#include "pch.h"
#include <TGUI/TGUI.hpp>

class PatternRenderer
{
public:
	using Pattern = std::vector<std::vector<int>>;

	PatternRenderer();
	void setPattern(Pattern newPattern);
	void remove();
	void setSize(tgui::Layout2d size);
	void setPosition(tgui::Layout2d pos);

private:
	void updateRendering();
	Pattern m_pattern;
	tgui::Group::Ptr m_guiGroup;
};