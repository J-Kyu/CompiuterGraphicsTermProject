#pragma once
#include "RigidBodyWorld.h"





class Rigidbody3D : public RigidBodyWorld {

public:


	void SphereRigidBodyInit();
	void SphereRigidBodyInit(float radius, float mass, float x, float y, float z);
	void SetKinematic(bool);
	void RotateRigidbody(float, vec3);

	void virtual ActivateComponent(mat4) override final;
	void virtual ActivateComponent(int,mat4,mat4, mat4) override final;
	virtual dBodyID GetRigidBodyID() override final;
	virtual mat4 GetRigidBodyTrans() override final;
private:

	dBodyID body;
	dGeomID geom;


	mat4 compute_modelling_transf();
	void RigidBodyActivate();

};

