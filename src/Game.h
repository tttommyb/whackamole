
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>
#include "Animal.h"

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game() = default;
  bool init();
  void update(float dt);
  void render();
  void mouseButtonPressed(sf::Event event);
  void mouseButtonReleased(sf::Event event);
  void keyPressed(sf::Event event);
  void dragSprite(sf::Sprite& sprite);
  void newAnimal();
  bool checkAccept();
  void initialiseText(sf::Text& text, std::string string, sf::Font& font, int character_size, sf::Color colour, const sf::Vector2f& position);

 private:
  sf::RenderWindow& window;

  sf::Sprite background;
  sf::Texture background_texture;

  sf::Sprite* dragged = nullptr;

  std::unique_ptr<Animal> character = nullptr;

  std::unique_ptr<sf::Sprite> passport;
  sf::Texture passport_texture;
  std::unique_ptr<Animal> passport_photo = nullptr;

  sf::Sprite accept_button;
  sf::Sprite reject_button;

  sf::Text score_text;
  int score = 0;

  sf::Text lives_text;
  int lives = 3;

  sf::Text timer_text;


  sf::Texture accept_texture;
  sf::Texture reject_texture;



  sf::Texture accept_stamp_texture;
  sf::Texture reject_stamp_texture;

  sf::Font main_font;

  sf::Text menu_text;
  sf::Text play_text;
  sf::Text quit_text;

  sf::Text gameover_text;
  sf::Text final_score_text;

  bool play_selected = true;
  bool in_menu = true;
  bool game_over = false;

  bool passport_accepted = false;
  bool passport_rejected = false;

  bool should_accept = false;

  bool show_button_context = false;

  sf::Vector2f drag_offset = { 50.0f, 50.0f };
  sf::Vector2f stamp_offset = { 0.0f, 0.0f };
  sf::Vector2f photo_offset = { 17.4f, 228.0f };

  sf::Clock gameover_delay;
  sf::Clock timer;

};

#endif // PLATFORMER_GAME_H
