#include "Spaceship.h"
#include "Game.h"


/**
 * Constructor
 *
 * @brief Spaceship::Spaceship
 */
Spaceship::Spaceship()
{
	// Set handling parameters
    Acceleration = 20.f;
    TurnSpeed = 0.325f;// 0.125f;
    MaxSpeed = 20.f;
    MinSpeed = 2.f;
    CurrentForwardSpeed = 2.f;
    SpeedMult = 1.f;
    Pitch = 0.f;
    Yaw = 0.f;
    Roll = 0.f;
    camShipBaseDistFoward = -6.f;
    camShipBaseDistUp = 3.f;

    loadRessources("assets/model/SpaceShip.obj", "assets/shader/PhongVertexShader.glsl", "assets/shader/PhongFragmentShader.glsl");
}


/**
 * Update ship
 *
 * @brief Spaceship::update
 * @param deltaTime
 */
void Spaceship::update(float deltaTime)
{
    //Check if spaceship is still alive
    alive();

    //Rotate ship
    m_rotation.rotationYawPitchRoll(Yaw, Pitch, 0.f);

    //Calculate position difference
    Vector addUp;
    addUp.X = CurrentForwardSpeed * deltaTime * m_rotation.forward().X;
    addUp.Y = CurrentForwardSpeed * deltaTime * m_rotation.forward().Y;
    addUp.Z = CurrentForwardSpeed * deltaTime * m_rotation.forward().Z;

    //If difference adding up with distance is smaller than half farplane distance then addUp the difference
    if(pos.length() + addUp.length() < fPlane/2 && pos.length() - (addUp.length() * 2.f) < fPlane/2 ){
        pos.X += addUp.X;
        pos.Y += addUp.Y;
        pos.Z += addUp.Z;

        //Set new position
        m_position.translation(pos.X, pos.Y, pos.Z);
    }

    //Create camera rotration with
    Matrix camRot = m_rotation;
    camRot.rotationYawPitchRoll(Yaw, Pitch, -Roll);

    //Combine matrices for the camera and set ist new values
    Matrix combined = m_position * camRot;
    g_Camera.setPosition(combined.forward() * (camShipBaseDistFoward - SpeedMult ) + combined.up() * (camShipBaseDistUp + SpeedMult) + combined.translation());
    g_Camera.setTarget(combined.translation() + combined.forward()*8.f);
    g_Camera.setUp(combined.up());
    g_Camera.apply();

    //Slow down
    ThrustInput(-0.2f);

    //Roll back
    if(Roll != 0.f){
        if(Roll > 0)
            Roll -= deltaTime * 0.1f * SpeedMult;
        else if(Roll < 0)
            Roll += deltaTime * 0.1f * SpeedMult;
    }
}


/**
 * Draw spaceship after transformation
 *
 * @brief Spaceship::draw
 */

void Spaceship::draw()
{
    glPushMatrix();
        glMultMatrixf(m_position*m_rotation);
        drawTriangles();
    glPopMatrix();
}


/**
 * Function to process player input
 *
 * @brief Spaceship::steer
 * @param forwardBackward
 * @param leftRight
 */

void Spaceship::steer(float forwardBackward, float leftRight)
{

    float additionSubtraction = 1.f;

    //Calculate pitch value
    Pitch += (0.5f  + SpeedMult) * forwardBackward * deltaTime * TurnSpeed;

    //Clamp pitch value
    if(Pitch > 2 * M_PI){
        Pitch-=2 * M_PI;
    }else if(Pitch < 0.f){
        Pitch += 2 * M_PI;
    }

    //Change orientation between 90 and 270 degrees
    if(Pitch > M_PI_2 && Pitch < (3 * M_PI_2) ){
        leftRight *= (-1.f);
        additionSubtraction = -1.f;
    }

    //Calculate yaw value
    Yaw -= (0.5f  + SpeedMult) * leftRight * deltaTime * TurnSpeed;

    //Clamp yaw value
    if(Yaw > 2 * M_PI){
        Yaw -= 2 * M_PI;
    }else if(Yaw < 0.f){
        Yaw += 2 * M_PI;
    }

    //Calc a slight roll
    Roll += leftRight * deltaTime * TurnSpeed * additionSubtraction;

    //Clamp role between a small value
    if(Roll > M_PI_4/2){
        Roll =  M_PI_4/2;
    }else if(Roll < (-M_PI_4/2)){
        Roll =  (-1.f) * M_PI_4/2;
    }
}


/**
 * Calculate thrust
 *
 * @brief Spaceship::ThrustInput
 * @param Val
 */

void Spaceship::ThrustInput(float Val)
{

	// If input is not held down, reduce speed
	float CurrentAcc = (Val * Acceleration);

	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (this->deltaTime * CurrentAcc);

	// Clamp between MinSpeed and MaxSpeed
    CurrentForwardSpeed = clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
    SpeedMult = CurrentForwardSpeed > 10 ? CurrentForwardSpeed/10 : 1.f;
}


void Spaceship::setDeltaTime(float deltaTime)
{
	this->deltaTime = deltaTime;
}


vector<Projectile*> Spaceship::getProjectiles() const
{
    return this->projectiles;
}

/**
 * Function to fire a new projectile
 *
 * @brief Spaceship::fire
 */
void Spaceship::fire()
{
    //Combine to spaceship position
    Matrix combined = m_position * m_rotation;

    //Generate new projectile
    Projectile *p = new Projectile(pos, combined.forward(), 0.5);
    p->setName("Projectile" + to_string(projectiles.size()));

    //play sound and add to projectile list
    SoundManager::getInstance()->playShootingSound();
    Game::getInstance()->getProjectileList()->push_back(p);
}
