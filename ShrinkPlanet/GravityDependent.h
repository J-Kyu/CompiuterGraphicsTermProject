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
#include "Graphic.h"
#include <string>
#include "Rigidbody3D.h"
#include "Coordinate.h"

class GravityDependent{


public:
	GravityDependent() {

	}
	GravityDependent(EmptyObject* attr, const char* objPath, const char* path, float scale = 1.0f, float radius = 1.0f, float mass = 20.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f) {
		attractor = attr;

		attrib_t attrib;
		mainEntity = new EmptyObject();

		Component* mainGraphic = new Graphic();


		mainGraphic->LoadObj(objPath, path, attrib, scale);
		glActiveTexture(GL_TEXTURE0);
		mainGraphic->LoadTexture(path, attrib.texcoords);
		mainGraphic->kyu = 1;

		mainEntity->AddComponent(mainGraphic);


		Rigidbody3D* mainRD = new Rigidbody3D();
		mainRD->SphereRigidBodyInit(radius, mass, x, y, z);
		mainEntity->AddComponent(mainRD);
		//mainRD->SetKinematic(true);
		//mainRD->RotateRigidbody(90, vec3(1.0f, 0.0f, 0.0f));

		Coordinate* coord = new Coordinate();
		mainEntity->AddComponent(coord);

		mainEntity->Init();
	}

	EmptyObject* mainEntity;
	EmptyObject* attractor;


	void Activate();
	void Activate(mat4 p, mat4 v, int color_mode = 0);
private:
	void AttractToAttractor();

};

