#include "GravityDependent.h"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>
#define _USE_MATH_DEFINES
#define THRESHOLD 0.0001
#define TINYVALUE(x) (abs(x)<THRESHOLD? 0 : x)
//using namespace glm;

void GravityDependent::Activate() {

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->Activate();
	}
}

void GravityDependent::Activate(mat4 p, mat4 v, int color_mode) {

	if (attractor == NULL) {
		cout << "Attracor is not assigned   !" << endl;
		exit(0);
	}


	CalculateRigidbody();

	//blocks[0]->rigidbody->CheckCollision();

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->rigidbody->CheckCollision();
		blocks[i]->SetPerspectiveMatrix(p);
		blocks[i]->SetViewMatrix(v);
		blocks[i]->Activate(color_mode);
	}



}

EmptyObject* GravityDependent::CopyBlock() {

	EmptyObject* mainEntity;

	attrib_t attrib;
	mainEntity = new EmptyObject();

	Component* mainGraphic = new Graphic();


	mainGraphic->CopyGraphic(defaultGraphic);
	glActiveTexture(GL_TEXTURE0);

	mainGraphic->objectCode = 1;
	mainEntity->graphic = mainGraphic;


	Rigidbody3D* mainRD = new Rigidbody3D();
	//mainRD->SphereRigidBodyInit(radius, mass, 0, 0, 0);
	mainRD->BoxRigidBodyInit(vec3(0.5f, 0.5f, 0.5f),vec3(0,20,0), mass);
	//mainRD->TrimeshRigidbodyInit(mainGraphic->GetVertices(),mass,0,0,0);
	mainEntity->rigidbody = mainRD;

	mainRD->SetKinematic(true);


	Coordinate* coord = new Coordinate();
	mainEntity->coordinate = coord;

	mainEntity->Init();
	wow = 0;
	elapsedTime = 0;

	return mainEntity;


}

void GravityDependent::CalculateRigidbody() {
	/*
		Physic Engine:
		* gravity
		* quaternion
	*/
	if (elapsedTime > 5.0f) {
		cout << "Passed 5 secs" << endl;
		elapsedTime = 0.0f;
		//dBodyDestroy(blocks[i]->rigidbody->GetRigidBodyID());
	}

	for (int i = 0; i < blocks.size(); i++) {
		elapsedTime += (float)RigidBodyWorld::dsElapsedTime();


		//move position

		const mat4 mainT = blocks[i]->GetObjectT();
		const mat4 attT = attractor->GetObjectT();
		const mat4 rotateT = blocks[i]->GetRotateT();

		const dReal* mainRigidbodyPos = dBodyGetPosition(blocks[i]->rigidbody->GetRigidBodyID());



		const mat4 rigidbodyT = blocks[i]->rigidbody->GetRigidBodyTrans();


		const mat4 bodyT = rigidbodyT * (mainT * rotateT);
		const dReal* newMainRigidbodyPos = dBodyGetPosition(blocks[i]->rigidbody->GetRigidBodyID());

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

		//quat[i]�� ���� w,x,y,z�̴�

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


		//quat�� (x,y,z,w) ���̴�........qQuaternion�� ������ �ٸ��ٴ� ���� ��������!!
		quat qr = quat_cast(mainT);

		//quat (x,y,z,w)
		quat calculatedQ = HemiltonProduct(targetQ, qr);

		mat4 calculatedQT = QuatToMat4(calculatedQ);

		//blocks[i]->SetObjectT(calculatedQT);


		//gravity apply
		double gravityCoe = -10;
		dBodyAddForce(blocks[i]->rigidbody->GetRigidBodyID(), (dReal)gravityUp.x * gravityCoe, (dReal)gravityUp.y * gravityCoe, (dReal)gravityUp.z * gravityCoe);
	}

	
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

void GravityDependent::GenerateBlock(vec3 pos){
	if (readyBlock.size() < 1) {
		cout << "Block has been sold out~!" << endl;
		return;
	}

	UI::GetInstance()->GenerateScoreBoard();
	GameSystem::GetInstance()->BlockProduced();
	EmptyObject* tempObj = readyBlock.front();
	readyBlock.pop();

	//tempObj->rigidbody->SphereRigidBodyInit(radius, mass, 10, 10, 10);
	dBodySetPosition(tempObj->rigidbody->GetRigidBodyID(), (0.7)*pos.x, (0.7) * pos.y, (0.7) * pos.z);
	tempObj->rigidbody->SetKinematic(false);
	blocks.push_back(tempObj);
}