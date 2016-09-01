#include "Projectile.h"

Projectile::Projectile()
{

}

Projectile::Projectile(const Vector& startPos, const Vector& direction, const float speed)
: GameObject(startPos)
{
  this->speed = speed;
  this->direction = direction;
  setLifeSpan(4);
  bool success = load("assets/model/projectile.obj", "assets/shader/PhongVertexShader.glsl", "assets/shader/PhongFragmentShader.glsl", false);

  if(!success)
    exit(4);
}

void Projectile::update(float deltaTime)
{
  alive();

  // Set position
  m_position.translation(pos.X, pos.Y, pos.Z);

  // Set rotation
  m_rotation.rotationYawPitchRoll(rot.X, rot.Y, rot.Z);

  // Fly in direction were shoot
  pos = pos + direction*speed;
}

void Projectile::setSpeed(const float speed)
{
  this->speed = speed;
}

float Projectile::getSpeed() const
{
  return this->speed;
}

void Projectile::setDirection(const Vector& direction)
{
  this->direction = direction;
}

Vector Projectile::getDirection() const
{
  return this->direction;
}
