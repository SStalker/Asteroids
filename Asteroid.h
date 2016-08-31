#ifndef ASTEROID_H
#define ASTEROID_H

#include "GameObject.h"

class Asteroid : public GameObject
{
private:

public:
  Asteroid();
  Asteroid(const Vector& startPos);
  Asteroid(const Vector& startPos, const Vector& startRot);
};

#endif
