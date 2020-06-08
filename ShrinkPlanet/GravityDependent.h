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
		mainEntity->graphic = mainGraphic;


		Rigidbody3D* mainRD = new Rigidbody3D();
		mainRD->SphereRigidBodyInit(radius, mass, 0.0, 5.0,0.0);
		//mainRD->RotateRigidBody(90, vec3(0.0f, 1.0f, 0.0f));
		//mainRD->ActivateComponent(mat4(1.0f));
		mainEntity->rigidbody = mainRD;

		//mainRD->SetKinematic(true);


		Coordinate* coord = new Coordinate();
		mainEntity->coordinate = coord;

		mainEntity->Init();
		wow = 0;
	}

	EmptyObject* mainEntity;
	EmptyObject* attractor;
	int wow;

	void Activate();
	void Activate(mat4 p, mat4 v, int color_mode = 0);

	void MoveDamObject(int);
private:
	void CalculateRigidbody();
	quat HemiltonProduct(quat v1, quat v2);
	mat4 QuatToMat4(quat q);

};

