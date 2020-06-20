#pragma once
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EmptyObject.h"
#include "Sphere.h"
#include "Rigidbody3D.h"
#include "Coordinate.h"
#include <ctime>
#include <cstdlib>


class Meteor{

public:
	Meteor() {}

	Meteor(const char* objPath, const char* path,vec3 pos,vec3 dir, int num = 1 ,float scale = 1.0f) {
		
		srand((unsigned int)time(NULL));
		meteorNum = num;
		this->origPos = pos;
		this->dirVector = normalize(dir);

		rigidbody = new Rigidbody3D();
		rigidbody->SphereRigidBodyInit(.5f, 10.f, origPos.x,origPos.y, origPos.z);
		rigidbody->SetKinematic(true);


		ResetTime();
		isFirst = true;

		do {

			if (meteorNum <= 0) break;
			GenerateMeteor(vec3(2.0f, 0.0f, 0.0f), objPath, path, scale);
			if (meteorNum == 1) break;
			GenerateMeteor(vec3(-2.0f, 0.0f, 0.0f), objPath, path, scale);
			if (meteorNum == 2) break;
			GenerateMeteor(vec3(0.0f, 2.0f, 0.0f), objPath, path, scale);
			if (meteorNum == 3) break;
			GenerateMeteor(vec3(0.0f, -2.0f, 0.0f), objPath, path, scale);
			if (meteorNum >= 4) break;

		} while (false);



		Init();
	}

	void Init();
	void Activate(mat4 p, mat4 v, int colorMode);
	void Activate(int, mat4, mat4, mat4);



	void SetPerspectiveMatrix(mat4);
	void SetViewMatrix(mat4);

	vector<EmptyObject*> children;

	Rigidbody3D* rigidbody;
	Coordinate* coordinate;
	Graphic* graphic;


private:
	bool isFirst;
	int meteorNum = 0;
	vec3 origPos;
	vec3 dirVector;


	float elapsedTime;
	float maxTime;
	float speed;

	mat4 perspectiveT = mat4(1.0f);
	mat4 viewT = mat4(1.0f);
	mat4 objectT = mat4(1.0f); //initinal matrix
	mat4 rotateT = mat4(1.0f); //real time rotating matrix
	void MovingMeteor();
	void ResetTime();
	void GenerateMeteor(vec3 pos,const char* objPath, const char* path, float scale = 1.0f);


};

