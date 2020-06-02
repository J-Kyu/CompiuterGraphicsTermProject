#pragma once
#include "Component.h"
#define dDOUBLE
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <windows.h>
#include <timeapi.h>
#define dEpsilon 0.0016


class RigidBodyWorld :public Component{


public:
	RigidBodyWorld() {
		pause = true;
	}
	static void WorldInit();

	static dWorldID ode_world; // simulation world
	static dSpaceID ode_space; // collision space
	static dJointGroupID ode_contactgroup; // a group of contact joints

	static void CheckCollision();
	static void nearCallback(void* data, dGeomID o1, dGeomID o2);

	static bool pause;

	static double dsElapsedTime();
protected:
	void virtual ActivateComponent();


};

