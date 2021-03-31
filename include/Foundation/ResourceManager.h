// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include <unordered_map>
#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>

class ResourceManager {
public:
	static ResourceManager& getInstance();
	void loadDefaults();

	std::shared_ptr<sf::Font> loadFontFromFile(std::string name, std::string path);
	std::shared_ptr<sf::Texture> loadTextureFromFile(std::string name, std::string path);
	std::shared_ptr<sf::Sprite> loadSpriteWithTexture(std::string name, std::string textureName);

	std::shared_ptr<sf::Sound> loadSoundFromFile(std::string name, std::string path);
	std::shared_ptr<sf::Music> loadMusicFromFile(std::string name, std::string path);
	
	std::shared_ptr<sf::Font> getFont(std::string name);
	std::shared_ptr<sf::Texture> getTexture(std::string name);
	std::shared_ptr<sf::Sprite> getSprite(std::string name);
	std::shared_ptr<tgui::Theme> getTheme();
	std::shared_ptr<sf::Sound> getSound(std::string name);
	std::shared_ptr<sf::Music> getMusic(std::string name);

	template<typename... Args>
	std::shared_ptr<sf::Font> addEmptyFont(std::string name, Args&&... args) {
		auto found = m_fonts.find(name);
		if (found == m_fonts.end()) {
			auto newFont = std::make_shared<sf::Font>(std::forward<Args>(args)...);
			m_fonts[name] = newFont;
			return newFont;
		}
		return (*found).second;
	}

	template<typename... Args>
	std::shared_ptr<sf::Texture> addEmptyTexture(std::string name, Args&&... args) {
		auto found = m_textures.find(name);
		if (found == m_textures.end()) {
			auto newTex = std::make_shared<sf::Texture>(std::forward<Args>(args)...);
			m_textures[name] = newTex;
			return newTex;
		}
		return (*found).second;
	}

	template<typename... Args>
	std::shared_ptr<sf::Sprite> addEmptySprite(std::string name, Args&&... args) {
		auto found = m_sprites.find(name);
		if (found == m_sprites.end()) {
			auto newSprite = std::make_shared<sf::Sprite>(std::forward<Args>(args)...);
			m_sprites[name] = newSprite;
			return newSprite;
		}
		return (*found).second;
	}

	template<typename... Args>
	std::shared_ptr<sf::SoundBuffer> addEmptySoundBuffer(std::string name, Args&&... args) {
		auto found = m_soundBuffers.find(name);
		if (found == m_soundBuffers.end()) {
			auto newBuffer = std::make_shared<sf::SoundBuffer>(std::forward<Args>(args)...);
			m_soundBuffers[name] = newBuffer;
			return newBuffer;
		}
		return (*found).second;
	}

	template<typename... Args>
	std::shared_ptr<sf::Sound> addEmptySound(std::string name, Args&&... args) {
		auto found = m_sounds.find(name);
		if (found == m_sounds.end()) {
			auto newSound = std::make_shared<sf::Sound>(std::forward<Args>(args)...);
			m_sounds[name] = newSound;
			return newSound;
		}
		return (*found).second;
	}

	template<typename... Args>
	std::shared_ptr<sf::Music> addEmptyMusic(std::string name, Args&&... args) {
		auto found = m_music.find(name);
		if (found == m_music.end()) {
			auto newMusic = std::make_shared<sf::Music>(std::forward<Args>(args)...);
			m_music[name] = newMusic;
			return newMusic;
		}
		return (*found).second;
	}

private:
	std::unordered_map<std::string, std::shared_ptr<sf::Font>> m_fonts;
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;
	std::unordered_map<std::string, std::shared_ptr<sf::Sprite>> m_sprites;

	std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> m_soundBuffers;
	std::unordered_map<std::string, std::shared_ptr<sf::Sound>> m_sounds;
	std::unordered_map<std::string, std::shared_ptr<sf::Music>> m_music;

	std::shared_ptr<tgui::Theme> m_theme;

	ResourceManager();
};