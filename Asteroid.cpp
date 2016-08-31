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
