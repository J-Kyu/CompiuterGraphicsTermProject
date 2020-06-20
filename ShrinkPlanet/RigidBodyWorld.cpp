#include "RigidBodyWorld.h"



void RigidBodyWorld::WorldInit() {


	dInitODE2(0);
	RigidBodyWorld::ode_world = dWorldCreate(); // Make a simulation world.
	RigidBodyWorld::ode_space = dSimpleSpaceCreate(0); // Make a collision space.
	RigidBodyWorld::ode_contactgroup = dJointGroupCreate(0); // Make a contact group.
	//dWorldSetGravity(ode_world, 0, -9.8, 0); // Set gravity.
	//dWorldSetGravity(ode_world, 0, -9.8, 0); // Set gravity.

	//RigidBodyWorld::ode_plane_geom = dCreatePlane(RigidBodyWorld::ode_space, 0, 1, 0, 0);
	RigidBodyWorld::pause = false;
}


double RigidBodyWorld::dsElapsedTime()
{
	static double prev = 0.0;
	double curr = ::timeGetTime() / 1000.0;
	if (!prev)
		prev = curr;
	double retval = curr - prev;
	prev = curr;
	if (retval > 1.0) retval = 1.0;
	if (retval < dEpsilon) retval = dEpsilon;
	return retval;
}


void RigidBodyWorld::ActivateComponent(mat4) {
	cout << "This is RigidBody World Virtual Clss.....should not be called" << endl;
}
void RigidBodyWorld::ActivateComponent(int, mat4, mat4, mat4) {
	cout << "This is RigidBody World Virtual Clss.....should not be called" << endl; 
}

dBodyID RigidBodyWorld::GetRigidBodyID(){
	cout << "This is RigidBody World Virtual Clss.....should not be called" << endl;
	return NULL;
}

void RigidBodyWorld::RotateRigidBody(float angle, vec3 vc) {
	cout << "This is RigidBody World Virtual Clss.....should not be called" << endl;
}

void RigidBodyWorld::SetRigidBodyTrans(mat4) {
	cout << "This is RigidBody World Virtual Clss.....should not be called" << endl;
}

void RigidBodyWorld::CheckCollision() {
	cout << "This is RigidBody World Virtual Clss.....should not be called" << endl;
}

void RigidBodyWorld::SetKinematic(bool) {
	cout << "This is not Rigidbody3D Type Object...RigidBodyWorld Type.....should not be called" << endl;
}

dGeomID RigidBodyWorld::GetRigidGeomID() {

	cout << "This object is not Rigidbody Component....Please use this function on TYPE Rigidbody" << endl;
	return NULL;
}