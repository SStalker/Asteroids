#include "Asteroid.h"

Asteroid::Asteroid()
{

}

Asteroid::Asteroid(const Vector& startPos)
: GameObject(startPos)
{
  loadRessources("assets/model/asteroid.obj", "assets/shader/PhongVertexShader.glsl", "assets/shader/PhongFragmentShader.glsl");
}

Asteroid::Asteroid(const Vector& startPos, const Vector& startRot)
: GameObject(startPos, startRot)
{
  loadRessources("assets/model/asteroid.obj", "assets/shader/PhongVertexShader.glsl", "assets/shader/PhongFragmentShader.glsl");
}

void Asteroid::update(float deltaTime)
{

  // Set position
  m_position.translation(pos.X, pos.Y, pos.Z);

  // Set rotation
  m_rotation.rotationYawPitchRoll(rot.X, rot.Y, rot.Z);

  // Change the position in direction to earth
  pos = pos - pos * 0.01 * ASTEROID_SPEED;


  rot.X -= 0.001;
  rot.Y -= 0.002;
  rot.Z -= 0.004;
}
