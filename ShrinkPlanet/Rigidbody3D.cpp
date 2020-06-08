#include "Rigidbody3D.h"



void Rigidbody3D::ActivateComponent(mat4) {
	RigidBodyActivate();
}
void Rigidbody3D::ActivateComponent(int, mat4, mat4, mat4) {
	RigidBodyActivate();
}



void Rigidbody3D::SphereRigidBodyInit() {

	SphereRigidBodyInit(1.0f,20.0f, 0.0f, 0.0f, 0.0f);

}

void Rigidbody3D::SphereRigidBodyInit(float radius, float mass, float x, float y, float z){


	dMatrix3 R;
	dMass m;
	dRSetIdentity(R);


	//sphere

	body = dBodyCreate(RigidBodyWorld::ode_world);
	dBodySetPosition(body, x, y, z);
	dBodySetRotation(body, R);
	dBodySetLinearVel(body, 0, 0, 0);
	dBodySetAngularVel(body, 0, 0, 0);
	geom = dCreateSphere(RigidBodyWorld::ode_space, radius);
	dGeomSetBody(geom, body);
	dMassSetSphereTotal(&m, mass, radius);
	dBodySetMass(body, &m);

	//*topT = compute_modelling_transf(body);

}

void Rigidbody3D::SetKinematic(bool is) {
	
	if (is) {
		dBodySetKinematic(body);
	}
	else {
		dBodySetDynamic(body);
	}
}

void Rigidbody3D::RigidBodyActivate() {
	//cout << "WOW" << endl;
	//(*topT) *= compute_modelling_transf();
}

mat4 Rigidbody3D::compute_modelling_transf()
{
	mat4 M(1.0f);
	const dReal* pos = dBodyGetPosition(body);
	const dReal* rot = dBodyGetRotation(body);
	M[3] = vec4(pos[0], pos[1], pos[2], 1.0f);
	for (int i = 0; i < 3; ++i) // for each column
		for (int j = 0; j < 3; ++j) // for each row
			M[i][j] = rot[j * 4 + i];
	return M;
}

void Rigidbody3D::SetRigidBodyTrans(mat4 t)
{


	dMatrix3 rot;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			rot[i + 4 * j] = t[i][j];
		}
	}
	cout << "1" << endl;
	dBodySetPosition(body, t[3][0], t[3][1], t[3][2]);
	cout << "2" << endl;
	dBodySetRotation(body, rot);
	cout << "3\n" << endl;


}
dBodyID Rigidbody3D::GetRigidBodyID() {
	return body;
}


void Rigidbody3D::RotateRigidBody(float speedR, vec3 vec) {
	
	//GLfloat radian = M_PI / 180;

	mat4 r = compute_modelling_transf();

	//cout << "WOW" << endl;

	r = rotate(r, speedR * (float)RigidBodyWorld::dsElapsedTime(), vec);
	//r = rotate(r, speedR* radian, vec);



	const dReal* bodyRot = dBodyGetRotation(body);

	dMatrix3 k;
	for (int i = 0; i < 12; i++) {
		k[i] = bodyRot[i];
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			k[i + 4*j] = r[i][j];
		}
	}

	dBodySetRotation(body,k);


}

mat4 Rigidbody3D::GetRigidBodyTrans() {
	return compute_modelling_transf();
}