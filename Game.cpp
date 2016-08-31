#include "Game.h"

Game* Game::getInstance()
{
  static Game instance;
  return &instance;
}

Game::Game()
{
	init();
}

Game::~Game()
{
	delete ship;

	asteroidList->clear();
	planetList->clear();
	projectileList->clear();
}

/**
 *  Erstellt die Asteroiden sowie die Erde und das Raumschiff
 */
void Game::init()
{
  if(Game::getInstance() != nullptr)
  {
    ship = new Spaceship();
    projectileList = new vector<Projectile*>();
    asteroidList = new vector<Asteroid*>();
    planetList = new vector<Planet*>();
  }
}

Spaceship* Game::getSpaceship()
{
  return this->ship;
}

vector<Projectile*>* Game::getProjectileList()
{
  return projectileList;
}

vector<Asteroid*>* Game::getAsteroidList()
{
  return asteroidList;
}

vector<Planet*>* Game::getPlanetList()
{
  return planetList;
}
