
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
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


	background_texture.loadFromFile("../Data/WhackaMole Worksheet/background.png");
	background.setTexture(background_texture);

	passports.push_back(std::make_unique<sf::Texture>());
	passports.back()->loadFromFile("../Data/Critter Crossing Customs/penguin passport.png");
	passports.push_back(std::make_unique<sf::Texture>());
	passports.back()->loadFromFile("../Data/Critter Crossing Customs/moose passport.png");
	passports.push_back(std::make_unique<sf::Texture>());
	passports.back()->loadFromFile("../Data/Critter Crossing Customs/elephant passport.png");


	animals.push_back(std::make_unique<sf::Texture>());
	animals.back()->loadFromFile("../Data/Critter Crossing Customs/penguin.png");
	animals.push_back(std::make_unique<sf::Texture>());
	animals.back()->loadFromFile("../Data/Critter Crossing Customs/moose.png");
	animals.push_back(std::make_unique<sf::Texture>());
	animals.back()->loadFromFile("../Data/Critter Crossing Customs/elephant.png");

	accept_texture.loadFromFile("../Data/Critter Crossing Customs/accept button.png");
	reject_texture.loadFromFile("../Data/Critter Crossing Customs/reject button.png");

	accept_stamp_texture.loadFromFile("../Data/Critter Crossing Customs/accept.png");
	reject_stamp_texture.loadFromFile("../Data/Critter Crossing Customs/reject.png");


	accept_button.setTexture(accept_texture);
	reject_button.setTexture(reject_texture);

	accept_button.setPosition(window.getSize().x - 300, window.getSize().y / 2 - 100);
	reject_button.setPosition(window.getSize().x - 300, window.getSize().y / 2 + 50);

	character = std::make_unique<sf::Sprite>();
	passport = std::make_unique<sf::Sprite>();

	newAnimal();

	return true;
}

void Game::update(float dt)
{
	if (dragged == nullptr) { return; }
	dragSprite(*dragged);
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
		window.draw(background);
		window.draw(*character);
		window.draw(*passport);
		if(show_button_context || passport_rejected)
		{
			window.draw(reject_button);
		}
		if (show_button_context || passport_accepted) 
		{
			window.draw(accept_button);

		}

		
	}
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
	passport_accepted = false;
	passport_rejected = false;

	int animal_index = rand() % 3;
	int passport_index = rand() % 3;

	if (animal_index == passport_index) 
	{
		should_accept = true;
	}
	else
	{
		should_accept = false;
	}

	character->setTexture(*animals.at(animal_index), true);
	character->setScale(1.8, 1.8);
	character->setPosition(window.getSize().x / 12, window.getSize().y / 12);


	passport->setTexture(*passports.at(passport_index));
	passport->setScale(0.6, 0.6);
	passport->setPosition(window.getSize().x / 2, window.getSize().y / 3);
}

void Game::dragSprite(sf::Sprite& sprite)
{
	
		sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
		sf::Vector2f mouse_positionf = static_cast<sf::Vector2f>(mouse_position);

		sf::Vector2f drag_position = mouse_positionf - drag_offset;
		sprite.setPosition(drag_position.x, drag_position.y);

		if(passport_accepted)
		{
			accept_button.setPosition(drag_position - stamp_offset);
		}
		if (passport_rejected)
		{
			reject_button.setPosition(drag_position - stamp_offset);
		}

}


void Game::mouseButtonPressed(sf::Event event)
{
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i click = sf::Mouse::getPosition(window);
		sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

		if (passport->getGlobalBounds().contains(clickf)) 
		{
		
			dragged = passport.get();
		}

		if(show_button_context)
		{
			show_button_context = false;

			if(accept_button.getGlobalBounds().contains(clickf))
			{
				passport_accepted = true;
				stamp_offset = passport->getPosition() - accept_button.getPosition();
				accept_button.setTexture(accept_stamp_texture);
				return;
			}

			else if (reject_button.getGlobalBounds().contains(clickf))
			{
				passport_rejected = true;
				stamp_offset = passport->getPosition() - reject_button.getPosition();
				reject_button.setTexture(reject_stamp_texture);

				return;
			}
			
		}
	}

	if (event.mouseButton.button == sf::Mouse::Right)
	{
		sf::Vector2i click = sf::Mouse::getPosition(window);
		sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

		if (passport->getGlobalBounds().contains(clickf) && (passport_accepted == passport_rejected))
		{
			show_button_context = true;

			accept_button.setPosition(clickf);
			reject_button.setPosition(clickf);
			reject_button.setPosition(clickf + sf::Vector2f(0 , 150.0f));
		}
		else {
			show_button_context = false;
		}
	}
}

void Game::mouseButtonReleased(sf::Event event)
{
	
	if (passport_accepted || passport_rejected) 
	{
		sf::Vector2i release_position = sf::Mouse::getPosition(window);
		sf::Vector2f release_positionf = static_cast<sf::Vector2f>(release_position);

		if(character->getGlobalBounds().contains(release_positionf))
		{
			if(checkAccept())
			{
				std::cout << "CORRECT!";
			}
			else {
				std::cout << "WRONG!";

			}
			accept_button.setTexture(accept_texture);
			reject_button.setTexture(reject_texture);
			newAnimal();
		}
	}
	dragged = nullptr;
}

bool Game::checkAccept()
{
	return(should_accept == passport_accepted);
}
