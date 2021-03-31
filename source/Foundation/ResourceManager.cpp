// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Foundation/ResourceManager.h"

ResourceManager::ResourceManager() {
	loadDefaults();
}

void ResourceManager::loadDefaults() {
	loadFontFromFile("ptsans_regular", "../assets/PTSans-Regular.ttf");
	loadFontFromFile("ptsans_bold", "../assets/PTSans-Bold.ttf");

	std::string bgName = "background";
	loadTextureFromFile(bgName, "../assets/background.png");
	loadSpriteWithTexture("background", bgName);

	std::string bgTitleName = "backgroundTitle";
	loadTextureFromFile(bgTitleName, "../assets/backgroundTitle.png");
	loadSpriteWithTexture("backgroundTitle", bgTitleName);

	std::string tutorialName = "tutorial";
	loadTextureFromFile(tutorialName, "../assets/tutorial.png");
	loadSpriteWithTexture("tutorial", tutorialName);

	std::string tutorialCharName = "tutorialChar";
	loadTextureFromFile(tutorialCharName, "../assets/tutorialCharacters.png");
	loadSpriteWithTexture("tutorialChar", tutorialCharName);

	std::string tutorialDebateStylesName = "tutorialDebateStyles";
	loadTextureFromFile(tutorialDebateStylesName, "../assets/tutorialDebateStyles.png");
	loadSpriteWithTexture("tutorialDebateStyles", tutorialDebateStylesName);

	std::string titleName = "title";
	loadTextureFromFile(titleName, "../assets/title.png");
	loadSpriteWithTexture("title", titleName);

	std::string logoName = "logo";
	loadTextureFromFile(logoName, "../assets/mmt_logo.png");
	loadSpriteWithTexture("logo", logoName);

	std::string effectivenessName = "effectiveness";
	loadTextureFromFile(effectivenessName, "../assets/effectiveness.png");
	loadSpriteWithTexture("effectiveness", effectivenessName);

	std::string rulesName = "rules";
	loadTextureFromFile(rulesName, "../assets/rules.png");
	loadSpriteWithTexture("rules", rulesName);

	std::string signName = "sign";
	loadTextureFromFile(signName, "../assets/sign.png");
	loadSpriteWithTexture("sign", signName);

	std::string tile = "tile";
	loadTextureFromFile(tile, "../assets/tile.png");
	loadSpriteWithTexture("tile",tile);

	std::string tile_highlight = "tile_highlight";
	loadTextureFromFile(tile_highlight, "../assets/tile_highlight.png");
	loadSpriteWithTexture(tile_highlight, tile_highlight);

	std::string tile_damage = "tile_damage";
	loadTextureFromFile(tile_damage, "../assets/tile_damage.png");
	loadSpriteWithTexture(tile_damage, tile_damage);

	// melee icons
	std::string meleeA = "meleeA";
	loadTextureFromFile(meleeA, "../assets/meleeA.png");
	loadSpriteWithTexture("meleeA", meleeA);

	std::string meleeB = "meleeB";
	loadTextureFromFile(meleeB, "../assets/meleeB.png");
	loadSpriteWithTexture("meleeB", meleeB);

	std::string meleeC = "meleeC";
	loadTextureFromFile(meleeC, "../assets/meleeC.png");
	loadSpriteWithTexture("meleeC", meleeC);


	// ranged icons
	std::string rangedA = "rangedA";
	loadTextureFromFile(rangedA, "../assets/rangedA.png");
	loadSpriteWithTexture("rangedA", rangedA);

	std::string rangedB = "rangedB";
	loadTextureFromFile(rangedB, "../assets/rangedB.png");
	loadSpriteWithTexture("rangedB", rangedB);

	std::string rangedC = "rangedC";
	loadTextureFromFile(rangedC, "../assets/rangedC.png");
	loadSpriteWithTexture("rangedC", rangedC);

	// random icons
	std::string randomA = "randomA";
	loadTextureFromFile(randomA, "../assets/randomA.png");
	loadSpriteWithTexture("randomA", randomA);

	std::string randomB = "randomB";
	loadTextureFromFile(randomB, "../assets/randomB.png");
	loadSpriteWithTexture("randomB", randomB);

	std::string randomC = "randomC";
	loadTextureFromFile(randomC, "../assets/randomC.png");
	loadSpriteWithTexture("randomC", randomC);

	// load sounds and music
	std::string buttonSound = "button";
	loadSoundFromFile(buttonSound, "../assets/button.wav");

	// TODO change this to attack sound
	std::string attackSound = "attack";
	loadSoundFromFile(attackSound, "../assets/button.wav");

	// TODO change this to move / shuffling sound
	std::string moveSound = "move";
	loadSoundFromFile(moveSound, "../assets/button.wav");

	std::string music = "music";
	loadMusicFromFile(music, "../assets/music.wav");

	m_theme = std::make_shared<tgui::Theme>("../assets/theme.txt");
	tgui::Theme::setDefault(&*m_theme);
}

std::shared_ptr<sf::Font> ResourceManager::loadFontFromFile(std::string name, std::string path)
{
	auto newFont = addEmptyFont(name);
	if (!newFont->loadFromFile(path)) {
		std::cerr << "Could not load font from path " << path << std::endl;
	}
	return newFont;
}

std::shared_ptr<sf::Texture> ResourceManager::loadTextureFromFile(std::string name, std::string path)
{
	auto newTex = addEmptyTexture(name);
	if (!newTex->loadFromFile(path)) {
		std::cerr << "Could not load texture from path " << path << std::endl;
	}
	return newTex;
}

std::shared_ptr<sf::Sprite> ResourceManager::loadSpriteWithTexture(std::string name, std::string textureName)
{
	auto texFound = m_textures.find(textureName);
	if (texFound == m_textures.end()) {
		throw std::invalid_argument("Texture with name " + textureName + " is not loaded yet");
	}
	return addEmptySprite(name, *(*texFound).second);
}

std::shared_ptr<sf::Sound> ResourceManager::loadSoundFromFile(std::string name, std::string path)
{
	auto buffer = addEmptySoundBuffer(name);
	if (!buffer->loadFromFile(path))
	{
		throw std::invalid_argument("Sound not found");
	}

	auto sound = addEmptySound(name);
	sound->setBuffer(*buffer);

	return sound;
}

std::shared_ptr<sf::Music> ResourceManager::loadMusicFromFile(std::string name, std::string path)
{
	auto music = addEmptyMusic(name);
	if (!music->openFromFile(path))
	{
		throw std::invalid_argument("Music not found");
	}
	return music;
}

ResourceManager& ResourceManager::getInstance() {
	static ResourceManager instance;
	return instance;
}

std::shared_ptr<sf::Font> ResourceManager::getFont(std::string name) {
	auto found = m_fonts.find(name);
	if (found != m_fonts.end())
		return (*found).second;
	return nullptr;
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(std::string name) {
	auto found = m_textures.find(name);
	if (found != m_textures.end())
		return (*found).second;
	return nullptr;
}

std::shared_ptr<sf::Sprite> ResourceManager::getSprite(std::string name) {
	auto found = m_sprites.find(name);
	if (found != m_sprites.end())
		return (*found).second;
	return nullptr;
}

std::shared_ptr<tgui::Theme> ResourceManager::getTheme()
{
	return m_theme;
}

std::shared_ptr<sf::Sound> ResourceManager::getSound(std::string name)
{
	auto found = m_sounds.find(name);
	if (found != m_sounds.end())
		return (*found).second;
	return nullptr;
}

std::shared_ptr<sf::Music> ResourceManager::getMusic(std::string name)
{
	auto found = m_music.find(name);
	if (found != m_music.end())
		return (*found).second;
	return nullptr;
}