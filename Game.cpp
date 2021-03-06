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

    std::random_device rand_dev;

    std::mt19937 rand_engine(rand_dev());

    //Create vector with texturenames for the skybox
    vector<string> SkyTex;
    SkyTex.push_back(string("assets/skybox/SkyBoxRT.bmp"));
    SkyTex.push_back(string("assets/skybox/SkyBoxLF.bmp"));
    SkyTex.push_back(string("assets/skybox/SkyBoxDN.bmp"));
    SkyTex.push_back(string("assets/skybox/SkyBoxUP.bmp"));
    SkyTex.push_back(string("assets/skybox/SkyBoxFT.bmp"));
    SkyTex.push_back(string("assets/skybox/SkyBoxBK.bmp"));


    skybox = new Skybox(SkyTex,"assets/shader/SkyboxVertexShader.glsl", "assets/shader/SkyboxFragmentShader.glsl", fPlane*0.5f);
    ship = new Spaceship();
    projectileList = new vector<Projectile*>();
    asteroidList = new vector<Asteroid*>();
    planetList = new vector<Planet*>();

    planetList->push_back(new Planet(Vector(0.f,0.f,0.f), "Erde"));

    for (unsigned int i = 0; i < 25; i++) {
      Vector pos(unif(rand_engine),unif(rand_engine),unif(rand_engine));
      Vector rot(unif(rand_engine),unif(rand_engine),unif(rand_engine));
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
