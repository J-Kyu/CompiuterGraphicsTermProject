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




#include "GravityAttractor.h"
#include "GravityDependent.h"



using namespace std;
using namespace glm;

GravityAttractor* earth;

vector< GravityDependent*> characters;

Camera* mainCamera = new Camera();


void init();
void display();
void Render(int color_mode = 0);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void mouseWheel(int wheel, int dir, int x, int y);


void keyboardSpecial(int key, int x, int y);

dWorldID RigidBodyWorld::ode_world;
dSpaceID RigidBodyWorld::ode_space;
dJointGroupID RigidBodyWorld::ode_contactgroup;
bool RigidBodyWorld::pause = false;

//dGeomID RigidBodyWorld::ode_plane_geom;


void main(int argc, char** argv)
{

	//Display Setting
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGBA);
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
	GravityDependent* character;

	RigidBodyWorld::WorldInit();
	
	
	earth = new GravityAttractor("models/earth.obj", "models/", 5.0f,2.5f,20.0f);

	
	characters.push_back(new GravityDependent(earth->mainEntity, "models/moon.obj", "models/", 0.2f, 0.1f, 10.0f, 0.0f, 7.0f, 0.0f));
	characters.push_back(new GravityDependent(earth->mainEntity, "models/moon.obj", "models/", 0.2f, 0.1f, 10.0f, 7.0f, 0.0f, 0.0f));
	characters.push_back(new GravityDependent(earth->mainEntity, "models/moon.obj", "models/", 0.2f, 0.1f, 10.0f, 0.0f, 0.0f, 7.0f));

	characters.push_back(new GravityDependent(earth->mainEntity, "models/moon.obj", "models/", 0.2f, 0.1f, 10.0f, 0.0f, -7.0f, 0.0f));
	characters.push_back(new GravityDependent(earth->mainEntity, "models/moon.obj", "models/", 0.2f, 0.1f, 10.0f, -7.0f, 0.0f, 0.0f));
	characters.push_back(new GravityDependent(earth->mainEntity, "models/moon.obj", "models/", 0.2f, 0.1f, 10.0f, 0.0f, 0.0f, -7.0f));




	//character = new GravityDependent(earth->mainEntity, "models/moon.obj", "models/", 0.2f,0.1f,10.0f);
	//character->mainEntity->MoveObject(vec3(.0f, 3.0f, 0.0f));

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



	earth->Activate(mainCamera->GetProjection(aspect), mainCamera->GetViewing(),color_mode);

	for (int i = 0; i < characters.size(); i++) {
		characters[i]->Activate(mainCamera->GetProjection(aspect), mainCamera->GetViewing(), color_mode);
	}
	//character->Activate(mainCamera->GetProjection(aspect), mainCamera->GetViewing(), color_mode);

	if (color_mode != 2) {
		glutSwapBuffers();
	}
}


void display() {
	Render();

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

		//character->MoveDamObject(15);
		break;
	}
	case GLUT_KEY_DOWN: {
		//character->MoveDamObject(-15);
		break;
	}
	case GLUT_KEY_RIGHT: {
		//character->mainEntity->rigidbody->RotateRigidBody(50, vec3(0.0f, -1.0f, 0.0f));
		//character->mainEntity->RotateObject(10, vec3(0.0f, 1.0f, 0.0f));
		break;
	}
	case GLUT_KEY_LEFT: {
		//character->mainEntity->rigidbody->RotateRigidBody(50, vec3(0.0f, 1.0f, 0.0f));
		//character->mainEntity->RotateObject(10, vec3(0.0f, -1.0f, 0.0f));
		break;
	}
	case GLUT_KEY_HOME: {
	
		break;
	}
	case GLUT_KEY_INSERT: {

		break;
	}
	}


}