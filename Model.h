//
//  Model.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Model__
#define __RealtimeRending__Model__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <limits>
#include <stdio.h>

#include "ShaderProgram.h"
#include "Material.h"
#include "Vector.h"
#include "Color.h"
#include "global.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define GL_DEBUG

#ifdef GL_DEBUG
#define CHECKGL CheckGLErrors();
#else
#define CHECKGL ;
#endif

void CheckGLErrors();

using namespace std;

struct Usemtl
{
    string mtl;
    unsigned int byCount;
};

struct MyFace
{
    unsigned int pidx[4], tidx[4];
    bool hasTextureCoords = true;
};

struct Vertex
{
    Vertex();
    Vertex( const Vector& p, const Vector& n, float TexS, float TexT);
    Vector Position;
    Vector Normal;
    float  TexcoordS;
    float  TexcoordT;
};

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox( const Vector& min, const Vector& max);
    void calculateAllPoints();
    Vector Min;
//    Vector BaseMin;
    Vector Max;
//    Vector BaseMax;
    Vector allPoints[8];
    Vector allPointsBase[8];
};

class BoundingSphere
{
public:
    BoundingSphere();
    BoundingSphere(const Vector &center, const float &radius);
    Vector Center;
    Vector BaseCenter;
    float Radius;
};


class Model
{
public:
    Model();
    ~Model();
    const BoundingBox& boundingBox() const;
    const BoundingSphere& boundingSphere() const;
    bool load( const char* Filename, bool FitSize=true);
    bool load( const char* Filename, const char* VertexShader, const char* FragmentShader, bool FitSize=true);
    bool loadMtl(string filename, string path);
    void buildVerteciesForTexture();
    void drawLines() const;
    void drawTriangles() const;
    void drawBounding() const;
    // projectOnVector(const Vector& v);

    // Hilfsfunktionen
    vector<string> & split(const string &s, char delim, vector<string> &elems);
    vector<string> split(const string &s, char delim);
    //vector<Vector*> getAllNormals();

    void createFace(string line);
    void createVertices();
    void calcBounding(const Vector& a, const Vector& b, const Vector& c);
    void calcSphere();
    void scale();
    void convertToFloat(vector<float>& color, const vector<string> &splitted);
    void drawSphere();

protected:
    Material* m_pMaterials;
    unsigned int m_MaterialCount;
    Vertex* m_pVertices;
    unsigned int m_VertexCount;
    BoundingBox m_Box;
    BoundingSphere m_Sphere;

    Usemtl* m_pMaterialsAt;
    unsigned int m_MaterialAtCount;

    GLuint vertexBuffer;
    GLuint indexBuffer;
    vector<GLuint> indizes;

    vector<unsigned int> mtlChange;

    vector<Vector*> my_vertices;
    //vector<Vector*> my_normals;
    vector<Vector*> my_texture_points;
    vector<MyFace*> faces;
    vector<Material*> mtl;


private:
    ShaderProgram sp;
};

#endif /* defined(__RealtimeRending__Model__) */
