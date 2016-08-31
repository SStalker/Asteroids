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

class Debug{
public:
    static void drawLine(const Vector a, const Vector b, const Color c){

        //cout << "Color: " << c << endl;
        glLineWidth(3.5);
        glColor3f(c.R, c.G, c.B);
        glBegin(GL_LINES);
        glVertex3f(a.X, a.Y, a.Z);
        glVertex3f(b.X, b.Y, b.Z);
        glEnd();
    }

    static void Drawmatrix(const Matrix& m)
    {
        drawLine(m.translation(), m.translation() + m.right()*10, Color(1,0,0));
        drawLine(m.translation(), m.translation() + m.up()*10, Color(0,1,0));
        drawLine(m.translation(), m.translation() + m.forward()*10, Color(0,0,1));
    }
};


// Model that should be loaded
const char* g_ModelToLoad = "sponza/sponza.obj";

// window x and y size
const unsigned int g_WindowWidth=1024;
const unsigned int g_WindowHeight=768;

int centerX = (float)g_WindowWidth / 2.0;
int centerY = (float)g_WindowHeight / 2.0;

// light position (point light)
const Vector g_LightPos = Vector( 0,8,0);

float oldTime = 0;


Camera g_Camera;
Model g_Model;
Spaceship sp;

int g_MouseButton = 0;
int g_MouseState = 0;


float g_forward = 0;
float g_right = 0;

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
		// making the window full screen
		glutFullScreen();
		glutSetCursor(GLUT_CURSOR_NONE);
#ifdef WIN32
	glewInit();
#endif

    SetupDefaultGLSettings();

    glutDisplayFunc(DrawScene);
    glutMouseFunc(MouseCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutPassiveMotionFunc(MouseMoveCallback);


    //sp.load("assets/model/SpaceShip.obj", "assets/shader/ToonVertexShader.glsl", "assets/shader/ToonFragmentShader.glsl");
    if(!sp.load("assets/model/SpaceShip.obj", "assets/shader/PhongVertexShader.glsl", "assets/shader/PhongFragmentShader.glsl")){
        cout << "Could not load model";
        exit(6);
    }

		sp.setPos(Vector(0.f,0.f,0.f));

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
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 300);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

void DrawGroundGrid()
{
    const float GridSize=1000.0f;
    const unsigned int GridSegments=2000;
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

    int mouseX = x-centerX;
    int mouseY = y-centerY;

    if(mouseX == 0 && mouseY == 0)
        return;

    g_right = clamp((float)mouseX,-1.f,1.f);
    g_forward = clamp((float)mouseY,-1.f,1.f);
    //cout << "x: " <<  << " Y: " <<  << endl;

    sp.steer((-1.f)*g_forward,g_right);

    // Set the cursor back to center;
    glutWarpPointer( centerX, centerY );

//    sp.steer(g_forward, g_right);
    sp.MoveUpInput(g_forward);
    sp.MoveRightInput(-1*g_right);
}

void KeyboardCallback( unsigned char key, int x, int y)
{
	int mod = glutGetModifiers();
    if( key == 'w')
        sp.ThrustInput(1.f);
}


void DrawScene()
{
    glLoadIdentity();
    float newtime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (newtime-oldTime)/1000.0;
    oldTime = newtime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);


    sp.setDeltaTime(deltaTime);
    sp.update(deltaTime);

    Matrix cPos = sp.getPosition();
    Matrix cRot = sp.getRotation();
    Matrix combined = cPos*cRot;

//    g_Camera.setPosition(combined.forward()*-3.f + combined.up()*4.f + combined.translation());
//    g_Camera.setTarget(combined.translation() + combined.forward()*4.f);
//    g_Camera.apply();

    DrawGroundGrid();
    sp.draw();
        Debug::Drawmatrix(combined);

    glutSwapBuffers();
    glutPostRedisplay();

}
