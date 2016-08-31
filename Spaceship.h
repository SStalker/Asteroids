#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "Matrix.h"
#include "Vector.h"
#include "global.h"
#include "GameObject.h"

class Spaceship : public GameObject{

public:
	Spaceship();

	void update(float deltaTime);
  void draw();
  //bool load( const char* model, const char* VertexShader, const char* FragmentShader, const Vector& startPos);
	void steer( float fowardBackward, float leftRight);

	/** Bound to the thrust axis */
	void ThrustInput(float Val);

	/** Bound to the vertical axis */
	void MoveUpInput(float Val);

	/** Bound to the horizontal axis */
	void MoveRightInput(float Val);

	void setDeltaTime(float deltaTime);

private:

	/** Dont if good idea.. save current deltaTime here */
	float deltaTime;

	/** How quickly forward speed changes */
	float Acceleration;

	/** How quickly pawn can steer */
	float TurnSpeed;

	/** Max forward speed */
	float MaxSpeed;

	/** Min forward speed */
	float MinSpeed;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;
};

#endif
