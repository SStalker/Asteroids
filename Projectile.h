#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"
#include "Vector.h"

class Projectile : public GameObject
{
private:
  float speed;
  Vector direction;

public:
  Projectile();
  Projectile(const Vector& startPos, const Vector& direction, const float speed);

  void update(float deltaTime);

  void setSpeed(const float speed);
  float getSpeed() const;

  void setDirection(const Vector& direction);
  Vector getDirection() const;
};

#endif
