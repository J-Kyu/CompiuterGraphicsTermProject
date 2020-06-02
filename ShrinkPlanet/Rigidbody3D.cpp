#include "Rigidbody3D.h"


void Rigidbody3D::ActivateComponent() {
	RigidBodyActivate();
}


void Rigidbody3D::SphereRigidBodyInit() {


	dMatrix3 R;
	dMass m;
	dRSetIdentity(R);


	//sphere

	body = dBodyCreate(RigidBodyWorld::ode_world);
	dBodySetPosition(body, 2, 10, 0);
	dBodySetRotation(body, R);
	dBodySetLinearVel(body, 0, 0, 0);
	dBodySetAngularVel(body, 0, 0, 0);
	geom = dCreateSphere(RigidBodyWorld::ode_space, 0.2f);
	dGeomSetBody(geom, body);
	dMassSetSphereTotal(&m, 20, .2f);
	dBodySetMass(body, &m);


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

}

void Rigidbody3D::SetKinematic() {

	dBodySetKinematic(body);
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