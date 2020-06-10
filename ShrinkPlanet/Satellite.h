#pragma once
#include "EmptyObject.h"
#include "Sphere.h"
#include "Rigidbody3D.h"
#include "Coordinate.h"

class Satellite
{

public:
	Satellite() {}

	Satellite(EmptyObject* attr,float radius,float x,float y, float z) {


		attractor = attr;

		graphic = new Sphere(0.1f,16,16);

		rigidbody = new Rigidbody3D();
		rigidbody->SphereRigidBodyInit(.5f, 10.f, x, y, z);
		rigidbody->SetKinematic(true);
		this->radius = radius;
		coordinate = new Coordinate();

		pTheta = 0;

		Init();
	}

	void Init();
	void Activate(int colorMode = 0);
	void Activate(mat4 p, mat4 v, int colorMode);
	void Activate(int, mat4, mat4, mat4);
	void IncreaseRadius(int inc = 1);

	vec3 GetPos();
	mat4 GetObjectMatrix();

	vector<EmptyObject*> children;

	Graphic* graphic;
	Rigidbody3D* rigidbody;
	Coordinate* coordinate;


private:

	EmptyObject* attractor;

	float radius;
	float pTheta;
	mat4 perspectiveT = mat4(1.0f);
	mat4 viewT = mat4(1.0f);
	mat4 objectT = mat4(1.0f); //initinal matrix
	mat4 rotateT = mat4(1.0f); //real time rotating matrix


	void CalculateRigidbody(float);
	quat HemiltonProduct(quat v1, quat v2);
	mat4 QuatToMat4(quat q);
	void PrintMatrix(mat4 m);

	void SetPerspectiveMatrix(mat4);
	void SetViewMatrix(mat4);

};

