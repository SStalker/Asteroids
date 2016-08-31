#ifndef GAME_H
#define GAME_H

#include "Asteroid.h"
#include "Projectile.h"
#include "Spaceship.h"
#include "Planet.h"

#include <vector>
#include <random>

class Game
{
private:
  Game();
  ~Game();

  /** Fighter instance **/
  Spaceship* ship;

  /** List of bullets **/
  vector<Projectile*>* projectileList;

  /** List of asteroids **/
  vector<Asteroid*>* asteroidList;

  /** List of planets **/
  vector<Planet*>* planetList;

public:
  static Game* getInstance();

  void init();

  Spaceship* getSpaceship();
  vector<Projectile*>* getProjectileList();
  vector<Asteroid*>* getAsteroidList();
  vector<Planet*>* getPlanetList();
};

#endif
