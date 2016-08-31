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
  string name;

protected:
  Vector pos;
  Vector rot;
  Matrix m_position;
  Matrix m_rotation;

public:
  GameObject();
  GameObject(const Vector& startPos);
  GameObject(const Vector& startPos, const Vector& startRotation);

  virtual void draw();
  virtual void update(float deltaTime);
  virtual void loadRessources(const char* obj, const char* vertexShader, const char* fragmentShader);

  /* GETTER AND SETTER */
  float getHealth() const;
  void setHealth(float health);

  bool isDead() const;
  void setDead(bool dead);

  string getName() const;
  void setName(string name);

  void setPosition(const Matrix& position);
  const Matrix getPosition() const;

  void setRotation(const Matrix& rotation);
  const Matrix getRotation() const;

  void setPos(const Vector& pos);
  const Vector getPos() const;

  void setRot(const Vector& rot);
  const Vector getRot() const;
};

#endif
