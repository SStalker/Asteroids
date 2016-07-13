#include "Color.h"

Color::Color(){
  R = B = G = 0;
}

Color::Color(float r, float g, float b){
  R = r;
  B = b;
  G = g;
}

Color Color::operator*(const Color &c) const{
  Color ret;
  
  ret.R = this->R*c.R;
  ret.G = this->G*c.G;
  ret.B = this->B*c.B;

  return ret;
}

Color Color::operator*(const float Factor) const{
  Color ret;
  
  ret.R = this->R*Factor;
  ret.G = this->G*Factor;
  ret.B = this->B*Factor;

  return ret;
}

Color Color::operator+(const Color &c)const{
  Color ret;
  
  ret.R = this->R+c.R;
  ret.G = this->G+c.G;
  ret.B = this->B+c.B;

  return ret;
}

Color & Color::operator+=(const Color &c){
  this->R += c.R;
  this->G += c.G;
  this->B += c.B;

  return *this;
}
