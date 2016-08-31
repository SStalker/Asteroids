#include "GameObject.h"

GameObject::GameObject()
{
  this->health = 100;
  this->dead = false;
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

void GameObject::setPos(const Vector& position)
{
	this->pos = position;
}

const Vector GameObject::getPos() const
{
	return this->pos;
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
