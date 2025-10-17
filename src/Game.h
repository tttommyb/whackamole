
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseButtonPressed(sf::Event event);
  void mouseButtonReleased(sf::Event event);
  void keyPressed(sf::Event event);
  void dragSprite(sf::Sprite* sprite);
  void newAnimal();
  bool checkAccept();

 private:
  sf::RenderWindow& window;

  sf::Sprite background;
  sf::Texture background_texture;

  sf::Sprite* dragged = nullptr;

  sf::Sprite* character;
  sf::Sprite* passport;

  sf::Texture* animals = new sf::Texture[3];
  sf::Texture* passports = new sf::Texture[3];

  sf::Sprite accept_button;
  sf::Sprite reject_button;



  sf::Texture accept_texture;
  sf::Texture reject_texture;



  sf::Texture accept_stamp_texture;
  sf::Texture reject_stamp_texture;

  sf::Font font;

  sf::Text menu_text;
  sf::Text play_text;
  sf::Text quit_text;

  bool play_selected = true;
  bool in_menu = true;

  bool passport_accepted = false;
  bool passport_rejected = false;

  bool should_accept = false;

  bool show_button_context = false;

  sf::Vector2f drag_offset = { 50.0f, 50.0f };
  sf::Vector2f stamp_offset = { 0.0f, 0.0f };
};

#endif // PLATFORMER_GAME_H
