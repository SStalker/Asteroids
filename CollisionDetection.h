#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <vector>

#include "Projectile.h"
#include "Asteroid.h"
#include "Spaceship.h"
#include "Planet.h"
#include "SoundManager.h"
//using namespace std;

class CollisionDetection{

private:
  vector<Projectile*>* projectiles;
  vector<Asteroid*>* asteroids;
  vector<Planet*>* planets;
  Spaceship* spaceship;

public:
  CollisionDetection();
  CollisionDetection(vector<Projectile*>* projectiles, vector<Asteroid*>* asteroids, vector<Planet*>* planets, Spaceship* spaceship);
  void react();

  // check Collision for asteroid <> asteroid
  bool check(Asteroid* a, Asteroid* b);

  // check Collision for spaceship <> asteroid
  bool check(Spaceship* a, Asteroid* b);

  // check Collision for projectile <> asteroid
  bool check(Projectile* a, Asteroid* b);

  // check Collision for asteroid <> planet
  bool check(Asteroid* a, Planet* b);

  // check Collision for spaceship <> planet
  bool check(Spaceship* a, Planet* b);

  // check Collision for projectile <> planet
  bool check(Projectile* a, Planet* b);

  bool checkSAT(GameObject* a, GameObject* b);
};

#endif
