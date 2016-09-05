#ifndef GLOBAL_H
#define GLOBAL_H


#include "Camera.h"

//Globale variablen
extern Camera g_Camera;
extern const Vector g_LightPos;
extern const float fovy;
extern const float aspectRatio;
extern const float nPlane;
extern const float fPlane;

inline float clamp(float val, float min, float max)
{
  return std::max(min, std::min(val, max));
}

inline float interpolateTo( float current, float target, float deltaTime, float speed )
{
	// If no interp speed, jump to target value
	if( speed <= 0.f )
	{
		return target;
	}

	// Distance to reach
	const float dist = target - current;

	// If distance is too small, just set the desired location
	if( sqrt(dist) < 0.0001 )
	{
		return target;
	}

	// Delta Move, Clamp so we do not over shoot.
	const float deltaMove = dist * clamp(deltaTime * speed, 0.f, 1.f);

	return current + deltaMove;
}

inline float angleToRadian(float angle)
{
  return angle*M_PI/180;
}

#endif // GLOBAL_H
