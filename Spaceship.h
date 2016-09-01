#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <cmath>
#include "Matrix.h"
#include "Vector.h"
#include "global.h"
#include "GameObject.h"
#include "Projectile.h"

class Spaceship : public GameObject{

public:
	Spaceship();

	void update(float deltaTime);
  void draw();
  //bool load( const char* model, const char* VertexShader, const char* FragmentShader, const Vector& startPos);
	void steer( float fowardBackward, float leftRight);

	/** Bound to the thrust axis */
	void ThrustInput(float Val);

	void setDeltaTime(float deltaTime);

	void fire();

	vector<Projectile*> getProjectiles() const;

private:

	/** Dont if good idea.. save current deltaTime here */
	float deltaTime;

  /** Distance from camera to ship */
  float camShipBaseDistFoward;
  float camShipBaseDistUp;

	/** How quickly forward speed changes */
	float Acceleration;

	/** How quickly pawn can steer */
	float TurnSpeed;

	/** Max forward speed */
	float MaxSpeed;

	/** Min forward speed */
	float MinSpeed;

    /** */
    float SpeedMult;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

  /** Pitch angle*/
  float Pitch;

  /** Yaw angle*/
  float Yaw;

  /** Roll angle*/
  float Roll;

	vector<Projectile*> projectiles;
};

#endif
