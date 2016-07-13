#ifndef RGBIMAGE_H
#define RGBIMAGE_H

#include "Color.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept> 

using namespace std;

class RGBImage{

public:
  RGBImage(unsigned int Width, unsigned int Height);
  ~RGBImage();
  void setPixelColor(unsigned int x, unsigned int y, const Color &c);
  const Color & getPixelColor(unsigned int x, unsigned int y) const;
  bool saveToDisk(const char* Filename);
  unsigned int width() const;
  unsigned int height() const;

  static unsigned char convertColorChannel(float f){
    if(f < 0){ f = 0; }
    if(f > 1){ f = 1; }

  	return static_cast<unsigned char>(f*255);    
  }

protected:
  Color** m_Image;
  unsigned int m_Height;
  unsigned int m_Width;

};

#endif