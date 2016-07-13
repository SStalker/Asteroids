//
//  Matrix.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 03.11.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Matrix__
#define __RealtimeRending__Matrix__

#include <iostream>
#include "Vector.h"

class Matrix
{
public:
    union
    {
        struct {
            float m00, m10,m20,m30; // column major storage.
            float m01, m11,m21,m31; // caution: rows and columns are stored swizzeled
            float m02, m12,m22,m32; // The first index denotes the row and the second the column number
            float m03, m13,m23,m33;
        };
        struct { float m[16]; };
    };
    Matrix();
    Matrix( float _00, float _01, float _02, float _03,
            float _10, float _11, float _12, float _13,
            float _20, float _21, float _22, float _23,
            float _30, float _31, float _32, float _33 );

    // special cast methods for opengl-API
    operator float*();
    operator const float* const();

    // basic matrix functionality
    Matrix operator*(const Matrix& M) const;
    Matrix& operator*=(const Matrix& M);
    Vector operator*(const Vector& v) const;
    bool operator==(const Matrix& M);
    bool operator!=(const Matrix& M);

    Matrix& identity();
    Matrix& transpose();
    Matrix& invert();
    Vector transformVec4x4( const Vector& v) const; // transforms a vector with all 4x4 matrix elements (x/w, y/w, z/w)
    Vector transformVec3x3( const Vector& v) const; // transforms a vector with 3x3 matrix elements (only rotation and scaling is applied)
    float determinat();
    
    // The matrix can be interpreted as coordinate system, if so the following
    // methods return the corresponding main axis of the (orthogonal) coordinate system
    // and its origin (translation)
    Vector left() const;
    Vector right() const;
    Vector up() const;
    Vector down() const;
    Vector forward() const;
    Vector backward() const;
    Vector translation() const;
    void up( const Vector& v);
    void forward( const Vector& v);
    void right( const Vector& v);
    
    // affine matrix transformation methods
    Matrix& multiply(const Matrix& M );
    Matrix& translation(float X, float Y, float Z );
    Matrix& translation(const Vector& XYZ );
    Matrix& rotationX(float Angle );
    Matrix& rotationY(float Angle );
    Matrix& rotationZ(float Angle );
    Matrix& rotationYawPitchRoll( float Yaw, float Pitch, float Roll );
    Matrix& rotationYawPitchRoll(const Vector& Angles );
    Matrix& rotationAxis(const Vector& Axis, float Angle);
    Matrix& scale(float ScaleX, float ScaleY, float ScaleZ );
    Matrix& scale(const Vector& Scalings );
    Matrix& scale(float Scaling );
    Matrix& lookAt(const Vector& Target, const Vector& Up, const Vector& Position );
    
    // projective matrix transformation methods
    Matrix& perspective(float Fovy, float AspectRatio, float NearPlane, float FarPlane ); //Fovy=Field of View Y, AspectRatio=WindowWidth/WindowHeight
    Matrix& orthographic(float Width, float Height, float Near, float Far );
};


#endif /* defined(__RealtimeRending__Matrix__) */
