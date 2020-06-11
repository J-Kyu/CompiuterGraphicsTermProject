#pragma once

#include "EmptyObject.h"
#define _USE_MATH_DEFINES
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <GL/freeglut.h>
#include <vector>
#include "glm/gtx/string_cast.hpp"
#include "Component.h"
//#include "Graphic.h"
#include <string>
#include "Rigidbody3D.h"
#include "Coordinate.h"
#include "Rect2D.h"


class Menu {
public:
	Menu() {
		isShown = true;

		startObj = new EmptyObject();

		attrib_t attrib;

		//start
		Graphic* startGraphic = new Graphic();

		startGraphic->LoadObj("models/start.obj","models/", attrib, 1.0f);
		glActiveTexture(GL_TEXTURE0);
		startGraphic->LoadTexture("models/", attrib.texcoords);
		startGraphic->objectCode = 100;
		startObj->graphic = startGraphic;

		//Coordinate* startCoord = new Coordinate();
		//startObj->coordinate = startCoord;
		startObj->Init();



		//end
		endObj = new EmptyObject();


		Graphic* endGraphic = new Graphic();

		endGraphic->LoadObj("models/end.obj", "models/", attrib, 1.0f);
		glActiveTexture(GL_TEXTURE0);
		endGraphic->LoadTexture("models/", attrib.texcoords);
		endGraphic->objectCode = 101;
		endObj->graphic = endGraphic;

		//Coordinate* endCoord = new Coordinate();
		//endObj->coordinate = endCoord;
		endObj->Init();


		startObj->MoveObject(vec3(-3, 0, 35));
		endObj->MoveObject(vec3(3, 0, 35));
	};

	void Activate();
	void Activate(mat4 p, mat4 v, int color_mode = 0);

private:
	bool isShown;

	EmptyObject* startObj;
	EmptyObject* endObj;
};

