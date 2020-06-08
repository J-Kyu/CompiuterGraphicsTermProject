#include "GravityDependent.h"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>
#define _USE_MATH_DEFINES
#define THRESHOLD 0.0001
#define TINYVALUE(x) (abs(x)<THRESHOLD? 0 : x)
//using namespace glm;

void GravityDependent::Activate() {
	mainEntity->Activate();
}

void GravityDependent::Activate(mat4 p, mat4 v, int color_mode) {

	if (attractor == NULL) {
		cout << "Attracor is not assigned   !" << endl;
		exit(0);
	}

	mainEntity->rigidbody->CheckCollision();
	CalculateRigidbody();

	mainEntity->SetPerspectiveMatrix(p);
	mainEntity->SetViewMatrix(v);
	mainEntity->Activate(color_mode);
	//apply rigid body topT*rigidbodyT
}

void GravityDependent::CalculateRigidbody() {



	elapsedTime += (float)RigidBodyWorld::dsElapsedTime();

	if (elapsedTime > 5.0f) {
		cout << "Passed 5 secs" << endl;
		elapsedTime = 0.0f;
		//dBodyDestroy(mainEntity->rigidbody->GetRigidBodyID());
	}

	//move position

	const mat4 mainT = mainEntity->GetObjectT();
	const mat4 attT = attractor->GetObjectT();
	const mat4 rotateT = mainEntity->GetRotateT();

	const dReal* mainRigidbodyPos = dBodyGetPosition(mainEntity->rigidbody->GetRigidBodyID());



	const mat4 rigidbodyT = mainEntity->rigidbody->GetRigidBodyTrans();


	const mat4 bodyT = rigidbodyT*(mainT* rotateT);
	const dReal* newMainRigidbodyPos = dBodyGetPosition(mainEntity->rigidbody->GetRigidBodyID());
	
	//vec3 forward = (5)*(float)RigidBodyWorld::dsElapsedTime() * normalize(vec3(bodyT[2][0],bodyT[2][1], bodyT[2][2]));
	//vec3 right = (float)RigidBodyWorld::dsElapsedTime() * normalize(vec3(bodyT[0][0], bodyT[0][1], bodyT[0][2]));
	//dBodySetPosition(mainEntity->rigidbody->GetRigidBodyID(), mainRigidbodyPos[0] + forward.x, mainRigidbodyPos[1] + forward.y, mainRigidbodyPos[2] + forward.z);	
	//dBodySetPosition(mainEntity->rigidbody->GetRigidBodyID(), mainRigidbodyPos[0] + right.x, mainRigidbodyPos[1] + right.y, mainRigidbodyPos[2] + right.z);

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

	/*
	quaternion을 적용하지 않으면 잘 된다.....
	근데 다른 것들은 어떡하지....?
	quaternion 이 문제인 것같다.
	
	
	*/



	//quat는 (x,y,z,w) 순이다........qQuaternion과 순서가 다르다는 점을 이해하자!!
	quat qr = quat_cast(mainT);

	//quat (x,y,z,w)
	quat calculatedQ = HemiltonProduct(targetQ, qr);

	mat4 calculatedQT = QuatToMat4(calculatedQ);

	mainEntity->SetObjectT(calculatedQT);
	//cout << "----------------------------------??-----------------" << endl;
	//PrintMatrix(rigidbodyT*calculatedQT);
	//PrintMatrix();
	//cout << "----------------------------------??-----------------" << endl;


	//gravity apply
	double gravityCoe = -5;
	dBodyAddForce(mainEntity->rigidbody->GetRigidBodyID(), (dReal)gravityUp.x * gravityCoe, (dReal)gravityUp.y * gravityCoe, (dReal)gravityUp.z * gravityCoe);
}


quat GravityDependent::HemiltonProduct(quat v1, quat v2) {
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


mat4 GravityDependent::QuatToMat4(quat q) {
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


void GravityDependent::MoveDamObject(int a) {

	const mat4 rigidbodyT = mainEntity->rigidbody->GetRigidBodyTrans();
	const mat4 mainT = mainEntity->GetObjectT();
	const dReal* mainRigidbodyPos = dBodyGetPosition(mainEntity->rigidbody->GetRigidBodyID());
	const mat4 rotateT = mainEntity->GetRotateT();


	const mat4 bodyT = rigidbodyT * (mainT * rotateT);
	vec3 forward =(a) *(float)RigidBodyWorld::dsElapsedTime() * normalize(vec3(bodyT[2][0], bodyT[2][1], bodyT[2][2]));
	dBodySetPosition(mainEntity->rigidbody->GetRigidBodyID(), mainRigidbodyPos[0] + forward.x, mainRigidbodyPos[1] + forward.y, mainRigidbodyPos[2] + forward.z);


	

}

void GravityDependent::PrintMatrix(mat4 m) {

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