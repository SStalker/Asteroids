#include <iostream>
#include <math.h>
#include <unistd.h>
#ifdef WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/GLUT.h>
	#include <GL/GL.h>
#else
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif

#define GL_DEBUG

#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "global.h"

#include "Spaceship.h"


// Model that should be loaded
const char* g_ModelToLoad = "sponza/sponza.obj";

// window x and y size
const unsigned int g_WindowWidth=1024;
const unsigned int g_WindowHeight=768;

// light position (point light)
const Vector g_LightPos = Vector( 0,8,0);

float oldTime = 0;


Camera g_Camera;
Model g_Model;
Spaceship sp;

int g_MouseButton = 0;
int g_MouseState = 0;

void SetupDefaultGLSettings();
void DrawScene();
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void KeyboardCallback( unsigned char key, int x, int y);

enum RenderMode
{
    RENDERMODE_LINES,
    RENDERMODE_TRIANGLES,
    LAST_RENDERMODE
};

RenderMode g_RenderMode = RENDERMODE_LINES;

int main(int argc, char * argv[])
{
    // initialize the glut system and create a window
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("CG Praktikum");
#ifdef WIN32
	glewInit();
#endif
    
    SetupDefaultGLSettings();
    
    glutDisplayFunc(DrawScene);
    glutMouseFunc(MouseCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutMotionFunc(MouseMoveCallback);

    /*int option = 0;
    int area = -1, perimeter = -1;
    string  vertexShader = "", fragmentShader = "";

    //Specifying the expected options
    //The two options l and b expect numbers as argument
    while ((option = getopt(argc, argv,"f:v:")) != -1) {
        switch (option) {
             case 'f' : fragmentShader = optarg; 
                 break;
             case 'v' : vertexShader = optarg;
                 break;
             default: printf("Usage: %s –f beispiel_fragmentshader.glsl –v beispiel_vertexshader.glsl\n", argv[0]);
                 exit(EXIT_FAILURE);
        }
    }
    if (vertexShader == "" && fragmentShader == "") {
        printf("Usage: %s –f beispiel_fragmentshader.glsl –v beispiel_vertexshader.glsl\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    cout << "Current params: " << endl;
    cout << "VertexShader: " << vertexShader << endl;
    cout << "FragmentShader: " << fragmentShader << endl;*/

    //g_Model.load(g_ModelToLoad, vertexShader.c_str(), fragmentShader.c_str());
    //sp.load("assets/model/SpaceShip.obj", "assets/shader/ToonVertexShader.glsl", "assets/shader/ToonFragmentShader.glsl");
    if(!sp.load("assets/model/SpaceShip2.obj", "assets/shader/ToonVertexShader.glsl", "assets/shader/ToonFragmentShader.glsl", Vector(0,0,0))){
        cout << "Could not load model";
        exit(6);
    }

    glutMainLoop();    
}


void SetupDefaultGLSettings()
{
    glClearColor(255, 255, 255, 255);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (double)g_WindowWidth/(double)g_WindowHeight, 0.045f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    // Setup Light Color
    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Setup default material
    float diff[4] = {1,1,1,1};
    float amb[4]  = {0.2f,0.2f,0.2f,1};
    float spec[4] = {0.5f,0.5f,0.5f,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMateriali(GL_FRONT, GL_SHININESS, 30);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

void DrawGroundGrid()
{
    const float GridSize=10.0f;
    const unsigned int GridSegments=20;
    const float GridStep=GridSize/(float)GridSegments;
    const float GridOrigin=-GridSize*0.5f;
    
    glDisable( GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for( unsigned int i=0; i<GridSegments+1; i++)
        {
            float itpos=GridOrigin + GridStep*(float)i;
            glVertex3f(itpos, 0, GridOrigin);
            glVertex3f(itpos, 0, GridOrigin+GridSize);
        
            glVertex3f(GridOrigin, 0, itpos);
            glVertex3f(GridOrigin+GridSize, 0, itpos);

        }
    glEnd();
    glEnable( GL_LIGHTING);

}

void MouseCallback(int Button, int State, int x, int y)
{
    g_MouseButton = Button;
    g_MouseState = State;
    g_Camera.mouseInput(x,y,Button,State);
}

void MouseMoveCallback( int x, int y)
{
    g_Camera.mouseInput(x,y,g_MouseButton,g_MouseState);
}

void KeyboardCallback( unsigned char key, int x, int y)
{
    if( key == 'l')
        g_RenderMode=RENDERMODE_LINES;
    else if( key == 't')
        g_RenderMode=RENDERMODE_TRIANGLES;
    
}


void DrawScene()
{
    float newtime = glutGet(GLUT_ELAPSED_TIME)*1.0 ;
    float deltaTime = (newtime-oldTime)/1000.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    Vector currentPos = sp.getPos();
    g_Camera.setPosition(Vector(currentPos.X, currentPos.Y+10, currentPos.Z-15));
    g_Camera.setTarget(Vector(currentPos));
    g_Camera.apply();
    
    DrawGroundGrid();
    
    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);

    /*if(g_RenderMode == RENDERMODE_LINES)
    {
        glDisable(GL_LIGHTING);
        sp.drawLines();
        glEnable(GL_LIGHTING);
    }
    else if(g_RenderMode== RENDERMODE_TRIANGLES)
        sp.drawTriangles();
    */
    sp.update(deltaTime);
    sp.draw();
    glutSwapBuffers();
    glutPostRedisplay();
    
}



/*
#include <GL/gl.h>
#include <GL/glut.h>
#include <unistd.h>

#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"

// Model that should be loaded
const char* g_ModelToLoad = "sponza/sponza.obj";

// window x and y size
const unsigned int g_WindowWidth=1024;
const unsigned int g_WindowHeight=768;

// light position (point light)
const Vector g_LightPos = Vector( 0,8,0);

enum RenderMode
{
    RENDERMODE_LINES,
    RENDERMODE_TRIANGLES,
    LAST_RENDERMODE
};

RenderMode g_RenderMode = RENDERMODE_LINES;


Camera g_Camera;
Model g_Model;
int g_MouseButton = 0;
int g_MouseState = 0;

void DrawScene();
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void KeyboardCallback( unsigned char key, int x, int y);

int main(int argc, char** argv){

    int option = 0;
    int area = -1, perimeter = -1;
    string  vertexShader = "", fragmentShader = "";

    //Specifying the expected options
    //The two options l and b expect numbers as argument
    while ((option = getopt(argc, argv,"f:v:")) != -1) {
        switch (option) {
             case 'f' : fragmentShader = optarg; 
                 break;
             case 'v' : vertexShader = optarg;
                 break;
             default: printf("Usage: %s –f beispiel_fragmentshader.glsl –v beispiel_vertexshader.glsl\n", argv[0]);
                 exit(EXIT_FAILURE);
        }
    }
    if (vertexShader == "" && fragmentShader == "") {
        printf("Usage: %s –f beispiel_fragmentshader.glsl –v beispiel_vertexshader.glsl\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    cout << "Current params: " << endl;
    cout << "VertexShader: " << vertexShader << endl;
    cout << "FragmentShader: " << fragmentShader << endl;

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutCreateWindow("Matrix Fractal");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(0.0,0.0,(GLfloat) 500, (GLfloat) 500);

    //glutDisplayFunc(draw);
    //glutReshapeFunc(reshape);

    //initShader("fsFractal.glsl");

    //glutMainLoop();
    //GLenum err = glewInit();
    //
    // initialize the glut system and create a window
    g_Model.load(g_ModelToLoad, "ToonVertexShader.glsl", "ToonFragmentShader.glsl");
    
    
    glutDisplayFunc(DrawScene);
    glutMouseFunc(MouseCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutMotionFunc(MouseMoveCallback);

  	
	ShaderProgram sp;

	bool hasLoaded = sp.load(vertexShader.c_str(), fragmentShader.c_str());

	if(hasLoaded)
		sp.compile(nullptr);
	
	glutMainLoop();
	return 0;
}

void MouseCallback(int Button, int State, int x, int y)
{
    g_MouseButton = Button;
    g_MouseState = State;
    g_Camera.mouseInput(x,y,Button,State);
}

void MouseMoveCallback( int x, int y)
{
    g_Camera.mouseInput(x,y,g_MouseButton,g_MouseState);
}

void KeyboardCallback( unsigned char key, int x, int y)
{
    if( key == 'l')
        g_RenderMode=RENDERMODE_LINES;
    else if( key == 't')
        g_RenderMode=RENDERMODE_TRIANGLES;
    
}


void DrawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    g_Camera.apply();
   
    
    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);

    if(g_RenderMode == RENDERMODE_LINES)
    {
        glDisable(GL_LIGHTING);
        g_Model.drawLines();
        glEnable(GL_LIGHTING);
    }
    else if(g_RenderMode== RENDERMODE_TRIANGLES)
        g_Model.drawTriangles();
    
    glutSwapBuffers();
    glutPostRedisplay();
    
}*/
