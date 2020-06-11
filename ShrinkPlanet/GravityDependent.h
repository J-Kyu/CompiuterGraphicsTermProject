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
#include <queue>
#include "GameSystem.h"

class GravityDependent{


public:
	GravityDependent() {

	}
	GravityDependent(EmptyObject* attr, const char* objPath, const char* path, float scale = 1.0f, float radius = 1.0f, float mass = 20.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f) {
		attractor = attr;
		this->mass = mass;
		this->scale = scale;
		this->radius = radius;

		EmptyObject* mainEntity;

		attrib_t attrib;
		mainEntity = new EmptyObject();

		defaultGraphic = new Graphic();


		defaultGraphic->LoadObj(objPath, path, attrib, scale);
		glActiveTexture(GL_TEXTURE0);
		defaultGraphic->LoadTexture(path, attrib.texcoords);
		defaultGraphic->objectCode = 1;
		mainEntity->graphic = defaultGraphic;


		Rigidbody3D* mainRD = new Rigidbody3D();
		mainRD->BoxRigidBodyInit(vec3(1.0f, 1.0f, 1.0f),vec3(0,5,0), mass);
		GameSystem::GetInstance()->SetInitBlock(mainRD->GetRigidGeomID());
		mainEntity->rigidbody = mainRD;

		mainRD->SetKinematic(true);


		Coordinate* coord = new Coordinate();
		mainEntity->coordinate = coord;

		mainEntity->Init();
		wow = 0;
		elapsedTime = 0;
		blockMaxNum = 10;

		blocks.push_back(mainEntity);


		for (int i = 0; i < blockMaxNum; i++) {
			readyBlock.push(CopyBlock());
		}

	}


	EmptyObject* attractor;

	vector <EmptyObject*> blocks;
	int wow;

	void Activate();
	void Activate(mat4 p, mat4 v, int color_mode = 0);
	void GenerateBlock(vec3);

private:

	queue<EmptyObject*> readyBlock;

	float mass;
	float scale;
	float radius;
	int blockMaxNum;

	Component* defaultGraphic;
	
	EmptyObject* CopyBlock();
	float elapsedTime;
	void CalculateRigidbody();
	quat HemiltonProduct(quat v1, quat v2);
	mat4 QuatToMat4(quat q);
	void PrintMatrix(mat4);
};

