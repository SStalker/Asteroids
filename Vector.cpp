#include "Vector.h"

Vector::Vector(){
}

Vector::Vector(float x, float y, float z){
  this->X = x;
  this->Y = y;
  this->Z = z;
}


float Vector::length() const{
  float inner = this->lengthSquared();
  float outer = sqrt(inner);

  return outer;
}

float Vector::dot(const  Vector& v) const{
  
  return ( (this->X * v.X) 
         + (this->Y * v.Y) 
         + (this->Z * v.Z)
         );
}
float Vector::lengthSquared() const{
  float lengthToSquare = this->dot(*this);

  return lengthToSquare;
}

Vector& Vector::normalize(){
  float length = this->length();
  
  X /= length;
  Y /= length;
  Z /= length;

  return *this;
}

Vector Vector::cross(const Vector& v) const{
  Vector crossVector;

  crossVector.X = ( this->Y*v.Z - v.Y*this->Z );
  crossVector.Y = ( this->Z*v.X - v.Z*this->X );
  crossVector.Z = ( this->X*v.Y - v.X*this->Y );

  return crossVector;
}

Vector Vector::operator+(const Vector& v) const{
  Vector resultVector;

  resultVector.X = this->X + v.X;
  resultVector.Y = this->Y + v.Y;
  resultVector.Z = this->Z + v.Z;

  return resultVector;
}

Vector Vector::operator-(const Vector& v) const{
  Vector resultVector;

  resultVector.X = this->X - v.X;
  resultVector.Y = this->Y - v.Y;
  resultVector.Z = this->Z - v.Z;

  return resultVector;
}

Vector& Vector::operator+=(const Vector& v){
  this->X += v.X;
  this->Y += v.Y;
  this->Z += v.Z;

  return *this;
}

Vector Vector::operator*(float c) const{
  Vector resultVector;

  resultVector.X = this->X * c;
  resultVector.Y = this->Y * c;
  resultVector.Z = this->Z * c;

  return resultVector;
}

Vector Vector::operator-() const{
  Vector resultVector;

  resultVector.X = -1*this->X;
  resultVector.Y = -1*this->Y;
  resultVector.Z = -1*this->Z;

  return resultVector;
}

/**
 *  vec3 reflect = normalize((2.0 * dot(normal, lightVector) * normal) - lightVector); 
 */
Vector Vector::reflection( const Vector& normal) const{
 Vector reflect;
  reflect = normal * this->dot(normal) * (-2) + *this;
  //reflect = normal * (this->dot(normal)) * 2 - *this;
  reflect.normalize();

  return reflect;
}

bool Vector::triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s ) const{

  // Vektor senkrecht auf Ebene
  Vector n = ((b-a).cross(c-a)).normalize();

  // Länge Richtungsvektor
  float dd = a.dot(n);

  // Strahl s (Skalierung des Richtungsvektors)
  float nDotOrtsvektor = -1*this->dot(n);
  float nDotD = n.dot(d);
  s = (dd+nDotOrtsvektor) / (nDotD);

  if(s < 0){
    return false;
  }

  // Vektor p (Auftreffpunkt auf der Ebene)
  Vector p = (*this)+d*s;

  // Fläche Dreieck abc
  float abc = ( (b-a).cross( c-a ) ).length() / 2;

  // Fläche abp
  float abp = ( (b-a).cross( p-a ) ).length() / 2;

  // Fläche acp
  float acp = ( (c-a).cross( p-a ) ).length() / 2;

  // Fläche bcp
  float bcp = ( (c-b).cross( p-b ) ).length() / 2;

  if(abc+EPSILON >= abp+acp+bcp){
    return true;
  }
  
  return false;
}
