#ifndef VECTOR_H
#define VECTOR_H
#define EPSILON 1e-6

#include <cmath>
#include <iostream>

using namespace std;

class Vector {

public:
  float X;
  float Y;
  float Z;

  Vector();
  Vector(float x, float y, float z);
  
  float dot(const  Vector& v) const;
  float length() const;
  float lengthSquared() const;
  Vector cross(const Vector& v) const;
  Vector& normalize();
  Vector reflection( const Vector& normal) const;

  bool triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s ) const;

  Vector operator+(const Vector& v) const;
  Vector operator-(const Vector& v) const;
  Vector& operator+=(const Vector& v);
  Vector operator*(float c) const;
  Vector operator-() const;

};

inline ostream& operator<<(ostream &s, const Vector &a) {
  return s << "X:" << a.X << " Y: " << a.Y << " Z: " << a.Z << endl;
}

#endif
