
#include "Game.h"
#include <iostream>
#include <chrono>
#include <cstdlib>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}


bool Game::init()
{
	srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	if (!main_font.loadFromFile("../Data/Fonts/OpenSans-Bold.ttf"))
	{
		std::cout << "font did not load \n";
	}

	//Menu Text

	initialiseText(menu_text, "Critter Crossing", main_font, 100, sf::Color(255, 128, 255, 255), { window.getSize().x / 2.0f, 100.0f });

	initialiseText(play_text, ">Play<", main_font, 50, sf::Color(255, 128, 255, 255), { window.getSize().x / 2 - 125.0f, window.getSize().y / 2.0f });

	initialiseText(quit_text, "Quit", main_font, 50, sf::Color(255, 128, 255, 255), { window.getSize().x / 2  + 125.0f, window.getSize().y / 2.0f });

	//Game Text

	initialiseText(score_text, "Score: 0", main_font, 35, sf::Color(0, 0, 0, 255), { window.getSize().x - 270.0f, 20.0f});

	initialiseText(lives_text, "Lives: 3", main_font, 35, sf::Color(255, 0, 0, 255), { window.getSize().x - 80.0f, 20.0f});

	initialiseText(timer_text, "Time: ", main_font, 35, sf::Color(255, 0, 0, 255), { window.getSize().x - 570.0f, 20.0f });

	//Gameover Text

	initialiseText(final_score_text, "Score: ", main_font, 50, sf::Color(255, 128, 255, 255), { window.getSize().x / 2.0f - final_score_text.getGlobalBounds().width / 2.0f , window.getSize().y / 3.0f });

	initialiseText(gameover_text, "Gameover!", main_font, 60, sf::Color(255, 0, 0, 255), { window.getSize().x / 2.0f - gameover_text.getGlobalBounds().width / 2.0f, window.getSize().y / 4.0f });

	background_texture.loadFromFile("../Data/WhackaMole Worksheet/background.png");
	background.setTexture(background_texture);

	accept_texture.loadFromFile("../Data/Critter Crossing Customs/accept button.png");
	reject_texture.loadFromFile("../Data/Critter Crossing Customs/reject button.png");

	accept_stamp_texture.loadFromFile("../Data/Critter Crossing Customs/accept.png");
	reject_stamp_texture.loadFromFile("../Data/Critter Crossing Customs/reject.png");


	accept_button.setTexture(accept_texture);
	reject_button.setTexture(reject_texture);

	accept_button.setPosition(window.getSize().x - 300, window.getSize().y / 2 - 100);
	reject_button.setPosition(window.getSize().x - 300, window.getSize().y / 2 + 50);

	passport_texture.loadFromFile("../Data/Critter Crossing Customs/default passport.png");
	passport = std::make_unique<sf::Sprite>(passport_texture);

	passport_photo = std::make_unique<Animal>(1, window);

	newAnimal();

	return true;
}

void Game::update(float dt)
{
	if (in_menu) { timer.restart(); }
	score_text.setString("Score: " + std::to_string(score));
	lives_text.setString("Lives: " + std::to_string(lives));
	timer_text.setString("Time: " +  std::to_string(3 - timer.getElapsedTime().asSeconds()).substr(0, 4));
	if(timer.getElapsedTime().asSeconds() >= 3 && game_over == false)
	{
		lives--;
		if (lives <= 0)
		{
			game_over = true;
			final_score_text.setString("Score: " + std::to_string(score));
			final_score_text.setPosition(window.getSize().x / 2 - final_score_text.getGlobalBounds().width / 2, window.getSize().y - 300);
			gameover_delay.restart();
		}
		newAnimal();
		timer.restart();
		dragged = nullptr;
		show_button_context = false;
		passport_accepted = false;
		passport_rejected = false;
		accept_button.setTexture(accept_texture);
		reject_button.setTexture(reject_texture);
	}

	if(game_over)
	{
		if (gameover_delay.getElapsedTime().asSeconds() >= 2)
		{
			final_score_text.setString("Score: ");
			game_over = false;
			in_menu = true;
			score = 0;
			lives = 3;
		}
	}

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
		if (game_over)
		{
			window.draw(gameover_text);
			window.draw(final_score_text);
			
			return;
		}
		window.draw(background);
		window.draw(character.get()->getSprite());
		window.draw(*passport);
		window.draw(passport_photo.get()->getSprite());
		if(show_button_context || passport_rejected)
		{
			window.draw(reject_button);
		}
		if (show_button_context || passport_accepted) 
		{
			window.draw(accept_button);

		}

		window.draw(score_text);
		window.draw(lives_text);
		window.draw(timer_text);

		
		
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

	int match = rand() % 10;
	int animal_index = rand() % 8;
	int passport_index = rand() % 8;

	if(match >= 6)
	{
		passport_index = animal_index;
	}

	if (animal_index == passport_index) 
	{
		should_accept = true;
	}
	else
	{
		should_accept = false;
	}

	if(character == nullptr)
	{
		character = std::make_unique<Animal>(animal_index, window);
	}
	else 
	{
		character.get()->setTexture(animal_index);
	}

	passport->setScale(0.6, 0.6);
	passport->setPosition(window.getSize().x / 2, window.getSize().y / 3);
	passport_photo.get()->setTexture(passport_index);
	passport_photo.get()->getSprite().setScale(80.0f/passport_photo.get()->getSprite().getLocalBounds().width, 110.0f / passport_photo.get()->getSprite().getLocalBounds().height);
	passport_photo.get()->getSprite().setPosition(passport.get()->getPosition() + photo_offset);

	timer.restart();
}

void Game::dragSprite(sf::Sprite& sprite)
{

	sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
	sf::Vector2f mouse_positionf = static_cast<sf::Vector2f>(mouse_position);

	sf::Vector2f drag_position = mouse_positionf - drag_offset;
	sprite.setPosition(drag_position.x, drag_position.y);


	passport_photo.get()->getSprite().setPosition(sprite.getPosition() + photo_offset);

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
			drag_offset = clickf - passport.get()->getPosition();
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

		if(character.get()->getSprite().getGlobalBounds().contains(release_positionf))
		{
			if (checkAccept())
			{
				std::cout << "CORRECT!";
				score += 100;
			}
			else {
				std::cout << "WRONG!";
				lives--;
				if (lives <= 0)
				{
					game_over = true;
					final_score_text.setString(final_score_text.getString() + std::to_string(score));
					final_score_text.setPosition(window.getSize().x / 2 - final_score_text.getGlobalBounds().width / 2, window.getSize().y - 300);
					gameover_delay.restart();
				}


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

	return(should_accept == passport_accepted && passport_accepted != passport_rejected);
}


void Game::initialiseText(sf::Text& text, std::string string, sf::Font& font, int character_size, sf::Color colour, const sf::Vector2f& position)
{
	std::cout << position.x << " " << position.y << "\n";
	text.setString(string);
	text.setFont(font);
	text.setCharacterSize(character_size);
	text.setFillColor(colour);
	text.setPosition(position.x - text.getGlobalBounds().width/2, position.y - text.getGlobalBounds().height / 2);
}