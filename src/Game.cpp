
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

	play_text.setString(">Play<");
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

	score_text.setString("Score: 0");
	score_text.setFont(font);
	score_text.setCharacterSize(35);
	score_text.setFillColor(sf::Color(0, 0, 0, 255));
	score_text.setPosition(window.getSize().x - score_text.getGlobalBounds().width - 200, score_text.getGlobalBounds().height );


	lives_text.setString("Lives: 3");
	lives_text.setFont(font);
	lives_text.setCharacterSize(35);
	lives_text.setFillColor(sf::Color(255, 0, 0, 255));
	lives_text.setPosition(window.getSize().x - lives_text.getGlobalBounds().width - 10, lives_text.getGlobalBounds().height);

	timer_text.setString("Time: ");
	timer_text.setFont(font);
	timer_text.setCharacterSize(35);
	timer_text.setFillColor(sf::Color(255, 0, 0, 255));
	timer_text.setPosition(window.getSize().x - timer_text.getGlobalBounds().width - 500, timer_text.getGlobalBounds().height);

	final_score_text.setString("Score: ");
	final_score_text.setFont(font);
	final_score_text.setCharacterSize(50);
	final_score_text.setFillColor(sf::Color(255, 128, 255, 255));
	final_score_text.setPosition(window.getSize().x/2 - final_score_text.getGlobalBounds().width/2 , window.getSize().y / 3);

	gameover_text.setString("Gameover!");
	gameover_text.setFont(font);
	gameover_text.setCharacterSize(60);
	gameover_text.setFillColor(sf::Color(255, 0, 0, 255));
	gameover_text.setPosition(window.getSize().x / 2 - gameover_text.getGlobalBounds().width / 2, window.getSize().y / 4);


	

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

	int animal_index = rand() % 8;
	int passport_index = rand() % 8;

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
	passport_photo.get()->getSprite().setScale(0.4, 0.4);
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
