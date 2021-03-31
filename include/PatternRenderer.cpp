#include "pch.h"
#include "PatternRenderer.h"
#include "GameManager.h"
#include "Foundation/ResourceManager.h"

PatternRenderer::PatternRenderer()
	: m_pattern()
{
	m_guiGroup = tgui::Group::create();
	GameManager::getInstance().getGui()->add(m_guiGroup);
}

void PatternRenderer::setPattern(Pattern newPattern)
{
	m_pattern = newPattern;
	updateRendering();
}

void PatternRenderer::remove()
{
	m_guiGroup->removeAllWidgets();
	GameManager::getInstance().getGui()->remove(m_guiGroup);
}

void PatternRenderer::setSize(tgui::Layout2d size)
{
	m_guiGroup->setSize(size);
	updateRendering();
}

void PatternRenderer::setPosition(tgui::Layout2d pos)
{
	m_guiGroup->setPosition(pos);
	updateRendering();
}

void PatternRenderer::updateRendering()
{
	int cols = m_pattern.size();
	if (cols == 0) return;

	int rows = m_pattern[0].size();

	m_guiGroup->removeAllWidgets();

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			float sizeX = m_guiGroup->getSize().x / cols;
			float sizeY = m_guiGroup->getSize().y / rows;
			float posX = sizeX * col;
			float posY = sizeY * row;

			tgui::Picture::Ptr tex;
			if (m_pattern[col][row] == 2) {
				// Add a meeple

				auto meep = tgui::Picture::create(*ResourceManager::getInstance().getTexture("randomA"));
				meep->setSize(sizeX, sizeY);
				meep->setPosition(posX, posY);
				m_guiGroup->add(meep);

				tex = tgui::Picture::create(*ResourceManager::getInstance().getTexture("tile"));
			}
			else if (m_pattern[col][row] == 1) {
				// Add a attack tile
				tex = tgui::Picture::create(*ResourceManager::getInstance().getTexture("tile_damage"));
			}
			else if (m_pattern[col][row] == 0) {
				// add a empty tile
				tex = tgui::Picture::create(*ResourceManager::getInstance().getTexture("tile"));
			}

			tex->setSize(sizeX, sizeY);
			tex->setPosition(posX, posY);
			m_guiGroup->add(tex);
			tex->moveToBack();
		}
	}
}
