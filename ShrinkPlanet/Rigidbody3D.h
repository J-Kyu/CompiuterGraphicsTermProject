#pragma once
#include "RigidBodyWorld.h"
#include "GameSystem.h"




class Rigidbody3D : public RigidBodyWorld {

public:


	void SphereRigidBodyInit();
	void SphereRigidBodyInit(float radius, float mass, float x, float y, float z);
	void TrimeshRigidbodyInit(vector<tinyobj::real_t>,float , float , float , float );
	void BoxRigidBodyInit(vec3, vec3,float);

	virtual void RotateRigidBody(float angle, vec3 vc) override final;
	void SetKinematic(bool) override final;
	void virtual ActivateComponent(mat4) override final;
	void virtual ActivateComponent(int,mat4,mat4, mat4) override final;
	virtual dBodyID GetRigidBodyID() override final;
	virtual dGeomID GetRigidGeomID() override final;
	virtual mat4 GetRigidBodyTrans() override final;
	virtual void SetRigidBodyTrans(mat4) override final;




	virtual void CheckCollision()override final;
	static void nearCallback(void* data, dGeomID o1, dGeomID o2);
private:

	dBodyID body;
	dGeomID geom;
	dTriMeshDataID ode_trimesh_data;
	vector<dTriIndex> ode_trimesh_index;



	mat4 compute_modelling_transf();
	void RigidBodyActivate();

};

