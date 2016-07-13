//
//  Texture.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 24.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "Texture.h"
#include "Rgbimage.h"
#include "Color.h"
#include <assert.h>
#include <stdint.h>

#ifndef WIN32
#define DWORD uint32_t
#define WORD __uint16_t
#define LONG int32_t
#endif

typedef struct tagBITMAPFILEHEADER_
{
    WORD bfType;  //specifies the file type
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    DWORD bfReserved;  //reserved; must be 0
    DWORD bOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER_;

typedef struct tagBITMAPINFOHEADER_
{
    DWORD biSize;  //specifies the number of bytes required by the struct
    LONG biWidth;  //specifies width in pixels
    LONG biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by th ebitmap
    DWORD biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER_;


Texture::Texture() : m_TextureID(0), m_pImage(NULL)
{
    
}

Texture::~Texture()
{
    if(m_pImage)
        delete m_pImage;
    m_pImage = NULL;
}

bool Texture::isValid() const
{
    return m_TextureID > 0;
}

bool Texture::LoadFromBMP( const char* Filename )
{
    unsigned int width, height;
    
    unsigned char* data = LoadBMP(Filename, width, height);
    if(data==NULL)
        return false;
    
    if( m_pImage )
        delete m_pImage;
    
    m_pImage = createImage(data, width, height);
    
    glGenTextures(1, &m_TextureID);
    
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glBindTexture(GL_TEXTURE_2D, 0);
    
    delete[] data;
    return true;
}

RGBImage* Texture::createImage( unsigned char* Data, unsigned int width, unsigned int height )
{
    // create CPU accessible image
    RGBImage* pImage = new RGBImage(width, height);
    assert(pImage);
    for( unsigned int i=0; i<height; i++)
		for (unsigned int j = 0; j<width; j++)
        {
            Color c( (float)*(Data)/255.0f, (float)*(Data+1)/255.0f, (float)*(Data+2)/255.0f);
            pImage->setPixelColor(j, i, c);
            Data+=3;
        }

    return pImage;
}

void Texture::apply() const
{
    if(m_TextureID==0)
        return;
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

const RGBImage* Texture::getRGBImage() const
{
    return m_pImage;
}

unsigned char* Texture::LoadBMP(const char* Filename, unsigned int& width, unsigned int& height)
{
    FILE *filePtr;
    BITMAPFILEHEADER_ bitmapFileHeader;
    BITMAPINFOHEADER_ bitmapInfoHeader;
    unsigned char *bitmapImage;
    
    
    filePtr = fopen(Filename,"rb");
    if (filePtr == NULL)
        return NULL;
    
    fread(&bitmapFileHeader.bfType, sizeof(WORD),1,filePtr);
    fread(&bitmapFileHeader.bfSize, sizeof(DWORD),1,filePtr);
    fread(&bitmapFileHeader.bfReserved, sizeof(DWORD),1,filePtr);
    fread(&bitmapFileHeader.bOffBits, sizeof(DWORD),1,filePtr);
    
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }
    
    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER_),1,filePtr);
    
    if(bitmapInfoHeader.biBitCount!=24)
    {
        fclose(filePtr);
        return NULL;
    }
    
    width = bitmapInfoHeader.biWidth;
    height = bitmapInfoHeader.biHeight;
    
    fseek(filePtr, bitmapFileHeader.bOffBits, SEEK_SET);
    bitmapImage = new unsigned char[bitmapInfoHeader.biSizeImage];
    
    if (!bitmapImage)
    {
        delete [] bitmapImage;
        fclose(filePtr);
        return NULL;
    }
    
    fread(bitmapImage,bitmapInfoHeader.biSizeImage,1,filePtr);
    
    if (bitmapImage == NULL)
    {
        delete [] bitmapImage;
        fclose(filePtr);
        return NULL;
    }
    
    //swap the r and b values to get RGB (bitmap is BGR)
    unsigned char tempRGB;
    for ( unsigned int imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage-2;imageIdx+=3)
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }
    
    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}
