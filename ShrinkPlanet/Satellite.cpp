#include "Satellite.h"
#define THRESHOLD 0.0001
#define TINYVALUE(x) (abs(x)<THRESHOLD? 0 : x)
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>

void Satellite::Init() {

	if (graphic != NULL) {
		graphic->InitComponent();
	}
	if (rigidbody != NULL) {
		rigidbody->InitComponent();
	}
	if (coordinate != NULL) {
		coordinate->InitComponent();
	}

	for (int i = 0; i < children.size(); i++) {
		children[i]->Init();
	}
}
void Satellite::Activate(mat4 p, mat4 v, int colorMode){
	/*
	Top paretn Activate
	*/

	SetPerspectiveMatrix(p);
	SetViewMatrix(v);

	pTheta += (float)RigidBodyWorld::dsElapsedTime()/4;
	pTheta = pTheta > 3.14f*2 ? 0 : pTheta;
	

	CalculateRigidbody(pTheta);

	graphic->ActivateComponent(colorMode, perspectiveT, viewT, rigidbody->GetRigidBodyTrans() * objectT * rotateT);

	if (coordinate != NULL) {
		coordinate->ActivateComponent(colorMode, perspectiveT, viewT, rigidbody->GetRigidBodyTrans() * objectT * rotateT);
	}


	for (int i = children.size() - 1; i >= 0; i--) {
		children[i]->Activate(colorMode, perspectiveT, viewT, objectT * rotateT);
	}
}



void Satellite::Activate(int colorMode, mat4 p, mat4 v, mat4 parentT) {
	/*
	Chil Object Activate
	*/

	pTheta += (float)RigidBodyWorld::dsElapsedTime();
	pTheta = pTheta > 3.14f ? 0 : pTheta;

	CalculateRigidbody(pTheta);
	graphic->ActivateComponent(colorMode, p, v, parentT * objectT * rotateT * rigidbody->GetRigidBodyTrans());
	//rigidbody-> ActivateComponent(colorMode, p, v, parentT * objectT * rotateT);
	coordinate->ActivateComponent(colorMode, p, v, parentT * objectT * rotateT * rigidbody->GetRigidBodyTrans());

	for (int i = 0; i < children.size(); i++) {
		children[i]->Activate(colorMode, p, v, parentT * objectT * rotateT * objectT);
	}
}



void Satellite::CalculateRigidbody(float theta){



	//move position
	dBodySetPosition(rigidbody->GetRigidBodyID(), radius * cos(theta), radius * sin(theta), 0);


	const mat4 mainT = objectT;
	const mat4 attT = attractor->GetObjectT();


	const dReal* mainRigidbodyPos = dBodyGetPosition(rigidbody->GetRigidBodyID());



	const mat4 rigidbodyT = rigidbody->GetRigidBodyTrans();


	const mat4 bodyT = rigidbodyT * (mainT);
	const dReal* newMainRigidbodyPos = dBodyGetPosition(rigidbody->GetRigidBodyID());

	//quaternion
	const dReal* attRigidbodyPos = dBodyGetPosition(attractor->rigidbody->GetRigidBodyID());

	vec3 gravityUp = vec3(newMainRigidbodyPos[0] - attRigidbodyPos[0], newMainRigidbodyPos[1] - attRigidbodyPos[1], newMainRigidbodyPos[2] - attRigidbodyPos[2]);

	gravityUp = normalize(gravityUp);

	vec3 localUp = vec3(bodyT[1][0], bodyT[1][1], bodyT[1][2]);
	localUp = normalize(localUp);




	vec3 axis = normalize(cross(localUp, gravityUp));


	float phi = angle(localUp, gravityUp);


	float rad = phi / 2;
	axis *= sin(rad);

	//quat[i]의 순은 w,x,y,z이다

	quat targetQ;
	targetQ.w = cos(rad);
	targetQ.x = axis.x;
	targetQ.y = axis.y;
	targetQ.z = axis.z;

	for (int i = 0; i < 4; i++) {
		if (isnan(targetQ[i])) {
			targetQ[i] = 0;
		}
	}

	//quat는 (x,y,z,w) 순이다........qQuaternion과 순서가 다르다는 점을 이해하자!!
	quat qr = quat_cast(mainT);

	//quat (x,y,z,w)
	quat calculatedQ = HemiltonProduct(targetQ, qr);

	mat4 calculatedQT = QuatToMat4(calculatedQ);

	objectT = calculatedQT;

}


quat Satellite::HemiltonProduct(quat v1, quat v2) {
	//x,y,z,w
	quat q;

	//w
	q.w = v1.w * v2.w - v1.x * v2.x - v1.y * v2.y - v1.z * v2.z;
	q.w = TINYVALUE(q.w);
	//z
	q.x = v1.w * v2.x + v1.x * v2.w + v1.y * v2.z - v1.z * v2.y;
	q.x = TINYVALUE(q.x);
	//y
	q.y = v1.w * v2.y - v1.x * v2.z + v1.y * v2.w + v1.z * v2.x;
	q.y = TINYVALUE(q.y);
	//z
	q.z = v1.w * v2.z + v1.x * v2.y - v1.y * v2.x + v1.z * v2.w;
	q.z = TINYVALUE(q.z);

	return q;
}


mat4 Satellite::QuatToMat4(quat q) {
	//https://www.youtube.com/watch?v=v4Wq3HTlPk4

	mat4 m(1.0f);


	m[0][0] = (float)(1 - 2 * pow(q.y, 2) - 2 * pow(q.z, 2));
	m[0][1] = (float)(2 * q.x * q.y + 2 * q.w * q.z);
	m[0][2] = (float)(2 * q.x * q.z - 2 * q.w * q.y);

	m[1][0] = (float)(2 * q.x * q.y - 2 * q.w * q.z);
	m[1][1] = (float)(1 - 2 * pow(q.x, 2) - 2 * pow(q.z, 2));
	m[1][2] = (float)(2 * q.y * q.z + 2 * q.w * q.x);

	m[2][0] = (float)(2 * q.x * q.z + 2 * q.w * q.y);
	m[2][1] = (float)(2 * q.y * q.z - 2 * q.w * q.x);
	m[2][2] = (float)(1 - 2 * pow(q.x, 2) - 2 * pow(q.y, 2));

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (abs(m[i][j]) < THRESHOLD) {
				m[i][j] = 0;
			}
		}
	}


	return m;
}


void Satellite::SetPerspectiveMatrix(mat4 T) {
	perspectiveT = T;
}

void Satellite::SetViewMatrix(mat4 T) {
	viewT = T;
}

void Satellite::PrintMatrix(mat4 m) {

	cout << "---------------matrix------- " << endl;

	for (int i = 0; i < 4; i++) {
		cout << "|";
		for (int j = 0; j < 4; j++) {
			cout << "\t" << m[i][j];
		}
		cout << "\t|" << endl;
	}

	cout << "\n" << endl;

}


vec3 Satellite::GetPos() {
	const dReal* pos = dBodyGetPosition(this->rigidbody->GetRigidBodyID());
	return vec3(pos[0], pos[1], pos[2]);
}


mat4 Satellite::GetObjectMatrix() {

	mat4 rdMat = rigidbody->GetRigidBodyTrans();
	return rdMat * objectT;

}

void Satellite::IncreaseRadius(int inc) {
	radius += inc;
}