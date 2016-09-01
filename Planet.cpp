#include "Planet.h"

Planet::Planet()
{

}

Planet::Planet(const Vector& pos, string name)
{
  this->pos = pos;
  setName(name);
  bool success = load("assets/model/earth/earth2.obj", "assets/shader/PhongVertexShader.glsl", "assets/shader/PhongFragmentShader.glsl");

  if(!success)
    exit(4);

}
