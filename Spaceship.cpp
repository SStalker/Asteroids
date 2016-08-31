#include "Spaceship.h"

Spaceship::Spaceship()
{
	// Set handling parameters
	Acceleration = 0.5f; // 500
	TurnSpeed = 5.f;
	MaxSpeed = 5.f; // 5000
	MinSpeed = 5.f; // 500
	CurrentForwardSpeed = 2.5f; // 500
    Pitch = 0.f;
    Yaw = 0.f;
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

    pos.Z = CurrentForwardSpeed * deltaTime;
    pos.Y = CurrentPitchSpeed * deltaTime;
    pos.X = CurrentYawSpeed * deltaTime;

    m_rotation.rotationYawPitchRoll(Yaw, Pitch, 0.f);
    m_position.translation(0.f,0.f, pos.Z);

    Matrix combined = m_position * m_rotation;

    g_Camera.setPosition(combined.forward()*-6.f + combined.up()*3.f + combined.translation());
    g_Camera.setTarget(combined.translation() + combined.forward()*8.f);
    g_Camera.setUp(combined.up());
    g_Camera.apply();

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
    Pitch += (-1.f) * forwardBackward * deltaTime * 0.125f; // * CurrentPitchSpeed?

    if(Pitch > 2 * M_PI)
        Pitch-=2 * M_PI;
    else if(Pitch < 0.f)
        Pitch += 2 * M_PI;

    if(Pitch > M_PI_2 && Pitch < (3 * M_PI_2) )
        leftRight *= (-1.f);


    Yaw += (-1.f) * leftRight * deltaTime * 0.125f;

    if(Yaw > 2 * M_PI)
        Yaw -= 2 * M_PI;
    else if(Yaw < 0.f)
        Yaw += 2 * M_PI;

    cout << "Pitch: " << Pitch << endl;
    cout << "Yaw: " << Yaw << endl;

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
