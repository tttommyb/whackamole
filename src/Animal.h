#pragma once
#include <SFML/Graphics.hpp>

class Animal
{
public:
	Animal(const int& texture_id, sf::RenderWindow& game_window);
	void setTexture(const int& texture_id);
	sf::Sprite& getSprite();

private:
	std::unique_ptr<sf::Sprite> sprite = nullptr;

	std::vector<std::unique_ptr<sf::Texture>> textures;

};