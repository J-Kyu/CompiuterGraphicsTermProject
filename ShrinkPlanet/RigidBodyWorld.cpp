#include "RigidBodyWorld.h"



void RigidBodyWorld::WorldInit(){


	dInitODE2(0);
	RigidBodyWorld::ode_world = dWorldCreate(); // Make a simulation world.
	RigidBodyWorld::ode_space = dSimpleSpaceCreate(0); // Make a collision space.
	RigidBodyWorld::ode_contactgroup = dJointGroupCreate(0); // Make a contact group.
	//dWorldSetGravity(ode_world, 0, -9.8, 0); // Set gravity.
	//dWorldSetGravity(ode_world, 0, -9.8, 0); // Set gravity.

	RigidBodyWorld::pause = false;
	return;
}

void RigidBodyWorld::CheckCollision() {


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


void RigidBodyWorld::nearCallback(void* data, dGeomID o1, dGeomID o2){
	const int N = 100;
	dContact contact[N];
	int n = dCollide(o1, o2, N, &(contact[0].geom), sizeof(dContact));
	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			contact[i].surface.mode = dContactSoftERP | dContactSoftCFM;
			contact[i].surface.mu = 0.8;
			contact[i].surface.soft_erp = 0.9;
			contact[i].surface.soft_cfm = 0.01;
			dJointID c = dJointCreateContact(ode_world, ode_contactgroup, &contact[i]);
			dBodyID body1 = dGeomGetBody(contact[i].geom.g1);
			dBodyID body2 = dGeomGetBody(contact[i].geom.g2);
			dJointAttach(c, body1, body2);
		}
	}
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