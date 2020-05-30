#include <iostream>
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <GL/freeglut.h>
#include <vector>
#include <time.h>
#include "EmptyObject.h"
#include "Sphere.h"
#include "Camera.h"

using namespace std;
using namespace glm;


EmptyObject* earthObj;
EmptyObject* moonObj;

Camera* mainCamera = new Camera();

void init();
void display();
void Render(int color_mode = 0);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void mouseWheel(int wheel, int dir, int x, int y);





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
	glutMainLoop();

}

void init() {

	//Component* earthGraphic = new Sphere(0.3f, 16, 16);
	//Component* ufo = new Sphere(2.0f, 16, 16);
	Component* earthGraphic = new Graphic();
	Component* moonGraphic = new Graphic();

	earthGraphic->kyu = 1;
	moonGraphic->kyu = 2;

	earthObj = new EmptyObject(earthGraphic);
	moonObj = new EmptyObject(moonGraphic);


	moonObj->MoveObject(vec3(1.0f, 0.0f, 0.0f));




	earthObj->AddChildren(moonObj);



	attrib_t attrib_1;

	earthGraphic->LoadObj("models/earth.obj", "models/", attrib_1, 1.0f);
	glActiveTexture(GL_TEXTURE0);
	earthGraphic->LoadTexture("models/", attrib_1.texcoords);

	attrib_t attrib_2;
	moonGraphic->LoadObj("models/moon.obj", "models/", attrib_2, .3f);
	glActiveTexture(GL_TEXTURE0);
	moonGraphic->LoadTexture("models/", attrib_2.texcoords);

	earthObj->Init();

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


	earthObj->RotatingYAxis(1.0f);
	moonObj->RotatingYAxis(-3.0f);

	earthObj->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	earthObj->SetViewMatrix(mainCamera->GetViewing());

	earthObj->Activate(color_mode);


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
