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

#include "drawtext.h"

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

// window x and y size
int g_WindowWidth= 1920;//1024;
int g_WindowHeight= 1080;//768;

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

const float fovy = 65.f;
const float aspectRatio = (float)((double)g_WindowWidth/(double)g_WindowHeight);
const float nPlane = 0.045f;
const float fPlane = 1000.f;

int g_MouseButton = 0;
int g_MouseState = 0;

int windowid;

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

struct dtx_font *font;

int main(int argc, char * argv[])
{
    // initialize the glut system and create a window
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    windowid = glutCreateWindow("CG Praktikum");
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
    sp->setPos(Vector(0.f,0.f,-220.f));
    cd = new CollisionDetection(game->getProjectileList(), game->getAsteroidList(), game->getPlanetList(), game->getSpaceship());

    SoundManager::getInstance()->init();
    SoundManager::getInstance()->playBackgroundMusic();

				if(!(font = dtx_open_font("fonts/nasalization/nasalization.ttf", 24))) {
						fprintf(stderr, "failed to open font\n");
						return 1;
					}

					dtx_use_font(font, 24);

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
    gluPerspective(fovy, aspectRatio, nPlane, fPlane);

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

    sp->steer(g_forward, g_right);

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
    case 27:
        glutDestroyWindow ( windowid );
        exit (0);
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
	  glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float newtime = glutGet(GLUT_ELAPSED_TIME);
		float deltaTime = (newtime-oldTime)/1000.0;
		oldTime = newtime;


		sp->setDeltaTime(deltaTime);
		sp->update(deltaTime);


		GLfloat lpos[4];
		lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
		glLightfv(GL_LIGHT0, GL_POSITION, lpos);

		sp->draw();
		sp->updateBounding();

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
				}
		}

		// Draw the earth
		for(auto& planet : *Game::getInstance()->getPlanetList())
		{
				planet->update(deltaTime);
				planet->updateBounding();
				planet->draw();
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

		// Check for collisions
		cd->react();

		// Draw the skybox
		game->drawSkybox();

		// Draw the 2d stuff here
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, g_WindowWidth,0, g_WindowHeight);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glClear(GL_DEPTH_BUFFER_BIT);

	// Hud schriften hier!!!!
	glPushMatrix();
		glTranslatef(0, g_WindowHeight-50, 0);
		glColor3f(1, 1, 1);

		string leftAsteroids = "Left Asteroids: " + to_string(Game::getInstance()->getAsteroidList()->size());
		dtx_string(leftAsteroids.c_str());
		glPopMatrix();


		glPushMatrix();
		glTranslatef(0, g_WindowHeight-75, 0);

		string leftHealthOfEarth = "Earth Health: " + to_string((int)Game::getInstance()->getPlanetList()->at(0)->getHealth());
		dtx_string(leftHealthOfEarth.c_str());
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 50, 0);

		string leftHealthOfPlayer = "Players Health: " + to_string((int)sp->getHealth());
		dtx_string(leftHealthOfPlayer.c_str());
		glPopMatrix();

		glPushMatrix();
		glTranslatef(g_WindowWidth-325, g_WindowHeight-50, 0);

		string creators = "Proudly made by\nLukas Hannigbrinck & \nRaphael Grewe";
		dtx_string(creators.c_str());
		glPopMatrix();

		if(sp->isDead() || Game::getInstance()->getPlanetList()->at(0)->isDead())
		{
			glPushMatrix();
			dtx_prepare(font, 150);
			dtx_use_font(font, 150);
			glTranslatef(g_WindowWidth/2-150, g_WindowHeight/2+150, 0);

			string lost = " You\n LOST \n;-)";
			dtx_string(lost.c_str());
			dtx_prepare(font, 24);
			dtx_use_font(font, 24);
			glPopMatrix();
		}

		drawCrosshair();

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glutSwapBuffers();
	  glutPostRedisplay();
}

void drawCrosshair()
{
    glPushMatrix();
    glLoadIdentity();

    glColor3ub(240, 240, 240);//white
    glLineWidth(2.0);

    int crossHair[8] =
    {
    g_WindowWidth / 2 - 7, g_WindowHeight / 2 +150, // horizontal line
    g_WindowWidth / 2 + 7, g_WindowHeight / 2 +150,

    g_WindowWidth / 2 , g_WindowHeight / 2 + 157, //vertical line
    g_WindowWidth / 2 , g_WindowHeight / 2 + 143
    };

    // activate vertext array state and assign pointer to vertext array data
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_INT, 0, crossHair);

    //draw primitive GL_LINES starting at the first vertex, use 2 total vertices
    glDrawArrays(GL_LINES, 0, 2); //draw horizontal line
    //Same as above but start at second vertex
    glDrawArrays(GL_LINES, 2, 2); //draw vertical line

    // deactivate vertex array state after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();


}
