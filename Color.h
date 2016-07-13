#ifndef COLOR_H
#define COLOR_H

#include <iostream>

using namespace std;

class Color{

public:
  float R;
  float G;
  float B;

  Color();
  Color(float r, float g, float b);
  Color operator*(const Color &c) const;
  Color operator*(const float Factor) const;
  Color operator+(const Color &c)const;
  Color & operator+=(const Color &c);
};

inline ostream& operator<<(ostream &s, const Color &a) {
  return s << "R:" << a.R << " G: " << a.G << " B: " << a.B << endl;
}

#endif