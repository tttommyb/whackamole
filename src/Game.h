
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
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void newAnimal();

 private:
  sf::RenderWindow& window;

  sf::Sprite* character;
  sf::Sprite* passport;

  sf::Texture* animals = new sf::Texture[3];
  sf::Texture* passports = new sf::Texture[3];

  sf::Sprite accept_button;
  sf::Sprite reject_button;

  sf::Texture accept_texture;
  sf::Texture reject_texture;

  sf::Font font;

  sf::Text menu_text;
  sf::Text play_text;
  sf::Text quit_text;

  bool play_selected = true;
  bool in_menu = true;

  enum critter_id { PENGUIN = 0, MOOSE, ELEPHANT };
  int character_id;
  int passport_id;
};

#endif // PLATFORMER_GAME_H
