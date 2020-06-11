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


}


void Rigidbody3D::TrimeshRigidbodyInit(vector<tinyobj::real_t>vertices, float mass, float x, float y, float z) {


	dMatrix3 R;
	dMass m;
	dRSetIdentity(R);


	body = dBodyCreate(RigidBodyWorld::ode_world);
	dBodySetPosition(body, 0, 0, 0);
	dBodySetRotation(body, R);
	dBodySetLinearVel(body, 0, 0, 0);
	dBodySetAngularVel(body, 0, 0, 0);

	int n = (int)(vertices.size() / 3);
	ode_trimesh_index.resize(n);
	for (int i = 0; i < n; i++) {
		ode_trimesh_index[i] = i;
	}

	ode_trimesh_data = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSingle(ode_trimesh_data, vertices.data(), 3 * sizeof(float), n, ode_trimesh_index.data(),n, 3 * sizeof(dTriIndex));

	geom = dCreateTriMesh(RigidBodyWorld::ode_space, ode_trimesh_data,0,0,0);
	dGeomSetBody(geom, body);
	dMassSetTrimeshTotal(&m, mass, geom);
	dGeomSetPosition(geom, -m.c[0], -m.c[1], -m.c[2]);
	dMassTranslate(&m, -m.c[0], -m.c[1], -m.c[2]);
	dBodySetMass(body,&m);

	dBodySetPosition(body, x, y, z);
}

void Rigidbody3D::BoxRigidBodyInit(vec3 length,vec3 pos, float mass) {
	dMatrix3 R;
	dMass m;
	dRSetIdentity(R);


	//box

	body = dBodyCreate(RigidBodyWorld::ode_world);
	dBodySetPosition(body, pos.x, pos.y, pos.z);
	dBodySetRotation(body, R);
	dBodySetLinearVel(body, 0, 0, 0);
	dBodySetAngularVel(body, 0, 0, 0);

	geom = dCreateBox(RigidBodyWorld::ode_space, length.x, length.y, length.z);
	dGeomSetBody(geom, body);
	dMassSetBoxTotal(&m, mass, length.x, length.y, length.z);
	dBodySetMass(body, &m);

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

	dBodySetPosition(body, t[3][0], t[3][1], t[3][2]);
	dBodySetRotation(body, rot);


}
dBodyID Rigidbody3D::GetRigidBodyID() {
	return body;
}


void Rigidbody3D::RotateRigidBody(float speedR, vec3 vec) {
	
	//GLfloat radian = M_PI / 180;

	mat4 r = compute_modelling_transf();

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


void Rigidbody3D::CheckCollision() {

	double stepsize = 0.005; // 5ms simulation step size
	double dt = dsElapsedTime();

	int no_of_steps = (int)ceilf(dt / stepsize);

	for (int i = 0; !RigidBodyWorld::pause && i < no_of_steps; ++i)
	{

		dSpaceCollide(RigidBodyWorld::ode_space, 0, &nearCallback);
		dWorldQuickStep(RigidBodyWorld::ode_world, stepsize);
		dJointGroupEmpty(RigidBodyWorld::ode_contactgroup); // remove all contact joints
	}
}


void Rigidbody3D::nearCallback(void* data, dGeomID o1, dGeomID o2) {

	 GameSystem::GetInstance()->CollideCheck(o1,o2);


	const int N = 100;
	dContact contact[N];
	int n = dCollide(o1, o2, N, &(contact[0].geom), sizeof(dContact));

	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			contact[i].surface.mode = dContactSoftERP | dContactSoftCFM;
			contact[i].surface.mu = 0.9;
			contact[i].surface.soft_erp = 0.001;
			contact[i].surface.soft_cfm = 0.0001;

			dJointID c = dJointCreateContact(RigidBodyWorld::ode_world, RigidBodyWorld::ode_contactgroup, &contact[i]);
			dBodyID body1 = dGeomGetBody(contact[i].geom.g1);
			dBodyID body2 = dGeomGetBody(contact[i].geom.g2);
			dJointAttach(c, body1, body2);
		}
	}
}
dGeomID Rigidbody3D::GetRigidGeomID() {

	return geom;

}
