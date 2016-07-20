#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "Model.h"
#include "Matrix.h"
#include "Vector.h"

class Spaceship{

public:
	Spaceship();

	void update(float deltaTime);
    void draw();
    bool load( const char* model, const char* VertexShader, const char* FragmentShader, const Vector& startPos);

	void setPosition(const Matrix& position);
	const Matrix getPosition() const;

	void setPos(const Vector& pos);
	const Vector getPos() const;

private:
	Model shipModel;

	Vector pos;
	Matrix m_position;

	//steer
    float g_forwardBackward;
    float g_leftRight;
    float speed;
};

#endif