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

using namespace std;
using namespace glm;

EmptyObject* earthObj;
EmptyObject* moonObj;
EmptyObject* sphereObj;
EmptyObject* cubeObj;;

Component* earthGraphic;
Component* moonGraphic;
Component* sphere;
Component* cubeGraphic;

Rigidbody3D* sphere_1RigidBody;
Rigidbody3D* sphere_2RigidBody;



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
bool RigidBodyWorld::pause;

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
	//ODEInit();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMouseWheelFunc(mouseWheel);
	glutSpecialFunc(keyboardSpecial);
	glutMainLoop();

}

void init() {
	//graphic
	sphere = new Sphere(.5f,16,16);
	cubeGraphic = new Cube();
	earthGraphic = new Graphic();
	moonGraphic = new Graphic();

	earthGraphic->kyu = 1;
	moonGraphic->kyu = 2;
	sphere->kyu = 3;


	//rigid body

	sphere_1RigidBody = new Rigidbody3D();
	sphere_1RigidBody->SphereRigidBodyInit();

	sphere_2RigidBody = new Rigidbody3D();
	sphere_2RigidBody->SphereRigidBodyInit();


	earthObj = new EmptyObject(earthGraphic);
	moonObj = new EmptyObject(moonGraphic);
	sphereObj = new EmptyObject(sphere);
	cubeObj = new EmptyObject(cubeGraphic);

	earthObj->AddComponent(sphere_1RigidBody);
	moonObj->AddComponent(sphere_2RigidBody);


	sphereObj->MoveObject(vec3(1.0f, 2.0f, 0.0f));
	earthObj->MoveObject(vec3(.50f, 1.0f, 0.0f));

	//earthObj->AddChildren(moonObj);

	attrib_t attrib_1;

	earthGraphic->LoadObj("models/earth.obj", "models/", attrib_1, 1.0f);
	glActiveTexture(GL_TEXTURE0);
	earthGraphic->LoadTexture("models/", attrib_1.texcoords);

	attrib_t attrib_2;
	moonGraphic->LoadObj("models/moon.obj", "models/", attrib_2, .3f);
	glActiveTexture(GL_TEXTURE0);
	moonGraphic->LoadTexture("models/", attrib_2.texcoords);

	earthObj->Init();
	moonObj->Init();
	sphereObj->Init();
	cubeObj->Init();
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

	//earthObj->RotatingYAxis(1.0f);
	//moonObj->RotatingYAxis(-3.0f);

	earthObj->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	earthObj->SetViewMatrix(mainCamera->GetViewing());

	moonObj->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	moonObj->SetViewMatrix(mainCamera->GetViewing());

	sphereObj->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	sphereObj->SetViewMatrix(mainCamera->GetViewing());

	cubeObj->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	cubeObj->SetViewMatrix(mainCamera->GetViewing());

	earthObj->Activate(color_mode);
	moonObj->Activate(color_mode);
	sphereObj->Activate(color_mode);
	cubeObj->Activate(color_mode);


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


		break;
	}
	case GLUT_KEY_DOWN: {

		break;
	}
	case GLUT_KEY_RIGHT: {

		break;
	}
	case GLUT_KEY_LEFT: {

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