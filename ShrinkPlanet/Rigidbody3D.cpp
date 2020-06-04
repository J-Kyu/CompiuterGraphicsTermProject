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
	*topT = compute_modelling_transf(body);
}

mat4 Rigidbody3D::compute_modelling_transf(dBodyID body)
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
dBodyID Rigidbody3D::GetRigidBodyID() {
	return body;
}


void Rigidbody3D::RotateRigidbody(float angle, vec3 vec) {
	
	GLfloat radian = M_PI / 180;
	dMatrix3 k;
	mat4 r;
	r = rotate(r, angle* radian, vec);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			k[3*i+j] = (dReal)r[i][j];
		}
	}

	dBodySetRotation(body,k);


}