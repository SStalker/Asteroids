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
#include "Game.h"
#include "CollisionDetection.h"
#include "SoundManager.h"

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
int g_WindowWidth=1024;
int g_WindowHeight=768;

int centerX = (float)g_WindowWidth / 2.0;
int centerY = (float)g_WindowHeight / 2.0;

// light position (point light)
const Vector g_LightPos = Vector( 0,8,0);

float oldTime = 0;


Game *game = Game::getInstance();
CollisionDetection *cd;
Camera g_Camera;
Model g_Model;
Spaceship *sp;

int g_MouseButton = 0;
int g_MouseState = 0;


float g_forward = 0;
float g_right = 0;

void SetupDefaultGLSettings();
void DrawScene();
void MouseCallback(int Button, int State, int x, int y);
void MousePassiveMoveCallback(int x, int y);
void KeyboardCallback( unsigned char key, int x, int y);
void MouseMoveCallback(int x, int y);
void Resize(int width, int height);
void drawCrosshair();

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
    glutPassiveMotionFunc(MousePassiveMoveCallback);
    glutMotionFunc(MouseMoveCallback);
		glutReshapeFunc(Resize);

		game->init();
		sp = game->getSpaceship();
		sp->setPos(Vector(0.f,0.f,-50.f));
		cd = new CollisionDetection(game->getProjectileList(), game->getAsteroidList(), game->getPlanetList(), game->getSpaceship());

		SoundManager::getInstance()->init();
		SoundManager::getInstance()->playBackgroundMusic();

    glutMainLoop();
}


void SetupDefaultGLSettings()
{
    glClearColor(0, 0, 0, 255);
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
    const float GridSize=10.0f;
    const unsigned int GridSegments=10;
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

void mouseMove(int x, int y)
{
    int mouseX = x-centerX;
    int mouseY = y-centerY;

    if(mouseX == 0 && mouseY == 0)
        return;

    g_right = clamp((float)mouseX,-1.f,1.f);
    g_forward = clamp((float)mouseY,-1.f,1.f);
    //cout << "x: " <<  << " Y: " <<  << endl;

    sp->steer((-1.f)*g_forward,g_right);

    // Set the cursor back to center;
    glutWarpPointer( centerX, centerY );
}

void MouseCallback(int Button, int State, int x, int y)
{
    if(Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
        sp->fire();
    }
}

void MousePassiveMoveCallback( int x, int y)
{
    mouseMove(x,y);
}

void MouseMoveCallback(int x, int y)
{
    mouseMove(x,y);
}

void KeyboardCallback( unsigned char key, int x, int y)
{
    int mod = glutGetModifiers();
    switch (key) {
    case 'w': sp->ThrustInput(1.f);
        break;
    case 's': sp->ThrustInput(-1.f);
        break;
    default:
        break;
    }
}

void Resize(int width, int height)
{
	glViewport(0, 0, (GLint)width, (GLint)height);
  g_WindowWidth = width;
  g_WindowHeight = height;
}

void DrawScene()
{
		//drawCrosshair();
    glLoadIdentity();
    float newtime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (newtime-oldTime)/1000.0;
    oldTime = newtime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sp->setDeltaTime(deltaTime);
    sp->update(deltaTime);

    //DrawGroundGrid();

    game->drawSkybox();

    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);

    sp->draw();
		sp->updateBounding();
		sp->drawSphere();

    // Draw every asteroid
		vector<Asteroid*> *alist = Game::getInstance()->getAsteroidList();

		for(int i = 0; i < alist->size(); i++)
		{
				if((*alist)[i]->isDead())
				{
					delete (*alist)[i];
					alist->erase(alist->begin()+i);
				}else{
					(*alist)[i]->update(deltaTime);
					(*alist)[i]->updateBounding();
					(*alist)[i]->draw();
					//(*alist)[i]->drawSphere();
				}
		}

    // Draw the earth
    for(auto& planet : *Game::getInstance()->getPlanetList())
    {
        planet->update(deltaTime);
        planet->updateBounding();
        planet->draw();
        planet->drawSphere();
        //planet->drawBounding();

        //Debug::Drawmatrix(planet->getPosition());
    }

    // Draw the projectiles
    vector<Projectile*> *plist = Game::getInstance()->getProjectileList();

    for(int i = 0; i < plist->size(); i++)
    {
        if((*plist)[i]->isDead())
        {
            delete (*plist)[i];
            plist->erase(plist->begin()+i);
        }else{
            (*plist)[i]->update(deltaTime);
            (*plist)[i]->updateBounding();
            (*plist)[i]->draw();
        }
    }



		cd->react();

		if(sp->isDead())
			exit(42);

    glutSwapBuffers();
    glutPostRedisplay();

}

void drawCrosshair()
{
    glPushMatrix();
    glViewport(0, 0, g_WindowWidth, g_WindowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, g_WindowWidth, g_WindowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3ub(240, 240, 240);//white
    glLineWidth(2.0);

    int crossHair[8] =
    {
    g_WindowWidth / 2 - 7, g_WindowHeight / 2, // horizontal line
    g_WindowWidth / 2 + 7, g_WindowHeight / 2,

    g_WindowWidth / 2, g_WindowHeight / 2 + 7, //vertical line
    g_WindowWidth / 2, g_WindowHeight / 2 - 7
    };

    // activate vertext array state and assign pointer to vertext array data
    glEnableClientState(GL_VERTEX_ARRAY);

    //2 = number of coordinates per vertext we are doing 2d so I don't need the Z coordinate
    // GL_INT = data type held in array
    // crossHair = pointer to vertext data array


    glVertexPointer(2, GL_INT, 0, crossHair);

    //draw primitive GL_LINES starting at the first vertex, use 2 total vertices
    glDrawArrays(GL_LINES, 0, 2); //draw horizontal line
    //Same as above but start at second vertex
    glDrawArrays(GL_LINES, 2, 2); //draw vertical line

    // deactivate vertex array state after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();


}
