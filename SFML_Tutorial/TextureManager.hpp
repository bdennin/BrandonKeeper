#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SFML\Graphics.hpp>
#include <vector>
#include <map>

class TextureManager {

private:

	std::map<std::string, sf::Texture*> textures;

public:

	TextureManager();
	~TextureManager();
	void addTexture(const std::string& key, const std::string& path);
	sf::Texture* getTexture(const std::string& key);
};

#endif