//
//  Model.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifdef WIN32
    #include <Windows.h>
    #include <GL/glew.h>
    #include <GL/GLUT.h>
    #include <GL/GL.h>
#elif __APPLE__
    #include <OpenGL/OpenGL.h>
    #include <GLUT/GLUT.h>
#else
    #include <GL/gl.h>
    #include <GL/glut.h>
#endif

#include "Model.h"
#include <vector>
#include <assert.h>
#include <math.h>
#include <map>
#include <float.h>
#include <cstring>
#include <algorithm>

Vertex::Vertex()
{

}

Vertex::Vertex( const Vector& p, const Vector& n, float TexS, float TexT)
{
    Position = p;
    Normal = n;
    TexcoordS = TexS;
    TexcoordT = TexT;
}

BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox( const Vector& min, const Vector& max) : Min(min), Max(max)
{
}

void BoundingBox::calculateAllPoints()
{
    allPoints[0] = allPointsBase[0] = Vector(Min.X, Min.Y, Min.Z); //1
    allPoints[1] = allPointsBase[1] = Vector(Max.X, Min.Y, Min.Z); //2
    allPoints[2] = allPointsBase[2] = Vector(Min.X, Max.Y, Min.Z); //3
    allPoints[3] = allPointsBase[3] = Vector(Max.X, Max.Y, Min.Z); //4
    allPoints[4] = allPointsBase[4] = Vector(Min.X, Min.Y, Max.Z); //5
    allPoints[5] = allPointsBase[5] = Vector(Max.X, Min.Y, Max.Z); //6
    allPoints[6] = allPointsBase[6] = Vector(Min.X, Max.Y, Max.Z); //7
    allPoints[7] = allPointsBase[7] = Vector(Max.X, Max.Y, Max.Z); //8

//    for(unsigned int i = 0; i < 8; i++)
//        cout << allPoints[i];

}

BoundingSphere::BoundingSphere()
{
}

BoundingSphere::BoundingSphere(const Vector& center, const float& radius ) : Center(center), Radius(radius)
{
}

Model::Model() : m_pVertices(NULL), m_pMaterials(NULL), m_MaterialCount(0), m_VertexCount(0)
{
}

Model::~Model()
{
    if( m_pVertices)
        delete [] m_pVertices;
    if(m_pMaterials)
        delete [] m_pMaterials;

    for(Vector *v : my_vertices)
        delete v;

    for(Vector *v : my_texture_points)
        delete v;

    for(MyFace *f : faces)
        delete f;
}

bool Model::load( const char* Filename, bool FitSize)
{
    ifstream file;
    string line;
    string identifier;
    string mtlName;

    vector<Usemtl*> usemtl;

    cout << Filename << endl;

    file.open(Filename);

    if(file.is_open())
    {
        while(getline(file, line))
        {
            if(line.substr(0,2) == "# ")
            {
                //cout << "Kommentar: " << line.substr(2) << endl;
            }
            else if(line.substr(0,7) == "mtllib ")
            {
                mtlName = line.substr(7);
//                cout << "Einbindung einer Materialbibliothek: " << line.substr(7) << endl;
            }
            else if(line.substr(0,2) == "o ")
            {
                //cout << "Objektname: " << line.substr(2) << endl;
            }
            else if(line.substr(0,2) == "g ")
            {
//                cout << "Gruppename: " << line.substr(2) << endl;
            }
            else if(line.substr(0,2) == "v ")
            {
                //cout << "Vertex: " << line.substr(2) << endl;
                vector<string> list = split(line.substr(2), ' ');


                my_vertices.push_back( new Vector( stof(list.at(0)), stof(list.at(1)), stof(list.at(2)) ) );
            }
            else if(line.substr(0,3) == "vt ")
            {
                //cout << "Beschreibung eines Texturkoordinatenpunktes: " << line.substr(3) << endl;
                vector<string> list = split(line.substr(2), ' ');
                if(list.size() == 2)
                  my_texture_points.push_back( new Vector( stof(list.at(0)), stof(list.at(1)), 0.f ) );
                else
                  my_texture_points.push_back( new Vector( stof(list.at(0)), stof(list.at(1)), stof(list.at(2)) ) );

            }
            else if(line.substr(0,3) == "vn ")
            {
                //cout << "Beschreibung einer Normale: " << line.substr(3) << endl;
            }
            else if(line.substr(0,2) == "f ")
            {
                //faces.push_back(createFace(line.substr(2)));
                //cout << "Beschreibung einer Fläche: " << line.substr(2) << endl;
                createFace(line.substr(2));
            }
            else if(line.substr(0,7).compare("usemtl ") == 0){
                //do something with this information
                Usemtl* material = new Usemtl();
                material->byCount = m_VertexCount;

                string mtl = line.substr(7,line.back());
                mtl.erase(mtl.find_last_not_of(" \n\r\t")+1);

                material->mtl = mtl;
                usemtl.push_back(material);
                m_MaterialAtCount++;

//                cout << m_MaterialAtCount << endl;
            }
        }
        file.close();

//        cout << "Count vertices = " << my_vertices.size() << endl;
//        cout << "Count texture vertices = " << my_texture_points.size() << endl;
//        cout << "Count faces = " << faces.size() << endl;
    }
    else
    {
        return false;
    }

    //eingelesene werte in m_pVertices schreiben und boundingbox ermitteln
    createVertices();

    calcSphere();
    // scale
    if(FitSize){
        scale();
    }


    //Calc all points of bbox
    m_Box.calculateAllPoints();

    //materails laden
    string path = Filename;

    //for linux
    size_t pos = path.rfind("/");

//    cout << "Pos: " << pos << endl;

    if(pos == string::npos)
        path = "";
    else
        path = path.substr(0, pos+1);

//    cout << "########" << path << " ++++++ " << mtlName << endl;

    if(!loadMtl(mtlName, path)){
        cout << "No mtl was found!" << endl;
    }else{
//        cout << "Used Materials: " << endl;
//        for(int i = 0; i < m_MaterialCount; i++){
//            cout << m_pMaterials[i].getName() << endl;
//        }
    }

    //bind material position to model
    m_pMaterialsAt = new Usemtl[usemtl.size()];
    for(int i = 0; i < usemtl.size(); i++){
        m_pMaterialsAt[i] = *usemtl.at(i);
//        cout << m_pMaterialsAt[i].mtl << ": " << m_pMaterialsAt[i].byCount << endl;
    }


    return true;
}

bool Model::load( const char* Filename, const char* VertexShader, const char* FragmentShader, bool FitSize){

    bool success = load(Filename, FitSize);

    //build Vertices for texture
    buildVerteciesForTexture();

    if(!sp.load(VertexShader, FragmentShader)){
        cout << "V or F" << endl;
        exit(2);
    }

    string* ErrorMsg = new string();

    if(!sp.compile(ErrorMsg)){
        cout << *ErrorMsg << endl;
        exit(3);
    }

    sp.activate();

    return success;
}

bool Model::loadMtl(string filename, string path)
{
    ifstream file;
    string line;

    Material* current = NULL;
    vector<string> splitted;

    string toOpen = path;

    toOpen.append(filename);
    toOpen.erase(toOpen.find_last_not_of(" \n\r\t")+1);
//    cout << "toOpen: " << toOpen << endl;
    file.open(toOpen);

    cout << file.is_open() << endl;

    if(file.is_open()){
        while(getline(file, line)){
            splitted = split(line, ' ');
            if(splitted.size() > 0 ){

                //remove tabbulator from string
                if(splitted[0].substr(0,1).compare("\t") == 0){
                    splitted[0] = splitted[0].substr(1,splitted[0].back());
                }

                for(string split: splitted){
                    split.erase(split.find_last_not_of(" \n\r\t")+1);
                }

                if(splitted[0].compare("newmtl") == 0){
//                    cout << "New Mtl" << endl;
                    mtl.push_back(new Material());
                    current = mtl.back();
                    current->setName(splitted[1]);
                }else if(splitted[0].compare("Kd") == 0){
                    cout << "Kd" << endl;
                    vector<float> color;
                    convertToFloat(color, splitted);
                    cout << color[0] << " | " << color[1] << " | " << color[2] << endl;
                    current->setDiffuseColor(Color(color[0], color[1], color[2]));
                }else if(splitted[0].compare("Ks") == 0){
//                    cout << "Ks" << endl;
                    vector<float> color;
                    convertToFloat(color, splitted);
                    current->setSpecularColor(Color(color[0], color[1], color[2]));
                }else if(splitted[0].compare("Ka") == 0){
//                    cout << "Ka" << endl;
                    vector<float> color;
                    convertToFloat(color, splitted);
                    current->setAmbientColor(Color(color[0], color[1], color[2]));
                }else if(splitted[0].compare("Ns") == 0){
//                    cout << "Ns" << endl;
                    vector<float> color;
                    convertToFloat(color, splitted);
                    current->setSpecularExponent(color[0]);
                }else if(splitted[0].compare("map_Kd") == 0){
//                    cout << "map_Kd" << endl;
//                    cout << (path+splitted[1]).length() << " " << (path+splitted[1]).c_str() << endl;
                    current->setDiffuseTexture((path+splitted[1]).c_str());
                }
            }
        }

        file.close();

    }else{
        return false;
    }

    m_pMaterials = new Material[mtl.size()];

    for(int i = 0; i< mtl.size(); i++){
        m_pMaterials[i] = *mtl.at(i);
    }

    m_MaterialCount = mtl.size();

    return true;
}

void Model::buildVerteciesForTexture()
{

    Usemtl* nextMat = &m_pMaterialsAt[0];
    //temp. map for material names and their vertices
    map<string, vector<Vertex>> MTLVertices;
    Material mtl;
    unsigned int counter = 1;

    //fill map with all keys
    for(int i = 0; i< m_MaterialCount; i++){
        if(MTLVertices.find(m_pMaterials[i].getName()) == MTLVertices.end()){
            vector<Vertex> tmp;
            MTLVertices.insert(pair< string, vector<Vertex> >(m_pMaterials[i].getName(), tmp));
        }
    }

    //Iterate over all vertices and add them to their material array in vertices
    for(unsigned int i = 0; i<m_VertexCount; i+=3){

        if(nextMat->byCount == i && counter <= m_MaterialAtCount){
            for(int i = 0; i< m_MaterialCount; i++){
                if(m_pMaterials[i].getName().compare(nextMat->mtl) == 0){
                    mtl = m_pMaterials[i];
                    cout << mtl.getName()<< endl;
                }
            }

            if(counter < m_MaterialAtCount){
                nextMat = &m_pMaterialsAt[counter];
                counter++;
            }
        }

        MTLVertices[mtl.getName()].push_back(m_pVertices[i]);
        MTLVertices[mtl.getName()].push_back(m_pVertices[i+1]);
        MTLVertices[mtl.getName()].push_back(m_pVertices[i+2]);
    }


    //Now build just one vector
    vector<Vertex> all;
    all.reserve(m_VertexCount);

    unsigned int value = 0;

    for(auto& kv: MTLVertices){
        value += kv.second.size();
        mtlChange.push_back(value);
        for(Vertex v :kv.second){
            all.push_back(v);
        }
    }

    //Generate indices
    indizes.reserve(all.size());
    for(unsigned int i = 0; i < all.size(); i+=3){
        indizes.push_back(i);
        indizes.push_back(i+1);
        indizes.push_back(i+2);
    }


    //Generate buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*all.size(), &(all.at(0)), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indizes.size(), &(indizes.at(0)), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

const BoundingBox& Model::boundingBox() const
{
    return m_Box;
}

void Model::drawLines() const
{
    // Aufgabe 1

    CheckGLErrors();
    glBegin(GL_LINES);
    for(unsigned int i = 0; i<m_VertexCount; i+=3){

        glColor3f(0.8,0,0.8);

        glVertex3f(m_pVertices[i].Position.X,m_pVertices[i].Position.Y,m_pVertices[i].Position.Z);
        glVertex3f(m_pVertices[i+1].Position.X,m_pVertices[i+1].Position.Y,m_pVertices[i+1].Position.Z);

        glVertex3f(m_pVertices[i+1].Position.X,m_pVertices[i+1].Position.Y,m_pVertices[i+1].Position.Z);
        glVertex3f(m_pVertices[i+2].Position.X,m_pVertices[i+2].Position.Y,m_pVertices[i+2].Position.Z);

        glVertex3f(m_pVertices[i+2].Position.X,m_pVertices[i+2].Position.Y,m_pVertices[i+2].Position.Z);
        glVertex3f(m_pVertices[i].Position.X,m_pVertices[i].Position.Y,m_pVertices[i].Position.Z);

    }
    glEnd();
}

void Model::drawTriangles() const
{
    CheckGLErrors();

    GLuint lightPos = sp.getParameterID("LightPos");
    GLuint lightColor = sp.getParameterID("LightColor");
    GLuint diffID = sp.getParameterID("DiffColor");
    GLuint specID = sp.getParameterID("SpecColor");
    GLuint ambID = sp.getParameterID("AmbientColor");
    GLuint specExpID = sp.getParameterID("SpecExp");
    GLuint textureID = sp.getParameterID("DiffuseTexture");

    Material mtl;
    int mtlNumber = 0;
    GLuint start = 0;

    //APPLY TEXTURES


    //DRAW ALL VERTICES FOR TEXTURE
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // setup position & normal pointers
    glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
    glNormalPointer( GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12)); // BUFFER_OFFSET(12) = Pointer auf erste Normale
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(24));

    //Setup Texture for shader
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);

    for(unsigned int nextMaterial: mtlChange){
        //disable old texture??
//        glDisable(GL_TEXTURE_2D);


        //fetch the right material
        mtl = m_pMaterials[mtlNumber];


        //Set parameter for Shader
        sp.setParameter(diffID,  mtl.getDiffuseColor());
        sp.setParameter(specID, mtl.getSpecularColor());
        sp.setParameter(ambID, mtl.getAmbientColor());
        sp.setParameter(specExpID, mtl.getSpecularExponent());
        sp.setParameter(textureID, 0);
        sp.setParameter(lightPos, g_LightPos);
        sp.setParameter(lightColor, Color(1.,1.,1.));
        CheckGLErrors();
        //Apply texture
        mtl.getDiffuseTexture().apply();
//        //Replace old texture
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        // we draw our plane for every material
        glDrawElements( GL_TRIANGLES, (int)(nextMaterial-start) , GL_UNSIGNED_INT, BUFFER_OFFSET(start* sizeof(GLuint)) );
        mtlNumber++;
        start = nextMaterial;
    }

    // disable states in reverse order
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Model::drawBounding() const
{
    CheckGLErrors();
    glBegin(GL_LINES);

    glVertex3f(m_Box.allPoints[0].X,m_Box.allPoints[0].Y,m_Box.allPoints[0].Z); //1
    glVertex3f(m_Box.allPoints[1].X,m_Box.allPoints[1].Y,m_Box.allPoints[1].Z); //2

    glVertex3f(m_Box.allPoints[1].X,m_Box.allPoints[1].Y,m_Box.allPoints[1].Z); //2
    glVertex3f(m_Box.allPoints[3].X,m_Box.allPoints[3].Y,m_Box.allPoints[3].Z); //4

    glVertex3f(m_Box.allPoints[3].X,m_Box.allPoints[3].Y,m_Box.allPoints[3].Z); //4
    glVertex3f(m_Box.allPoints[2].X,m_Box.allPoints[2].Y,m_Box.allPoints[2].Z); //3

    glVertex3f(m_Box.allPoints[2].X,m_Box.allPoints[2].Y,m_Box.allPoints[2].Z); //3
    glVertex3f(m_Box.allPoints[0].X,m_Box.allPoints[0].Y,m_Box.allPoints[0].Z); //1

    glVertex3f(m_Box.allPoints[0].X,m_Box.allPoints[0].Y,m_Box.allPoints[0].Z); //1
    glVertex3f(m_Box.allPoints[4].X,m_Box.allPoints[4].Y,m_Box.allPoints[4].Z); //5

    glVertex3f(m_Box.allPoints[4].X,m_Box.allPoints[4].Y,m_Box.allPoints[4].Z); //5
    glVertex3f(m_Box.allPoints[5].X,m_Box.allPoints[5].Y,m_Box.allPoints[5].Z); //6

    glVertex3f(m_Box.allPoints[5].X,m_Box.allPoints[5].Y,m_Box.allPoints[5].Z); //6
    glVertex3f(m_Box.allPoints[7].X,m_Box.allPoints[7].Y,m_Box.allPoints[7].Z); //8

    glVertex3f(m_Box.allPoints[7].X,m_Box.allPoints[7].Y,m_Box.allPoints[7].Z); //8
    glVertex3f(m_Box.allPoints[6].X,m_Box.allPoints[6].Y,m_Box.allPoints[6].Z); //7

    glVertex3f(m_Box.allPoints[2].X,m_Box.allPoints[2].Y,m_Box.allPoints[2].Z); //3
    glVertex3f(m_Box.allPoints[6].X,m_Box.allPoints[6].Y,m_Box.allPoints[6].Z); //7

    glVertex3f(m_Box.allPoints[4].X,m_Box.allPoints[4].Y,m_Box.allPoints[4].Z); //5
    glVertex3f(m_Box.allPoints[6].X,m_Box.allPoints[6].Y,m_Box.allPoints[6].Z); //7

    glVertex3f(m_Box.allPoints[3].X,m_Box.allPoints[3].Y,m_Box.allPoints[3].Z); //4
    glVertex3f(m_Box.allPoints[7].X,m_Box.allPoints[7].Y,m_Box.allPoints[7].Z); //8

    glVertex3f(m_Box.allPoints[1].X,m_Box.allPoints[1].Y,m_Box.allPoints[1].Z); //2
    glVertex3f(m_Box.allPoints[5].X,m_Box.allPoints[5].Y,m_Box.allPoints[5].Z); //6

    glEnd();
}


vector<string> &Model::split(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if (!item.empty()) elems.push_back(item);
    }
    return elems;
}


vector<string> Model::split(const string &s, char delim)
{
    vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

// 2867/3783 2881/3798 2894/3812 2880/3797
void Model::createFace(string line)
{
    int status = 0;
    // geometricVertex
    unsigned int gv[4] = {0,0,0,0};
    // textureVertex
    unsigned int tv[4] = {0,0,0,0};

    vector<string> listSeperatedBySpace = split(line, ' ');
    if(!listSeperatedBySpace.empty())
    {
        size_t found = listSeperatedBySpace[0].find("//");
        if(found != string::npos)
        {
            status = sscanf(line.c_str(), "%i//%i %i//%i %i//%i %i//%i", &gv[0], &tv[0], &gv[1], &tv[1], &gv[2], &tv[2], &gv[3], &tv[3]);
            // f 2//1 3//1 4//1
            // Found an // go further
        }
        else
        {
            size_t found = listSeperatedBySpace[0].find("/");
            if(found != string::npos)
            {
                status = sscanf(line.c_str(), "%i/%i %i/%i %i/%i %i/%i", &gv[0], &tv[0], &gv[1], &tv[1], &gv[2], &tv[2], &gv[3], &tv[3]);
                // 2/1 3/1 4/1 oder f 2/3/1 3/4/1 4/6/1
                // Found an / go further
            }
            else
            {
                status = sscanf(line.c_str(), "%i %i %i %i", &gv[0], &gv[1], &gv[2], &gv[3]);
                // f 1 2 3
                // No / found it must be 4 4 4 4 ....
            }
        }
    }

    //cout << "Status: " << status << endl;
    if(status == 3)
    {
        MyFace *f = new MyFace();

        for(int i = 0; i < status; i++)
        {
            f->pidx[i] = gv[i];
            m_VertexCount++;
        }

        f->hasTextureCoords = false;
        faces.push_back(f);
    }
    else if(status == 4)
    {
        MyFace *f1 = new MyFace();
        MyFace *f2 = new MyFace();

        f1->pidx[0] = gv[0];
        f1->pidx[1] = gv[1];
        f1->pidx[2] = gv[2];
        f2->pidx[0] = gv[0];
        f2->pidx[1] = gv[2];
        f2->pidx[2] = gv[3];

        m_VertexCount += 3*2;

        f1->hasTextureCoords = false;
        f2->hasTextureCoords = false;
        faces.push_back(f1);
        faces.push_back(f2);
    }
    else if(status == 6)
    {
        MyFace *f = new MyFace();

        for(int i = 0; i < status/2; i++)
        {
            f->pidx[i] = gv[i];
            f->tidx[i] = tv[i];
            m_VertexCount++;
        }
        faces.push_back(f);
    }
    else if(status == 8)
    {
        MyFace *f1 = new MyFace();
        MyFace *f2 = new MyFace();

        f1->pidx[0] = gv[0];
        f1->tidx[0] = tv[0];
        f1->pidx[1] = gv[1];
        f1->tidx[1] = tv[1];
        f1->pidx[2] = gv[2];
        f1->tidx[2] = tv[2];

        f2->pidx[0] = gv[0];
        f2->tidx[0] = tv[0];
        f2->pidx[1] = gv[2];
        f2->tidx[1] = tv[2];
        f2->pidx[2] = gv[3];
        f2->tidx[2] = tv[3];


        m_VertexCount += 3*2;

        faces.push_back(f1);
        faces.push_back(f2);
    }
    else
    {
        cout << "Status: " << status << " " << line << endl;
        exit(-1);
    }
}

void Model::createVertices()
{
    Vector normal;
    Vector a,b,c;

    //Initialwerte die definitiv zum Model gehören
    m_Box.Min = m_Box.Max = *my_vertices[1];

    m_pVertices = new Vertex[m_VertexCount*3];

    unsigned int PosIdx0, PosIdx1, PosIdx2, TexIdx0, TexIdx1, TexIdx2;

    for(int i = 0; i < faces.size(); i++)
    {
        PosIdx0 = faces.at(i)->pidx[0]-1;
        PosIdx1 = faces.at(i)->pidx[1]-1;
        PosIdx2 = faces.at(i)->pidx[2]-1;

        if(faces.at(i)->hasTextureCoords)
        {
            TexIdx0 = faces.at(i)->tidx[0]-1;
            TexIdx1 = faces.at(i)->tidx[1]-1;
            TexIdx2 = faces.at(i)->tidx[2]-1;
            m_pVertices[i*3].TexcoordS = my_texture_points[TexIdx0]->X;
            m_pVertices[i*3+1].TexcoordS = my_texture_points[TexIdx1]->X;
            m_pVertices[i*3+2].TexcoordS = my_texture_points[TexIdx2]->X;

            m_pVertices[i*3].TexcoordT = my_texture_points[TexIdx0]->Y;
            m_pVertices[i*3+1].TexcoordT = my_texture_points[TexIdx1]->Y;
            m_pVertices[i*3+2].TexcoordT = my_texture_points[TexIdx2]->Y;
        }

        a = m_pVertices[i*3].Position = *my_vertices[ PosIdx0];
        b = m_pVertices[i*3+1].Position = *my_vertices[ PosIdx1];
        c = m_pVertices[i*3+2].Position = *my_vertices[ PosIdx2];

        //Calculate boundingbox
        calcBounding(a,b,c);

        normal = (b-a).cross(c-a);
        normal.normalize();

        m_pVertices[i*3].Normal =
        m_pVertices[i*3+1].Normal =
        m_pVertices[i*3+2].Normal = normal;
    }
}

void Model::calcBounding(const Vector &a, const Vector &b, const Vector &c)
{
    if(m_Box.Max.X < a.X || m_Box.Max.X < b.X || m_Box.Max.X < c.X)
    {
        m_Box.Max.X = max({a.X,b.X,c.X});
    }
    else if(m_Box.Min.X > a.X || m_Box.Min.X > b.X || m_Box.Min.X > c.X)
    {
        m_Box.Min.X = min({a.X,b.X,c.X});
    }


    if(m_Box.Max.Y < a.Y || m_Box.Max.Y < b.Y || m_Box.Max.Y < c.Y)
    {
        m_Box.Max.Y = max({a.Y,b.Y,c.Y});
    }
    else if(m_Box.Min.Y > a.Y || m_Box.Min.Y > b.Y || m_Box.Min.Y > c.Y)
    {
        m_Box.Min.Y = min({a.Y,b.Y,c.Y});
    }


    if(m_Box.Max.Z < a.Z || m_Box.Max.Z < b.Z || m_Box.Max.Z < c.Z)
    {
        m_Box.Max.Z = max({a.Z,b.Z,c.Z});
    }
    else if(m_Box.Min.Z > a.Z || m_Box.Min.Z > b.Z || m_Box.Min.Z > c.Z)
    {
        m_Box.Min.Z = min({a.Z,b.Z,c.Z});
    }

}

void Model::calcSphere(){
    m_Sphere.Center = m_Sphere.BaseCenter = (m_Box.Min + m_Box.Max) * 0.5f;

    float radius = 0.0f;

    for(unsigned int i = 0; i < m_VertexCount; i++){
        float distance = (m_Sphere.Center - m_pVertices[i].Position).length();
        if(radius < distance){
            radius = distance;
        }
    }

    m_Sphere.Radius = radius;
}

void Model::drawSphere(){
    CheckGLErrors();

    glBegin(GL_LINES);
    for(float i = 0; i < 2 * M_PI; i += M_PI/12){
        glVertex3f(m_Sphere.Center.X + cos(i - M_PI/12) * m_Sphere.Radius, m_Sphere.Center.Y + sin(i - M_PI/12) * m_Sphere.Radius, m_Sphere.Center.Z);
        glVertex3f(m_Sphere.Center.X + cos(i) * m_Sphere.Radius, m_Sphere.Center.Y + sin(i) * m_Sphere.Radius, m_Sphere.Center.Z);
    }
    glEnd();

}

void Model::scale()
{
    float scale = 1.f;
    //Berechnen der Skalierung und skalieren 10.f / scale
    scale = 10.f / ((m_Box.Max - m_Box.Min).length());

    //cout << "Lenght BB : " << 1.f/((m_Box.Max - m_Box.Min).length()) << endl;

    for(unsigned int i = 0; i < m_VertexCount; i++){
        m_pVertices[i].Position = m_pVertices[i].Position * scale;
    }

    m_Box.Max = m_Box.Max * scale;
    m_Box.Min = m_Box.Min * scale;

    m_Sphere.Radius *= scale;
    m_Sphere.BaseCenter = m_Sphere.Center = m_Sphere.Center * scale;
}

void Model::convertToFloat(vector<float>& color, const vector<string>& splitted)
{
    float tmp;
    for(int i = 1; i < splitted.size(); i++){
        stringstream(splitted[i]) >> tmp;
        color.push_back(tmp);
    }
}

void CheckGLErrors()
{
    GLenum Error = glGetError();
    if(Error !=0)
    {
        std::cout << gluErrorString(Error) << std::endl;
        throw std::exception();
    }
}

