#include "Planet.h"

Planet::Planet()
{

}

Planet::Planet(const Vector& pos, string name)
{
  this->pos = pos;
  setName(name);
  bool success = load("assets/model/earth/earth.obj", "assets/shader/ToonVertexShader.glsl", "assets/shader/ToonFragmentShader.glsl", false);

  if(!success)
    exit(4);
}

void Planet::update(float deltaTime)
{
  alive();

  // Set position
  m_position.translation(pos.X, pos.Y, pos.Z);

  //Rotate earth
  m_rotation.rotationYawPitchRoll(rot.X, rot.Y, rot.Z);

  rot.X -= 0.004;
  rot.Y -= 0.f;
  rot.Z -= 0.f;
}
