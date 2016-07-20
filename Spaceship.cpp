#include "Spaceship.h"

Spaceship::Spaceship()
{
	speed = 0.05f;
}

bool Spaceship::load( const char* model, const char* VertexShader, const char* FragmentShader, const Vector& startPos)
{
	bool success = false;
    //Load Model to variable
    cout << model << " " << VertexShader << " " << FragmentShader << endl;
    success = shipModel.load(model, VertexShader, FragmentShader);

    pos = startPos;
    g_forwardBackward = 0;
    g_leftRight = 0;

    return success;
}

void Spaceship::update(float deltaTime)
{
    //g_turnChassis += g_leftRight*deltaTime;

    //m_rotationChassis.rotationY(g_turnChassis);

    pos.X += speed * deltaTime;
    //pos.Z += speed * deltaTime;

    m_position.translation(pos.X, 0.0, 0.0);
}

void Spaceship::draw()
{
    glPushMatrix();
        glMultMatrixf(m_position);
        shipModel.drawTriangles();
    glPopMatrix();
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
