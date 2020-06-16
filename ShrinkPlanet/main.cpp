//#pragma commnet(lib,"winmm.lib");


#include <iostream>
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <GL/freeglut.h>
#include <vector>
#include <time.h>
#include "EmptyObject.h"
#include "Sphere.h"
#include "Camera.h"
#include "glm/gtx/string_cast.hpp"
#include "Cube.h"
#include <cmath>
#include "Rigidbody3D.h"
#include "RigidBodyWorld.h"
#include "Satellite.h"
#include "GameSystem.h"
#include "UI.h"

#include "GravityAttractor.h"
#include "GravityDependent.h"

#include "Menu.h"


using namespace std;
using namespace glm;



void init();
void display();
void Render(int color_mode = 0);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void mouseWheel(int wheel, int dir, int x, int y);


void keyboardSpecial(int key, int x, int y);

enum State { MENU, PLAYING, DONE };

dWorldID RigidBodyWorld::ode_world;
dSpaceID RigidBodyWorld::ode_space;
dJointGroupID RigidBodyWorld::ode_contactgroup;
bool RigidBodyWorld::pause = false;

GameSystem* GameSystem::instance = nullptr;
UI* UI::instance = nullptr;


//dGeomID RigidBodyWorld::ode_plane_geom;


GravityAttractor* earth;
Satellite* satellite;
GravityDependent* characters;

Camera* mainCamera;
EmptyObject* bg = new EmptyObject();
EmptyObject* endCredits = new EmptyObject();
Menu* menu;

char g_szMsg[100];

void main(int argc, char** argv)
{

	//Display Setting
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutCreateWindow("VAO example");

	//load glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMouseWheelFunc(mouseWheel);
	glutSpecialFunc(keyboardSpecial);
	glutMainLoop();

}

void init() {

	GameSystem::GetInstance()->SetState(GameSystem::MENU);

	GravityDependent* character;

	RigidBodyWorld::WorldInit();

	//backgroudn
	attrib_t attrib;
	bg->graphic = new Graphic();
	bg->graphic->LoadObj("models/universe.obj", "models/", attrib, 100.0f);
	glActiveTexture(GL_TEXTURE0);
	bg->graphic->LoadTexture("models/", attrib.texcoords);
	bg->graphic->objectCode = 0;
	bg->Init();

	//endCredits
	endCredits->graphic = new Graphic();
	endCredits->graphic->LoadObj("models/done.obj", "models/", attrib, 5.0f);
	glActiveTexture(GL_TEXTURE0);
	endCredits->graphic->LoadTexture("models/", attrib.texcoords);
	endCredits->graphic->objectCode = 102;
	endCredits->Init();
	endCredits->MoveObject(vec3(0, 0, 20));

	//Menu

	menu = new Menu();
	
	earth = new GravityAttractor("models/earth.obj", "models/", 10.0f,5.0f,20.0f);

	satellite = new Satellite(earth->mainEntity,15.0f,0.0f,4.0f,0.0f);

	characters = new GravityDependent(earth->mainEntity, "models/Crate.obj", "models/", 2.0f, .5f, 5.0f, 0.0f, 11.0f, 0.0f);

	mainCamera = new Camera(satellite);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}







void Render(int color_mode) {

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	double aspect = 1.0 * width / height;

	bg->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	bg->SetViewMatrix(mainCamera->GetViewing());

	endCredits->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	endCredits->SetViewMatrix(mainCamera->GetViewing());


	switch (GameSystem::GetInstance()->GetState()) {
	case GameSystem::MENU: {
		menu->Activate(mainCamera->GetProjection(aspect), mainCamera->GetViewing(), color_mode);
		break;
	}
	case GameSystem::PLAYING: {
		bg->Activate(color_mode);
		earth->Activate(mainCamera->GetProjection(aspect), mainCamera->GetViewing(), color_mode);
		satellite->Activate(mainCamera->GetProjection(aspect), mainCamera->GetViewing(), color_mode);
		characters->Activate(mainCamera->GetProjection(aspect), mainCamera->GetViewing(), color_mode);

		UI::GetInstance()->Activate(mainCamera->GetProjection(aspect), mainCamera->GetViewing(), color_mode, mainCamera->GetEye(), mainCamera->GetUp());
		
		break;
	}
	case GameSystem::DONE:{
		mainCamera->ViewSatellite(false);
		UI::GetInstance()->IsSatelliteOn(false);
		//end credit
		endCredits->RotatingYAxis(-1.0f);
		endCredits->Activate(color_mode);
		mainCamera->ResetEye();
		UI::GetInstance()->Activate(mainCamera->GetProjection(aspect), mainCamera->GetViewing(), color_mode, mainCamera->GetEye(), mainCamera->GetUp());

		break;
	}
	default: {

		break;
	}

	}

	if (color_mode != 2) {
		glutSwapBuffers();
	}
}


void display() {
	Render();
	glColor3f(1.0, 1.0, 1.0);
	glFlush();
	glutPostRedisplay();

}

void mouse(int button, int state, int x, int y) {
	mainCamera->Mouse(button, state, x, y);
	Render(2);
	glFlush();

	if (state == GLUT_UP)
	{
		unsigned char res[4];
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		glReadPixels(x, height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
		switch (res[0])
		{
		case 1: printf("The earth is clicked!\n"); break;
		case 2: printf("The moon is clicked!\n"); break;
		case 100: {
			printf("Start\n"); 
			GameSystem::GetInstance()->SetState(GameSystem::PLAYING);
			break;
		}		
		case 101: {
			printf("Exit\n"); 
			exit(0);
			break;
		}
		case 102: {
			printf("To Mars\n");
			exit(0);
			break;
		}
		}
	}
}

void motion(int x, int y) {
	mainCamera->Motion(x, y);
}


void mouseWheel(int wheel, int dir, int x, int y) {
	mainCamera->MouseWheel(wheel, dir, x, y);
}



void keyboardSpecial(int key, int x, int y) {

	GLfloat radian = M_PI / 180;

	switch (key) {
	case GLUT_KEY_UP: {
		if (GameSystem::GetInstance()->GetState() != GameSystem::PLAYING) {
			break;
		}
		vec3 pos = satellite->GetPos();
		characters->GenerateBlock(pos);
		satellite->IncreaseRadius(2.0f);
		break;
	}
	case GLUT_KEY_DOWN: {

		mainCamera->ResetEye();
		GameSystem::GetInstance()->SetState(GameSystem::MENU);
		break;
	}
	case GLUT_KEY_RIGHT: {

		break;
	}
	case GLUT_KEY_LEFT: {

		break;
	}
	case GLUT_KEY_HOME: {
		mainCamera->ViewSatellite(true);
		UI::GetInstance()->IsSatelliteOn(true);
		break;
	}
	case GLUT_KEY_INSERT: {
		mainCamera->ViewSatellite(false);
		UI::GetInstance()->IsSatelliteOn(false);
		break;
	}
	}


}