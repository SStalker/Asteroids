#include "Rgbimage.h"

RGBImage::RGBImage(unsigned int Width, unsigned int Height){
  m_Width = Width;
  m_Height = Height;
  m_Image = new Color*[Width];

  for (int i = 0; i < Width; ++i){
  	m_Image[i] = new Color[Height];
  } 

  for (int i = 0; i < Width; ++i){
  	for (int j = 0; j < Height; ++j){
  		m_Image[i][j].R = 0;
  		m_Image[i][j].G = 0;
  		m_Image[i][j].B = 0;
  	}
  }
}

RGBImage::~RGBImage(){
  for (int i = 0; i < m_Width; ++i)
    delete[] m_Image[i];

  delete[] m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color &c){
  if((x < m_Width) && (y < m_Height)){
    m_Image[x][y] = c;
  }
}

const Color & RGBImage::getPixelColor(unsigned int x, unsigned int y) const{  
  if((x < m_Width) && (y < m_Height))
    return m_Image[x][y];  
  else
    throw std::invalid_argument("Wrong coords");
}

bool RGBImage::saveToDisk(const char* Filename){

  fstream stream;
  stream.open(Filename, fstream::out | fstream::trunc | fstream::binary);

  unsigned char bitmapfileheader[14] = {
      'B','M',    // Kennung "BM"
      0,0,0,0,    // Dateigröße in Bytes
      0,0,0,0,    // bfReserved Standard 0
      54,0,0,0    // Pixeloffset in Bytes
  };

  unsigned char bitmapinfoheader[40] = {
      40,0,0,0,       // Größe des Infoblocks in Bytes (40 Bytes für Windows Version 3)
      0,0,0,0,        // Bildbreite in Pixel
      0,0,0,0,        // Bildhöhe in Pixel
      1,0,            // Ebenen (immer 1)
      24,0,           // Bits pro Pixel. Mögliche Werte sind: 1, 4, 8, 16, 24, 32
      0,0,0,0,        // Kompression: 0 für unkomprimierte Bilddaten.
      0,0,0,0,        // Bildgröße in Bytes
      0,0,0,0,        // Anzahl der Pixel pro Meter in X-Richtung
      0,0,0,0,        // Anzahl der Pixel pro Meter in Y-Richtung
      0,0,0,0,        // Größe der Farbtabelle (Anzahl der Einträge)
      0,0,0,0,        // Wichtige Farben (Anzahl der verwendeten Einträge der Farbtabelle)
  };


  //int padSize  = (4-(m_Width*3)%4)%4;
  int sizeData = m_Width*m_Height*3;// + m_Height*padSize;
  int sizeAll  = sizeData + sizeof(bitmapfileheader) + sizeof(bitmapinfoheader);

  bitmapfileheader[ 2] = (unsigned char)( sizeAll    );
  bitmapfileheader[ 3] = (unsigned char)( sizeAll>> 8);
  bitmapfileheader[ 4] = (unsigned char)( sizeAll>>16);
  bitmapfileheader[ 5] = (unsigned char)( sizeAll>>24);

  bitmapinfoheader[ 4] = (unsigned char)( m_Width   );
  bitmapinfoheader[ 5] = (unsigned char)( m_Width>> 8);
  bitmapinfoheader[ 6] = (unsigned char)( m_Width>>16);
  bitmapinfoheader[ 7] = (unsigned char)( m_Width>>24);

  bitmapinfoheader[ 8] = (unsigned char)( m_Height    );
  bitmapinfoheader[ 9] = (unsigned char)( m_Height>> 8);
  bitmapinfoheader[10] = (unsigned char)( m_Height>>16);
  bitmapinfoheader[11] = (unsigned char)( m_Height>>24);

  //bitmapinfoheader[20] = (unsigned char)( sizeData    );
  //bitmapinfoheader[21] = (unsigned char)( sizeData>> 8);
  //bitmapinfoheader[22] = (unsigned char)( sizeData>>16);
  //bitmapinfoheader[23] = (unsigned char)( sizeData>>24);

  stream.write( (char*)bitmapfileheader, sizeof(bitmapfileheader) );
  stream.write( (char*)bitmapinfoheader, sizeof(bitmapinfoheader) );

  unsigned char pad[3] = {0,0,0};

  for ( int y=m_Height-1; y>=0; y-- )
  {
      for ( int x=0; x<m_Width; x++ )
      {          
          unsigned char red = RGBImage::convertColorChannel(m_Image[x][y].R);
          unsigned char green = RGBImage::convertColorChannel(m_Image[x][y].G);
          unsigned char blue = RGBImage::convertColorChannel(m_Image[x][y].B);


          unsigned char pixel[3];
          pixel[0] = blue;
          pixel[1] = green;
          pixel[2] = red;

          stream.write( (char*)pixel, 3 );
      }
      //stream.write( (char*)pad, padSize );
  }
  return true;
}

unsigned int RGBImage::width() const{
  return this->m_Width;
}

unsigned int RGBImage::height() const{
  return this->m_Height;
}
