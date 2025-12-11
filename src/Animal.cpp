#include "Animal.h"

Animal::Animal(const int& texture_id, sf::RenderWindow& window)
{
	//Give the animal access to all textures
	textures.push_back(std::make_unique<sf::Texture>());
	textures.back()->loadFromFile("../Data/Critter Crossing Customs/penguin.png");
	textures.push_back(std::make_unique<sf::Texture>());
	textures.back()->loadFromFile("../Data/Critter Crossing Customs/moose.png");
	textures.push_back(std::make_unique<sf::Texture>());
	textures.back()->loadFromFile("../Data/Critter Crossing Customs/elephant.png");
	textures.push_back(std::make_unique<sf::Texture>());
	textures.back()->loadFromFile("../Data/Critter Crossing Customs/buffalo.png");
	textures.push_back(std::make_unique<sf::Texture>());
	textures.back()->loadFromFile("../Data/Critter Crossing Customs/chicken.png");
	textures.push_back(std::make_unique<sf::Texture>());
	textures.back()->loadFromFile("../Data/Critter Crossing Customs/giraffe.png");
	textures.push_back(std::make_unique<sf::Texture>());
	textures.back()->loadFromFile("../Data/Critter Crossing Customs/gorilla.png");
	textures.push_back(std::make_unique<sf::Texture>());
	textures.back()->loadFromFile("../Data/Critter Crossing Customs/narwhal.png");



	sprite = std::make_unique < sf::Sprite>();
	sprite->setTexture(*textures.at(texture_id), true);
	sprite->setScale(1.8, 1.8);
	sprite->setPosition(window.getSize().x / 12, window.getSize().y / 12);
}

sf::Sprite& Animal::getSprite()
{
	return *sprite;
}



void Animal::setTexture(const int& texture_id)
{
	sprite->setTexture(*textures.at(texture_id), true);
}