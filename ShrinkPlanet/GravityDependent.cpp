#include "GravityDependent.h"
#include <glm/gtx/vector_angle.hpp>
#define _USE_MATH_DEFINES


void GravityDependent::Activate() {
	mainEntity->Activate();
}

void GravityDependent::Activate(mat4 p, mat4 v, int color_mode) {

	if (attractor == NULL) {
		cout << "Attracor is not assigned   !" << endl;
		exit(0);
	}
	AttractToAttractor();
	mainEntity->SetPerspectiveMatrix(p);
	mainEntity->SetViewMatrix(v);
	mainEntity->Activate(color_mode);
}

void GravityDependent::AttractToAttractor() {


	const dReal* tempGravity = dBodyGetPosition(attractor->components[1]->GetRigidBodyID());
	const dReal* tempLocal = dBodyGetPosition(mainEntity->components[1]->GetRigidBodyID());
	vec3 gravityUp = vec3(tempLocal[0]- tempGravity[0], tempLocal[1] - tempGravity[1], tempLocal[2] - tempGravity[2]);
	
	vec3 forward = (float)RigidBodyWorld::dsElapsedTime()/**(float)0.01*/*vec3(mainEntity->GetObjectT()[2][0], mainEntity->GetObjectT()[2][1], mainEntity->GetObjectT()[2][2]);
	dBodySetPosition(mainEntity->components[1]->GetRigidBodyID(), tempLocal[0]+forward.x, tempLocal[1]+forward.y, tempLocal[2]+forward.z);
	gravityUp = normalize(gravityUp);

	wow++;

	if (!isnan(gravityUp.x)) {
		dMatrix3 R;


		double gravityCoe = -20;
		dBodyAddForce(mainEntity->components[1]->GetRigidBodyID(), (dReal)gravityUp.x * gravityCoe, (dReal)gravityUp.y * gravityCoe, (dReal)gravityUp.z * gravityCoe);

		//from to
		/*
			Quaternion targetRotation = Quaternion.FromToRotation(localUp,gravityUp) * body.rotation;
			//body.rotation = Quaternion.Slerp(body.rotation, targetRotation,500f*Time.deltaTime);
			body.rotation = targetRotation;
			//https://stackoverflow.com/questions/51549366/what-is-the-math-behind-fromtorotation-unity3d
			//https://stackoverflow.com/questions/50081475/opengl-local-up-and-right-from-matrix-4x4
			//https://answers.unity.com/questions/1668856/whats-the-source-code-of-quaternionfromtorotation.html
			quaternion (w,x,y,z) ���̴�...�׸��� cross���� ���ؼ� axis�� ������ �� �� �ִµ�.....�̰� ��������!
		*/

		vec3 localUp = vec3(mainEntity->GetObjectT()[1][0], mainEntity->GetObjectT()[1][1], mainEntity->GetObjectT()[1][2]);


		vec3 axis = normalize(cross(localUp,gravityUp));


		
		//float phi = angle(localUp, gravityUp);
		float dotPhi = dot(localUp, gravityUp);
		float phi = acos(dotPhi);


		float rad = phi / 2 ;

		axis *= sin(rad);

		dQuaternion targetQ = { cos(rad),axis[0],axis[1],axis[2] };


		const dReal* qr = dBodyGetQuaternion(mainEntity->components[1]->GetRigidBodyID());

		dQuaternion calculatedQ = {
			{ targetQ[0] * qr[0] - targetQ[1] * qr[1] - targetQ[2] * qr[2] - targetQ[3] * qr[3]},
			{ targetQ[0] * qr[1] + targetQ[1] * qr[0] + targetQ[2] * qr[3] - targetQ[3] * qr[2]},
			{ targetQ[0] * qr[2] - targetQ[1] * qr[3] + targetQ[2] * qr[0] + targetQ[3] * qr[1]},
			{ targetQ[0] * qr[3] + targetQ[1] * qr[2] - targetQ[2] * qr[1] + targetQ[3] * qr[0]}
		};

		if (!isnan(calculatedQ[1])) {

			dBodySetQuaternion(mainEntity->components[1]->GetRigidBodyID(), calculatedQ);
			return;
		}

		cout << "OW" << endl;

		
	}

}
