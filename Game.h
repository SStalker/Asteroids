#ifndef GAME_H
#define GAME_H

#include "Asteroid.h"
#include "Projectile.h"
#include "Spaceship.h"
#include "Planet.h"
#include "Skybox.h"

#include <vector>
#include <random>

class Game
{
private:
  Game();
  ~Game();

  // Spaceship
  Spaceship* ship;

  // List of projectiles
  vector<Projectile*>* projectileList;

  // List of asteroids
  vector<Asteroid*>* asteroidList;

  // List of planets
  vector<Planet*>* planetList;

  // Skybox
  Skybox* skybox;

public:
  static Game* getInstance();

  void init();
  void drawSkybox();

  // Getter and Setter
  Spaceship* getSpaceship();
  vector<Projectile*>* getProjectileList();
  vector<Asteroid*>* getAsteroidList();
  vector<Planet*>* getPlanetList();
};

#endif
