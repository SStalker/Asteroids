#ifndef PLANET_H
#define PLANET_H

#include "GameObject.h"

class Planet : public GameObject
{
private:

public:
  Planet();
  Planet(const Vector& pos, string name);
  void update(float deltaTime);
};

#endif
