#pragma once
#include "EmptyObject.h"

#include <GL/glew.h> 
#include <GL/glut.h> 
#include <GL/freeglut.h>
#include <vector>
#include "glm/gtx/string_cast.hpp"
#include "Component.h"
#include "Graphic.h"
#include <string>
#include "Rigidbody3D.h"
#include "Coordinate.h"

using namespace std;
using namespace glm;

class GravityAttractor{

public:
	GravityAttractor() {

	}
	GravityAttractor(const char* objPath, const char* path, float scale = 1.0f, float radius = 1.0f, float mass = 20.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f) {
		attrib_t attrib;
		mainEntity = new EmptyObject();

		Component* mainGraphic = new Graphic();


		mainGraphic->LoadObj(objPath, path, attrib, scale);
		glActiveTexture(GL_TEXTURE0);
		mainGraphic->LoadTexture(path, attrib.texcoords);
		mainGraphic->objectCode = 1;

		mainEntity->graphic = mainGraphic;


		Rigidbody3D* mainRD = new Rigidbody3D();
		mainRD->SphereRigidBodyInit(radius, mass, x, y, z);
		mainRD->SetKinematic(true);
		//mainRD->RotateRigidbody(90, vec3(0.0f, 1.0f, 0.0f));
		mainEntity->rigidbody = mainRD;


		Coordinate* coord = new Coordinate();
		mainEntity->coordinate = coord;

		mainEntity->Init();
	}

	EmptyObject* mainEntity;

	void Activate();
	void Activate(mat4 p, mat4 v, int color_mode = 0);
private:

};

