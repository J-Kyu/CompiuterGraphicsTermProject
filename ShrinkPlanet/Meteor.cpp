#include "Meteor.h"



void Meteor::Init() {

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


void Meteor::GenerateMeteor(vec3 pos, const char* objPath, const char* path, float scale ) {

	EmptyObject* meteor = new EmptyObject();


	meteor->graphic = new Graphic();

	attrib_t attrib;

	meteor->graphic->LoadObj(objPath,path, attrib, scale);
	glActiveTexture(GL_TEXTURE0);
	meteor->graphic->LoadTexture(path, attrib.texcoords);
	meteor->graphic->objectCode = 1;

	meteor->MoveObject(pos);

	this->children.push_back(meteor);

}

void Meteor::Activate(mat4 p, mat4 v, int colorMode) {
	/*
	Top paretn Activate
	*/

	SetPerspectiveMatrix(p);
	SetViewMatrix(v);

	//pTheta += (float)RigidBodyWorld::dsElapsedTime();
	//pTheta = pTheta > 3.14f * 2 ? 0 : pTheta;
	MovingMeteor();

	if (isFirst) {
		return;
	}


	mat4 rdMat(1.0f);
	if (rigidbody != NULL) {
		rdMat = rigidbody->GetRigidBodyTrans();
	}

	//CalculateRigidbody(pTheta);

	if (graphic != NULL) {
		graphic->ActivateComponent(colorMode, perspectiveT, viewT, rdMat * objectT * rotateT);
	}

	if (coordinate != NULL) {
		coordinate->ActivateComponent(colorMode, perspectiveT, viewT, rdMat * objectT * rotateT);
	}

	for (int i = children.size() - 1; i >= 0; i--) {
		children[i]->Activate(colorMode, perspectiveT, viewT, rdMat * objectT * rotateT);
	}
}


void Meteor::Activate(int colorMode, mat4 p, mat4 v, mat4 parentT) {
	/*
	called as Child Object Activate
	*/

	mat4 rdMat(1.0f);
	if (rigidbody != NULL) {
		rdMat = rigidbody->GetRigidBodyTrans();
	}

	if (graphic != NULL) {
		graphic->ActivateComponent(colorMode, p, v, parentT * rdMat * objectT * rotateT);
	}

	if (coordinate != NULL) {
		coordinate->ActivateComponent(colorMode, p, v, parentT * rdMat * objectT * rotateT);
	}

}



void Meteor::SetPerspectiveMatrix(mat4 T) {
	perspectiveT = T;
}

void Meteor::SetViewMatrix(mat4 T) {
	viewT = T;
}

void Meteor::MovingMeteor() {

	elapsedTime += (float)RigidBodyWorld::dsElapsedTime();


	if (elapsedTime > maxTime) {
		//reset to ori position
		dBodySetPosition(this->rigidbody->GetRigidBodyID(),origPos.x,origPos.y,origPos.z );
		ResetTime();
		return;

	}

	for (int i = children.size() - 1; i >= 0; i--) {
		children[i]->RotatingAxis(vec3(1.0, 1.0f, 0.0f));
	}

	do {

		if (meteorNum <= 0) break;
		children[0]->RotatingAxis(vec3(0.0, 1.0f, -1.0f),4.0f);
		if (meteorNum == 1) break;
		children[1]->RotatingAxis(vec3(-1.0, 1.0f, 0.0f),-1.0f);
		if (meteorNum == 2) break;
		children[2]->RotatingAxis(vec3(1.0, 1.0f, -1.0f));
		if (meteorNum == 3) break;
		children[3]->RotatingAxis(vec3(1.0, -1.0f, 1.0f),-1.0);
		if (meteorNum >= 4) break;

	} while (false);

	const dReal* mainRigidbodyPos = dBodyGetPosition(this->rigidbody->GetRigidBodyID());
	vec3 curPos = vec3(mainRigidbodyPos[0], mainRigidbodyPos[1], mainRigidbodyPos[2]);

	dBodySetPosition(this->rigidbody->GetRigidBodyID(), curPos.x+dirVector.x* speed* elapsedTime, curPos.y+ dirVector.y* speed* elapsedTime, curPos.z+ dirVector.z* speed* elapsedTime);

}

void Meteor::ResetTime() {
	maxTime = 3.5f + rand() % 3;
	speed = (float)(10 + rand() % 50) / 100;
	elapsedTime = 0;
	isFirst = false;
	//cout << "MaxTime:" << maxTime << "\tSpeed:\t" << speed << endl;
}