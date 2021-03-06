#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
  this->health = 100;
  this->dead = false;
  this->name = "GameObject";
  this->lifeSpan = 0;
  this->pos = Vector(0.f,0.f,0.f);
  this->rot = Vector(0.f,0.f,0.f);

  this->timeStamp = time(nullptr);
}

GameObject::GameObject(const Vector& startPos) : GameObject()
{
  this->pos = startPos;
  this->rot = Vector(0.f,0.f,0.f);
}

GameObject::GameObject(const Vector& startPos, const Vector& startRotation)
: GameObject(startPos)
{
  this->rot = startRotation;
}

void GameObject::draw()
{
  glPushMatrix();
      glMultMatrixf(m_position*m_rotation);
      drawTriangles();
  glPopMatrix();
}

void GameObject::update(float deltaTime)
{
  alive();

  //Reset Bounding box
  for(unsigned int i = 0; i < 8; i++){
      m_Box.allPoints[i] = m_Box.allPointsBase[i];
  }

  //Set new position
  m_position.translation(pos.X, pos.Y, pos.Z);
  m_rotation.rotationYawPitchRoll(rot.X, rot.Y, rot.Z);
}

void GameObject::updateBounding(){
    //Calc new points after transformation
    Matrix combined = m_position * m_rotation;

    for(unsigned int i = 0; i < 8; i++){
        m_Box.allPoints[i] = combined * m_Box.allPointsBase[i];
    }

    m_Sphere.Center = combined * m_Sphere.BaseCenter;
}

void GameObject::loadRessources(const char* obj, const char* vertexShader, const char* fragmentShader)
{
  bool success = load(obj, vertexShader, fragmentShader, true);

  if(!success)
    exit(4);
}

bool GameObject::alive()
{
  if(health <= 0){
    die();
    return false;
  }

  if(this->lifeSpan == 0)
    return true;

  long delta = time(nullptr)-this->timeStamp;

  if(delta >= this->lifeSpan){
    die();
    return false;
  }
}

void GameObject::die()
{
  this->dead = true;
}

void GameObject::takeDamage(float damageAmount)
{
  this->health -= damageAmount;
}

float GameObject::getHealth() const
{
  return this->health;
}

void GameObject::setHealth(float health)
{
  this->health = health;
}

bool GameObject::isDead() const
{
  return this->dead;
}

void GameObject::setDead(bool dead)
{
  this->dead = dead;
}

string GameObject::getName() const
{
  return this->name;
}

void GameObject::setName(string name)
{
  this->name = name;
}

void GameObject::setPosition(const Matrix& position)
{
	this->m_position = position;
}

const Matrix GameObject::getPosition() const
{
	return this->m_position;
}

void GameObject::setRotation(const Matrix& rotation)
{
	this->m_rotation = rotation;
}

const Matrix GameObject::getRotation() const
{
	return this->m_rotation;
}

void GameObject::setPos(const Vector& position)
{
	this->pos = position;
}

const Vector GameObject::getPos() const
{
	return this->pos;
}

void GameObject::setRot(const Vector& rot)
{
  this->rot = rot;
}

const Vector GameObject::getRot() const
{
  return rot;
}

void GameObject::setLifeSpan(int lifeSpan)
{
  this->lifeSpan = lifeSpan;
}

int GameObject::getLifeSpan()
{
  return lifeSpan;
}
