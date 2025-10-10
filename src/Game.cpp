
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{
	delete[] animals;
	delete[] passports;
	delete character;
	delete passport;
}

bool Game::init()
{
	srand(time(0));

	if (!font.loadFromFile("../Data/Fonts/OpenSans-Bold.ttf"))
	{
		std::cout << "font did not load \n";
	}

	menu_text.setString("Critter Crossing");
	menu_text.setFont(font);
	menu_text.setCharacterSize(100);
	menu_text.setFillColor(sf::Color(255, 128, 255, 255));
	menu_text.setPosition(
		window.getSize().x / 2 - menu_text.getGlobalBounds().width / 2,
		100);

	play_text.setString("Play");
	play_text.setFont(font);
	play_text.setCharacterSize(50);
	play_text.setFillColor(sf::Color(255, 128, 255, 255));
	play_text.setPosition(
		window.getSize().x / 2 - play_text.getGlobalBounds().width / 2 - 125,
		window.getSize().y / 2 - play_text.getGlobalBounds().height / 2);

	quit_text.setString("Quit");
	quit_text.setFont(font);
	quit_text.setCharacterSize(50);
	quit_text.setFillColor(sf::Color(255, 128, 255, 255));
	quit_text.setPosition(window.getSize().x / 2 - quit_text.getGlobalBounds().width / 2 + 125, window.getSize().y / 2 - quit_text.getGlobalBounds().height / 2);


	passports[0].loadFromFile("../Data/Critter Crossing Customs/penguin passport.png");
	passports[1].loadFromFile("../Data/Critter Crossing Customs/moose passport.png");
	passports[2].loadFromFile("../Data/Critter Crossing Customs/elephant passport.png");


	animals[0].loadFromFile("../Data/Critter Crossing Customs/penguin.png");
	animals[1].loadFromFile("../Data/Critter Crossing Customs/moose.png");
	animals[2].loadFromFile("../Data/Critter Crossing Customs/elephant.png");
	
	accept_texture.loadFromFile("../Data/Critter Crossing Customs/accept button.png");
	reject_texture.loadFromFile("../Data/Critter Crossing Customs/reject button.png");

	character = new sf::Sprite;
	passport = new sf::Sprite;

	character_id = static_cast<critter_id>(rand() % 2);
	passport_id = static_cast<critter_id>(rand() % 2);

	character->setTexture(animals[character_id]);
	character->setPosition(100, 100);
	character->setScale(1.5, 1.5);

	passport->setTexture(passports[passport_id]);
	passport->setScale(0.75, 0.75);
	passport->setPosition(window.getSize().x - passport->getGlobalBounds().width - 100, 100);



  return true;
}

void Game::update(float dt)
{

}

void Game::render()
{
	if (in_menu) {
		window.draw(menu_text);
		window.draw(play_text);
		window.draw(quit_text);
	}
	else 
	{
		window.draw(*character);
		window.draw(*passport);
	}
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);


}

void Game::keyPressed(sf::Event event)
{
	if(
		(event.key.code == sf::Keyboard::
			Left) ||
		(event.key.code == sf::Keyboard::
			Right))
	{
		play_selected = !play_selected;
		if (play_selected)
		{
			play_text.setString("> Play <");
			quit_text.setString("Quit");
		}
		else
		{
			play_text.setString("Play");
			quit_text.setString("> Quit <");
		}
	}

	else if (event.key.code == sf::Keyboard::
		Enter)
	{
		if (play_selected)
		{
			in_menu = false;
		}
		else
		{
			window.close();
		}
	}
}

void Game::newAnimal()
{
	passport
}




