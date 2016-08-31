#include "Spaceship.h"

Spaceship::Spaceship()
{
	// Set handling parameters
	Acceleration = 0.5f; // 500
	TurnSpeed = 5.f;
	MaxSpeed = 5.f; // 5000
	MinSpeed = 5.f; // 500
	CurrentForwardSpeed = 2.5f; // 500
}

/*bool Spaceship::load( const char* model, const char* VertexShader, const char* FragmentShader, const Vector& startPos)
{
	bool success = false;
    //Load Model to variable
    cout << model << " " << VertexShader << " " << FragmentShader << endl;
    success = load(model, VertexShader, FragmentShader);

    pos = startPos;

    return success;
}*/

void Spaceship::update(float deltaTime)
{
		cout << "CurrentForwardSpeed: " << CurrentForwardSpeed << endl;
		cout << "Yaw: " << CurrentYawSpeed << " Pitch: " << CurrentPitchSpeed << " Roll: " << CurrentRollSpeed << endl;

    pos.Z = CurrentForwardSpeed * deltaTime;
		pos.Y = CurrentPitchSpeed * deltaTime;
		pos.X = CurrentYawSpeed * deltaTime;

		cout << "forward: " << m_position.forward() << endl;
		m_rotation.rotationYawPitchRoll(angleToRadian(CurrentYawSpeed), angleToRadian(CurrentPitchSpeed), 0.f);

		Vector v = m_rotation.forward();

    m_position.translation(m_rotation.forward()*CurrentForwardSpeed*deltaTime);
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

void Spaceship::MoveUpInput(float Val)
{
	// Target pitch speed is based in input
	/*float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = interpolateTo(CurrentPitchSpeed, TargetPitchSpeed, this->deltaTime, 2.f);*/
	CurrentPitchSpeed += Val;

	if(CurrentPitchSpeed >= 360 || CurrentPitchSpeed <= -360)
		CurrentPitchSpeed = 0;
}

void Spaceship::MoveRightInput(float Val)
{
	CurrentYawSpeed += Val;

	if(CurrentYawSpeed >= 360 || CurrentYawSpeed <= -360)
		CurrentYawSpeed = 0;
	// Target yaw speed is based on input
	/*float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = interpolateTo(CurrentYawSpeed, TargetYawSpeed, this->deltaTime, 2.f);

	// Is there any left/right input?
	const bool bIsTurning = abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (2.f * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = interpolateTo(CurrentRollSpeed, TargetRollSpeed, this->deltaTime, 2.f);*/
}

void Spaceship::setDeltaTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}
