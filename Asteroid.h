#ifndef ASTEROID_H
#define ASTEROID_H

#include "GameObject.h"
#define ASTEROID_SPEED 0.1

class Asteroid : public GameObject
{
private:

public:
  Asteroid();
  Asteroid(const Vector& startPos);
  Asteroid(const Vector& startPos, const Vector& startRot, const string name);

  void update(float deltaTime);
};

#endif
