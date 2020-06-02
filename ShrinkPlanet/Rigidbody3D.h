#pragma once
#include "RigidBodyWorld.h"





class Rigidbody3D : public RigidBodyWorld {

public:


	void SphereRigidBodyInit();
	void SphereRigidBodyInit(float radius, float mass, float x, float y, float z);
	void SetKinematic();
	void RigidBodyActivate();
	void virtual ActivateComponent() override final;
private:

	dBodyID body;
	dGeomID geom;


	mat4 compute_modelling_transf(dBodyID body);


};

