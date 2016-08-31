//
//  Camera.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 22.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "Camera.h"

#ifdef WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/GLUT.h>
	#include <GL/GL.h>
#else
	#include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include <math.h>

Camera::Camera() : m_Position(0.0f,5.0f,-5.0f), m_Target(0.0f,0.0f,0.0f), m_Up(0.0f,1.0f,0.0f), m_LastMouseX(-1), m_LastMouseY(-1), m_Panning(0,0,0), m_Zoom(0,0,0), m_Rotation(0,0,0)
{

}

Camera::Camera(Vector& Pos, Vector& Target, Vector& Up)
{
    m_Up = Up;
    m_Position = Pos;
    m_Target = Target;
}

Vector Camera::getPosition()
{
    return m_Position + m_Panning + m_Zoom + m_Rotation;
}
Vector Camera::getTarget()
{
    return m_Target + m_Panning;
}
Vector Camera::getUp()
{
    return m_Up;
}

Quaternion Camera::getView()
{
	return View;
}

void Camera::setPosition( const Vector& Pos)
{
    m_Position = Pos;
    m_Panning = m_Rotation = m_Zoom = Vector(0,0,0);


}
void Camera::setTarget( const Vector& Target)
{
    m_Target = Target;
    m_Panning = Vector(0,0,0);
}
void Camera::setUp( const Vector& Up)
{
    m_Up = Up;
}

void Camera::rotate(double angle, double x, double y, double z)
{
	Quaternion t,v,r;

	t.x = x*sin(angle/2);
	t.y = y*sin(angle/2);
	t.z = z*sin(angle/2);
	t.w = cos(angle/2);

	v.x = View.x;
	v.y = View.y;
	v.z = View.z;
	v.w = 0;

	//Quaternion tmp = t.multiply( );
	r = Quaternion::multiply( Quaternion::multiply(t,v), t.conjugate() );

	View.x = r.x;
	View.y = r.y;
	View.z = r.z;
}

void Camera::mouseInput( int x, int y, int Button, int State)
{
    if(State == GLUT_DOWN)
    {
        if(m_LastMouseX==-1) m_LastMouseX = x;
        if(m_LastMouseY==-1) m_LastMouseY = y;

        if( Button == GLUT_LEFT_BUTTON )
        {
            rotate((float)x, (float)y );
        }
        else if( Button == GLUT_RIGHT_BUTTON)
        {
            pan( (float)(m_LastMouseX-x)*0.01f, (float)(m_LastMouseY-y)*0.01f );
        }
        else if( Button == GLUT_MIDDLE_BUTTON)
        {
            zoom( (float)(m_LastMouseY-y)*0.01f );

        }
    }
    else
    {
        m_Position += m_Panning + m_Zoom + m_Rotation;
        m_Target += m_Panning;
        m_Panning = Vector(0,0,0);
        m_Zoom = Vector(0,0,0);
        m_Rotation = Vector(0,0,0);
        m_LastMouseX = -1;
        m_LastMouseY = -1;

    }
}

void Camera::pan( float dx, float dy)
{
    // calculate panning-plane

    Vector aDir = m_Target-m_Position;
    aDir.normalize();
    Vector aRight = aDir.cross(m_Up);
    aRight.normalize();
    Vector aUp = aDir.cross(aRight);
    m_Panning = aRight * dx + aUp * dy;
}

void Camera::zoom( float dz)
{
    Vector aDir = m_Target-m_Position;
    float Dist = aDir.length();
    aDir.normalize();

    if( Dist-dz <= 1.0f)
    {
        m_Zoom = aDir * (Dist-1.0f);
        return;
    }

     m_Zoom = aDir * dz;
}

void Camera::rotate( float x, float y )
{
    Vector po = getVSpherePos((float) m_LastMouseX, (float)m_LastMouseY);
    Vector pn = getVSpherePos(x, y);

    if((po-pn).lengthSquared() < 0.0001f )
        return;

    float cosangle = po.dot(pn);
    if(cosangle>1.0f) cosangle=1.0f;
    if(cosangle<-1.0f) cosangle=-1.0f;

    const float angle = acos(cosangle);
    Vector RotAxis = pn.cross(po);
    RotAxis.normalize();


    //Vector Diff = m_Position-m_Target;
    Vector Diff(0,0,(m_Position-m_Target).length());

    Vector RotDiff = rotateAxisAngle(Diff, RotAxis, angle);

    Vector cdir = m_Target-m_Position;
    cdir.normalize();
    Vector cup = m_Up;
    Vector cright = cdir.cross(cup);
    cright.normalize();
    cup = cright.cross(cdir);

    Vector RotDiffW;
    RotDiffW.X = cright.X * RotDiff.X + cup.X * RotDiff.Y +  -cdir.X * RotDiff.Z;
    RotDiffW.Y = cright.Y * RotDiff.X + cup.Y * RotDiff.Y +  -cdir.Y * RotDiff.Z;
    RotDiffW.Z = cright.Z * RotDiff.X + cup.Z * RotDiff.Y +  -cdir.Z * RotDiff.Z;
    m_Rotation = RotDiffW - (m_Position-m_Target);
}

Vector Camera::rotateAxisAngle( Vector v, Vector n, float a)
{
    float co = cos(a);
    float si = sin(a);

    Vector o;
    Vector mx( n.X*n.X*(1.0f-co)+co, n.X*n.Y*(1.0f-co)-n.Z*si,n.X*n.Z*(1.0f-co)+n.Y*si );
    Vector my( n.X*n.Y*(1.0f-co)+n.Z*si, n.Y*n.Y*(1.0f-co)+co, n.Y*n.Z*(1.0f-co)-n.X*si );
    Vector mz( n.X*n.Z*(1.0f-co)-n.Y*si, n.Z*n.Y*(1.0f-co)+n.X*si, n.Z*n.Z*(1.0f-co)+co);
    o.X = mx.dot(v);
    o.Y = my.dot(v);
    o.Z = mz.dot(v);

    return o;
}


Vector Camera::getVSpherePos( float x, float y)
{
    Vector p( 1.0f*x/(float)g_WindowWidth*2.0f - 1.0f, 1.0f*y/(float)g_WindowHeight*2.0f -1.0f, 0);
    p.Y = -p.Y;

    float sqrLen = p.lengthSquared();

    if( sqrLen <= 1.0f)
    {
        p.Z = sqrt( 1- sqrLen);
    }
    else
        p.normalize();

    return p;

}

void Camera::apply()
{
    Vector Pos = getPosition(); //m_Position + m_Panning + m_Zoom + m_Rotation;
    Vector Target = getTarget(); //m_Target + m_Panning;

    gluLookAt(Pos.X, Pos.Y, Pos.Z, Target.X, Target.Y, Target.Z, m_Up.X, m_Up.Y, m_Up.Z);

}
