#include <SFML\Graphics.hpp>
#include <string>
#include <map>
#include "TextureManager.hpp"

TextureManager::TextureManager() {

}

TextureManager::~TextureManager() {

}

void TextureManager::addTexture(const std::string& key, const std::string& path) {

	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile(path);
	texture->setSmooth(true);
	this->textures[key] = texture;
}

sf::Texture* TextureManager::getTexture(const std::string& key) {
	return this->textures[key];
}