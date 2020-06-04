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

	vec3 gravityUp = vec3(mainEntity->GetObjectT()[3][0] - attractor->GetObjectT()[3][0], mainEntity->GetObjectT()[3][1] - attractor->GetObjectT()[3][1], mainEntity->GetObjectT()[3][2] - attractor->GetObjectT()[3][2]);

	gravityUp = normalize(gravityUp);

	if (!isnan(gravityUp.x)) {
		dMatrix3 R;

		//dRFromAxisAndAngle(R, 0,1, 0, gravityUp.y);
		double gravityCoe = -20;
		dBodyAddForce(mainEntity->components[1]->GetRigidBodyID(), (dReal)gravityUp.x * gravityCoe, (dReal)gravityUp.y * gravityCoe, (dReal)gravityUp.z * gravityCoe);
		//dBodySetRotation(mainEntity->components[1]->GetRigidBodyID(), R);
		dBodyAddForce(mainEntity->components[1]->GetRigidBodyID(), 1.0f,0.0f,0.0f);

		//from to
		/*
			Quaternion targetRotation = Quaternion.FromToRotation(localUp,gravityUp) * body.rotation;
			//body.rotation = Quaternion.Slerp(body.rotation, targetRotation,500f*Time.deltaTime);
			body.rotation = targetRotation;
			//https://stackoverflow.com/questions/51549366/what-is-the-math-behind-fromtorotation-unity3d
			quaternion (w,x,y,z) 순이다...그리고 cross으로 인해서 axis가 음수가 될 수 있는데.....이거 주의하자!
		*/


		


		vec3 localUp = vec3(mainEntity->GetObjectT()[0][1], mainEntity->GetObjectT()[1][1], mainEntity->GetObjectT()[2][1]);

		vec3 axis = abs(normalize(cross(localUp,gravityUp)));

		

		float phi = angle(localUp, gravityUp);

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

		//dQFromAxisAndAngle(targetQ,sin(phi/2)*mainEntity->GetObjectT()[3][0], sin(phi / 2) * mainEntity->GetObjectT()[3][1], sin(phi / 2) * mainEntity->GetObjectT()[3][2], cos(phi/2));
//		dBodySetQuaternion(mainEntity->components[1]->GetRigidBodyID(), targetQ);


		cout << "OW" << endl;

		
	}

}

