#pragma once
#include "RigidBodyWorld.h"





class Rigidbody3D : public RigidBodyWorld {

public:


	void SphereRigidBodyInit();
	void SphereRigidBodyInit(float radius, float mass, float x, float y, float z);
	void SetKinematic(bool);

	virtual void RotateRigidBody(float angle, vec3 vc) override final;
	void virtual ActivateComponent(mat4) override final;
	void virtual ActivateComponent(int,mat4,mat4, mat4) override final;
	virtual dBodyID GetRigidBodyID() override final;
	virtual mat4 GetRigidBodyTrans() override final;
	virtual void SetRigidBodyTrans(mat4) override final;




	virtual void CheckCollision()override final;
	static void nearCallback(void* data, dGeomID o1, dGeomID o2);
private:

	dBodyID body;
	dGeomID geom;


	mat4 compute_modelling_transf();
	void RigidBodyActivate();

};

