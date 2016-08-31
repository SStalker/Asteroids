#include "Spaceship.h"

Spaceship::Spaceship()
{
	// Set handling parameters
    Acceleration = 10.f; // 500
    TurnSpeed = 0.125f;
    MaxSpeed = 20.f; // 5000
    MinSpeed = 2.f; // 500
    CurrentForwardSpeed = 2.f; // 500
    Pitch = 0.f;
    Yaw = 0.f;
    Roll = 0.f;
    camShipBaseDistFoward = -6.f;
    camShipBaseDistUp = 3.f;
}

void Spaceship::update(float deltaTime)
{
    //Rotate ship
    m_rotation.rotationYawPitchRoll(Yaw, Pitch, Roll);

    //Calculate new position
    pos.X += CurrentForwardSpeed * deltaTime * m_rotation.forward().X;
    pos.Y += CurrentForwardSpeed * deltaTime * m_rotation.forward().Y;
    pos.Z += CurrentForwardSpeed * deltaTime * m_rotation.forward().Z;

    //Set new position
    m_position.translation(pos.X, pos.Y, pos.Z);

    //Combine matrices for the camera
    Matrix combined = m_position * m_rotation;

    g_Camera.setPosition(combined.forward() * (camShipBaseDistFoward - CurrentForwardSpeed/10.f ) + combined.up() * (camShipBaseDistUp + CurrentForwardSpeed/10.f) + combined.translation());
    g_Camera.setTarget(combined.translation() + combined.forward()*8.f);
    g_Camera.setUp(combined.up());
    g_Camera.apply();

    //Slow down
    ThrustInput(-0.2f);

    //Roll back
    if(Roll != 0.f){
        if(Roll > 0)
            Roll -= deltaTime * 0.1f;
        else if(Roll < 0)
            Roll += deltaTime * 0.1f;
    }

}

void Spaceship::draw()
{
    glPushMatrix();
        glMultMatrixf(m_position*m_rotation);
        drawTriangles();
    glPopMatrix();
}

void Spaceship::steer(float forwardBackward, float leftRight)
{

    float vorzeichen = 1.f;

    Pitch += (-1.f) * forwardBackward * deltaTime * TurnSpeed;// * CurrentPitchSpeed;//?

    if(Pitch > 2 * M_PI){
        Pitch-=2 * M_PI;
    }else if(Pitch < 0.f){
        Pitch += 2 * M_PI;
    }

    if(Pitch > M_PI_2 && Pitch < (3 * M_PI_2) ){
        leftRight *= (-1.f);
        vorzeichen = -1.f;
    }

    Yaw += (-1.f) * leftRight * deltaTime * TurnSpeed;

    if(Yaw > 2 * M_PI){
        Yaw -= 2 * M_PI;
    }else if(Yaw < 0.f){
        Yaw += 2 * M_PI;
    }

    Roll += leftRight * deltaTime * TurnSpeed * vorzeichen;

    if(Roll > M_PI_4){
        Roll =  M_PI_4;
    }else if(Roll < (-M_PI_4)){
        Roll =  (-1.f) * M_PI_4;
    }
}

void Spaceship::ThrustInput(float Val)
{

	// If input is not held down, reduce speed
	float CurrentAcc = (Val * Acceleration);

	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (this->deltaTime * CurrentAcc);

	// Clamp between MinSpeed and MaxSpeed
    CurrentForwardSpeed = clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void Spaceship::setDeltaTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}
