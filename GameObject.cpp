#include "GameObject.h"

GameObject::GameObject()
{
  this->health = 100;
  this->dead = false;
  this->name = "GameObject";
}

GameObject::GameObject(const Vector& startPos) : GameObject()
{
  this->pos = startPos;
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
      drawBounding();
//      drawSphere();
  glPopMatrix();
}

void GameObject::update(float deltaTime)
{
    //Reset Bounding box
//    m_Box.Min = m_Box.BaseMin;
//    m_Box.Max = m_Box.BaseMax;
//    m_Box.allPoints[0] = m_Box.allPointsBase[0];
//    m_Box.allPoints[1] = m_Box.allPointsBase[1];
//    m_Box.allPoints[2] = m_Box.allPointsBase[2];
//    m_Box.allPoints[3] = m_Box.allPointsBase[3];
//    m_Box.allPoints[4] = m_Box.allPointsBase[4];
//    m_Box.allPoints[5] = m_Box.allPointsBase[5];
//    m_Box.allPoints[6] = m_Box.allPointsBase[6];
//    m_Box.allPoints[7] = m_Box.allPointsBase[7];

    //reset sphere
//    m_Sphere.Center = m_Sphere.BaseCenter;

    //Set new position
    m_position.translation(pos.X, pos.Y, pos.Z);
    m_rotation.rotationYawPitchRoll(rot.X, rot.Y, rot.Z);

//    Matrix combined = m_position * m_rotation;

//    m_Box.Min += combined.translation()+combined.forward()+combined.up()+combined.right();
//    m_Box.Max += combined.translation()+combined.forward()+combined.up()+combined.right();

//    for(unsigned int i = 0; i < 8; i++){
//        m_Box.allPoints[i] += combined.translation();
//    }

//    m_Sphere.Center += m_position.translation();
}

void GameObject::loadRessources(const char* obj, const char* vertexShader, const char* fragmentShader)
{
  bool success = load(obj, vertexShader, fragmentShader, true);

  if(!success)
    exit(4);
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
