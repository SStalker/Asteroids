#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"
#include "Vector.h"
#include "Matrix.h"

/**
* Klasse zwischen den Model selbst und einem Objekt im Spiel
*/
class GameObject : public Model{

private:
  float health;
  bool dead;

protected:
  Vector pos;
  Matrix m_position;
  Matrix m_rotation;

public:
  GameObject();

  /* GETTER AND SETTER */
  float getHealth() const;
  void setHealth(float health);

  bool isDead() const;
  void setDead(bool dead);

  void setPosition(const Matrix& position);
  const Matrix getPosition() const;

  void setRotation(const Matrix& rotation);
  const Matrix getRotation() const;

  void setPos(const Vector& pos);
  const Vector getPos() const;
};

#endif
