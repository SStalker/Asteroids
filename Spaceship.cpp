#include "Spaceship.h"

Spaceship::Spaceship()
{
	// Set handling parameters
	Acceleration = 0.5f; // 500
	TurnSpeed = 75.f;
	MaxSpeed = 15.f; // 5000
	MinSpeed = 5.f; // 500
	CurrentForwardSpeed = 0.f; // 500
}

bool Spaceship::load( const char* model, const char* VertexShader, const char* FragmentShader, const Vector& startPos)
{
	bool success = false;
    //Load Model to variable
    cout << model << " " << VertexShader << " " << FragmentShader << endl;
    success = shipModel.load(model, VertexShader, FragmentShader);

    pos = startPos;

    return success;
}

void Spaceship::update(float deltaTime)
{
    //yaw += g_leftRight*deltaTime;
		//pitch += g_forwardBackward*deltaTime;

		cout << "CurrentForwardSpeed: " << CurrentForwardSpeed << endl;
		cout << "Yaw: " << CurrentYawSpeed << " Pitch: " << CurrentPitchSpeed << " Roll: " << CurrentRollSpeed << endl;

    pos.Z = CurrentForwardSpeed * deltaTime;
		pos.Y = CurrentPitchSpeed * deltaTime;
		pos.X = CurrentYawSpeed * deltaTime;
    //pos.Z += speed * deltaTime;
		m_rotation.rotationYawPitchRoll(CurrentYawSpeed/100, CurrentPitchSpeed/100, 0.f);
    m_position.translation(0.f,0.f, pos.Z);
		//m_position *=m_rotation;
}

void Spaceship::draw()
{
    glPushMatrix();
        glMultMatrixf(m_position*m_rotation);
        shipModel.drawTriangles();
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
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = interpolateTo(CurrentPitchSpeed, TargetPitchSpeed, this->deltaTime, 2.f);
}

void Spaceship::MoveRightInput(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = interpolateTo(CurrentYawSpeed, TargetYawSpeed, this->deltaTime, 2.f);

	// Is there any left/right input?
	const bool bIsTurning = abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	//float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (m_rotation.Roll * -2.f);

	// Smoothly interpolate roll speed
	//CurrentRollSpeed = interpolateTo(CurrentRollSpeed, TargetRollSpeed, this->deltaTime, 2.f);
}

void Spaceship::setPos(const Vector& position)
{
	this->pos = position;
}

const Vector Spaceship::getPos() const
{
	return this->pos;
}

void Spaceship::setPosition(const Matrix& position)
{
	this->m_position = position;
}

const Matrix Spaceship::getPosition() const
{
	return this->m_position;
}

void Spaceship::setRotation(const Matrix& rotation)
{
	this->m_rotation = rotation;
}

const Matrix Spaceship::getRotation() const
{
	return this->m_rotation;
}

void Spaceship::setDeltaTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}
