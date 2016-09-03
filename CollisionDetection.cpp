#include "CollisionDetection.h"

CollisionDetection::CollisionDetection()
{

}

CollisionDetection::CollisionDetection(vector<Projectile*>* projectiles, vector<Asteroid*>* asteroids, vector<Planet*>* planets, Spaceship* spaceship)
{
  this->projectiles = projectiles;
  this->asteroids = asteroids;
  this->planets = planets;
  this->spaceship = spaceship;
}

void CollisionDetection::react()
{
  // check Collision for asteroid <> asteroid
  for(int i = 0; i < asteroids->size(); ++i)
  {
    for(int j = i+1; j < asteroids->size(); ++j)
    {
      if(check((*asteroids)[i], (*asteroids)[j]))
      {
        cout << "Collision between: " << (*asteroids)[i]->getName() << " and " << (*asteroids)[j]->getName() << endl;
        (*asteroids)[i]->die();
        (*asteroids)[j]->die();
      }
    }
  }
  // check Collision for spaceship <> asteroid
  // check Collision for projectile <> asteroid
  for(int i = 0; i < projectiles->size(); ++i)
  {
    for(int j = i+1; j < asteroids->size(); ++j)
    {
      if(check((*projectiles)[i], (*asteroids)[j]))
      {
        cout << "Projectile collieded with the Asteroid" << endl;
        (*asteroids)[j]->die();
        (*projectiles)[i]->die();
        SoundManager::getInstance()->playExplosionSound();
      }
    }
  }
  // check Collision for asteroid <> planet
  // check Collision for spaceship <> planet
  // check Collision for projectile <> planet
  for(int i = 0; i < planets->size(); ++i)
  {
    for(int j = i+1; j < projectiles->size(); ++j)
    {
      if(check((*projectiles)[j], (*planets)[i]))
      {
        cout << "Projectile collieded with the Earth" << endl;

      }
    }
  }
}

// check Collision for asteroid <> asteroid
bool CollisionDetection::check(Asteroid* a, Asteroid* b)
{
  /*BoundingBox a1 = a->boundingBox();
  BoundingBox b1 = b->boundingBox();

  Vector t(
    max(a1.Max.X, b1.Max.X) - min(a1.Min.X, b1.Min.X),
    max(a1.Max.Y, b1.Max.Y) - min(a1.Min.Y, b1.Min.Y),
    max(a1.Max.Z, b1.Max.Z) - min(a1.Min.Z, b1.Min.Z)
  );

  return t.X < ( (a1.Max.X-a1.Min.X) + (a1.Max.X-a1.Min.X) ) &&
          t.Y < ( (a1.Max.Y-a1.Min.Y) + (a1.Max.Y-a1.Min.Y) ) &&
          t.Z < ( (a1.Max.Z-a1.Min.Z) + (a1.Max.Z-a1.Min.Z) );*/
          BoundingSphere a1 = a->boundingSphere();
          BoundingSphere b1 = b->boundingSphere();
          const Vector diff = b1.Center-a1.Center;
          const float R = a1.Radius+b1.Radius;

          return R*R > diff.lengthSquared();
}

// check Collision for spaceship <> asteroid
bool CollisionDetection::check(Spaceship* a, Asteroid* b)
{

}

// check Collision for projectile <> asteroid
bool CollisionDetection::check(Projectile* a, Asteroid* b)
{
  BoundingSphere a1 = a->boundingSphere();
  BoundingSphere b1 = b->boundingSphere();
  const Vector diff = b1.Center-a1.Center;
  const float R = a1.Radius+b1.Radius;

  return R*R > diff.lengthSquared();
}

// check Collision for asteroid <> planet
bool CollisionDetection::check(Asteroid* a, Planet* b)
{

}

// check Collision for spaceship <> planet
bool CollisionDetection::check(Spaceship* a, Planet* b)
{

}

// check Collision for projectile <> planet
bool CollisionDetection::check(Projectile* a, Planet* b)
{
  BoundingSphere a1 = a->boundingSphere();
  BoundingSphere b1 = b->boundingSphere();
  const Vector diff = b1.Center-a1.Center;
  const float R = a1.Radius+b1.Radius;

  return R*R > diff.lengthSquared();
}

/*bool CollisionDetection::checkSAT(GameObject* a, GameObject* b)
{
  vector<Vector*> axes;
  vector<Vector*> an = a->getAllNormals();
  vector<Vector*> bn = b->getAllNormals();

  axes.insert(end(axes), begin(an), end(an));
  axes.insert(end(axes), begin(bn), end(bn));

  for(auto vec : axes)
  {
    Projection pa = a->projectOnVector(*vec);
    Projection pb = b->projectOnVector(*vec);

    if(pa.overlap(pb) <= 0)
      return false;
  }
  return true;
}*/
