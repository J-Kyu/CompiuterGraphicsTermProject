#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h> 
#include <stdio.h> 
#include <iostream>
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <vector>
#include <time.h>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "LoadShaders.h"
#include "Satellite.h"

using namespace glm;
using namespace std;

class Camera{

	public:
	

		Camera(): eye(0,0,15),center(0,0,0),up(0,1,0),zoomFactor(1.0f), projectionMode(ORTHOGRAPHIC),zNear(0.01f),zFar(100.0f),fovy((float)(M_PI/180.0*(30.0f))),xRight(1.2f){};

		Camera(Satellite* satellite) : eye(0, 0, 15), center(0, 0, 0), up(0, 1, 0), zoomFactor(1.0f), projectionMode(ORTHOGRAPHIC), zNear(0.01f), zFar(100.0f), fovy((float)(M_PI / 180.0 * (30.0f))), xRight(1.2f) {
			this->satellite = satellite;
		};
		
		mat4 GetViewing();
		mat4 GetProjection(GLfloat);
		void Motion(int, int);
		void Mouse(int button, int state, int x, int y);
		void MouseWheel(int, int,int,int);
		void ViewSatellite(bool);


		int projectionMode;

	private:

		Satellite* satellite;
		bool isSatellite = false;


		enum { ORTHOGRAPHIC, PERSPECTIVE };
		vec3 eye;
		vec3 center;
		vec3 up;
		GLfloat zoomFactor;
		GLfloat zNear;
		GLfloat zFar;
		GLfloat fovy;
		GLfloat xRight;

		int button_pressed[3] = { GLUT_UP,GLUT_UP,GLUT_UP };
		int mouse_pos[2] = { 0,0 };


		mat4 Parallel(double r, double aspect, double n, double f);



};

