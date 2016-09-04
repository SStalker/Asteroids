#include "Game.h"

Game* Game::getInstance()
{
  static Game instance;
  return &instance;
}

Game::Game()
{

}

Game::~Game()
{
	delete ship;
    delete skybox;

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

    const float lower_bound = -200.f;
    const float upper_bound = 200.f;
    std::uniform_real_distribution<float> unif(lower_bound, upper_bound);

    std::random_device rand_dev;          // Use random_device to get a random seed.

    std::mt19937 rand_engine(rand_dev()); // mt19937 is a good pseudo-random number
                                          // generator.

    vector<string> SkyTex;
    SkyTex.push_back(string("assets/skybox/right.bmp"));
    SkyTex.push_back(string("assets/skybox/left.bmp"));
    SkyTex.push_back(string("assets/skybox/top.bmp"));
    SkyTex.push_back(string("assets/skybox/bottom.bmp"));
    SkyTex.push_back(string("assets/skybox/back.bmp"));
    SkyTex.push_back(string("assets/skybox/front.bmp"));

    skybox = new Skybox(SkyTex,"assets/shader/SkyboxVertexShader.glsl", "assets/shader/SkyboxFragmentShader.glsl", 1000.f);
    ship = new Spaceship();
    projectileList = new vector<Projectile*>();
    asteroidList = new vector<Asteroid*>();
    planetList = new vector<Planet*>();

    planetList->push_back(new Planet(Vector(0.f,0.f,0.f), "Erde"));

    for (unsigned int i = 0; i < 15; i++) {
      Vector pos(unif(rand_engine),unif(rand_engine),unif(rand_engine));
      Vector rot(unif(rand_engine),unif(rand_engine),unif(rand_engine));
      //cout << "As Pos: " << v << endl;
      asteroidList->push_back(new Asteroid(pos, rot, "Asteroid" + to_string(asteroidList->size())));
    }
  }
}

void Game::drawSkybox()
{
    skybox->draw();
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
