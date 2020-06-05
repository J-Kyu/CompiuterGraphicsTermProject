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
	//AttractToAttractor();
	mainEntity->SetPerspectiveMatrix(p);
	mainEntity->SetViewMatrix(v);
	mainEntity->Activate(color_mode);
}

void GravityDependent::AttractToAttractor() {


	const dReal* tempGravity = dBodyGetPosition(attractor->components[1]->GetRigidBodyID());
	const dReal* tempLocal = dBodyGetPosition(mainEntity->components[1]->GetRigidBodyID());
	vec3 gravityUp = vec3(tempLocal[0]- tempGravity[0], tempLocal[1] - tempGravity[1], tempLocal[2] - tempGravity[2]);
	//vec3 gravityUp = vec3(mainEntity->GetObjectT()[3][0] - attractor->GetObjectT()[3][0], mainEntity->GetObjectT()[3][1] - attractor->GetObjectT()[3][1], mainEntity->GetObjectT()[3][2] - attractor->GetObjectT()[3][2]);

	gravityUp = normalize(gravityUp);

	if (!isnan(gravityUp.x)) {
		dMatrix3 R;

		//dRFromAxisAndAngle(R, 0,1, 0, gravityUp.y);
		double gravityCoe = -20;
		dBodyAddForce(mainEntity->components[1]->GetRigidBodyID(), (dReal)gravityUp.x * gravityCoe, (dReal)gravityUp.y * gravityCoe, (dReal)gravityUp.z * gravityCoe);
		//dBodySetRotation(mainEntity->components[1]->GetRigidBodyID(), R);
		//dBodyAddForce(mainEntity->components[1]->GetRigidBodyID(), 0.5f,0.0f,0.0f);

		//from to
		/*
			Quaternion targetRotation = Quaternion.FromToRotation(localUp,gravityUp) * body.rotation;
			//body.rotation = Quaternion.Slerp(body.rotation, targetRotation,500f*Time.deltaTime);
			body.rotation = targetRotation;
			//https://stackoverflow.com/questions/51549366/what-is-the-math-behind-fromtorotation-unity3d
			quaternion (w,x,y,z) 순이다...그리고 cross으로 인해서 axis가 음수가 될 수 있는데.....이거 주의하자!
		*/

		/*
		if (wow == false) {
			return;
		}

		wow = false;
		*/
		//vec3 localUp = vec3(mainEntity->GetObjectT()[0][1], mainEntity->GetObjectT()[1][1], mainEntity->GetObjectT()[2][1]);
		//compute_modelling_transf
		vec3 localUp = vec3(mainEntity->components[1]->GetRigidBodyTrans()[0][1], mainEntity->components[1]->GetRigidBodyTrans()[1][1], mainEntity->components[1]->GetRigidBodyTrans()[2][1]);
	/*	vec3 localLeft = vec3(mainEntity->components[1]->GetRigidBodyTrans()[0][0], mainEntity->components[1]->GetRigidBodyTrans()[1][0], mainEntity->components[1]->GetRigidBodyTrans()[2][0]);
		vec3 localRight = vec3(mainEntity->components[1]->GetRigidBodyTrans()[0][2], mainEntity->components[1]->GetRigidBodyTrans()[1][2], mainEntity->components[1]->GetRigidBodyTrans()[2][2]);*/



		//vec3 localUp = vec3(0.0f, 1.0f, 0.0f);



		vec3 axis = normalize(cross(localUp,gravityUp));

		if (wow > 0) {
			return;
		}
		cout << "INDEX: " <<wow << endl;

		wow++;
		cout << "localUP:\t" << to_string(localUp) << endl;
		//cout << "localLeft:\t" << to_string(localLeft) << endl;
		//cout << "localRight:\t" << to_string(localRight) << endl;
		cout << "gravityUp:\t" << to_string(gravityUp) << endl;
		cout <<"Axis:\t" << to_string(axis) << endl;


		
		//float phi = angle(localUp, gravityUp);
		float dotPhi = dot(localUp, gravityUp);
		float phi = acos(dotPhi);


		float rad = phi / 2 ;
		cout <<"Phi: " <<phi << endl;
		//exit(0);

		axis *= sin(rad);

		dQuaternion targetQ = { cos(rad),axis[0],axis[1],axis[2] };
		//dQuaternion targetQ = { cos(rad),axis.x,axis.y,axis.z };


		const dReal* qr = dBodyGetQuaternion(mainEntity->components[1]->GetRigidBodyID());
		cout << "q------r: " << "\t" << qr[0] << "\t" << qr[1] << "\t" << qr[2] << "\t" << qr[3] << endl;
		dQuaternion calculatedQ = {
			{ targetQ[0] * qr[0] - targetQ[1] * qr[1] - targetQ[2] * qr[2] - targetQ[3] * qr[3]},
			{ targetQ[0] * qr[1] + targetQ[1] * qr[0] + targetQ[2] * qr[3] - targetQ[3] * qr[2]},
			{ targetQ[0] * qr[2] - targetQ[1] * qr[3] + targetQ[2] * qr[0] + targetQ[3] * qr[1]},
			{ targetQ[0] * qr[3] + targetQ[1] * qr[2] - targetQ[2] * qr[1] + targetQ[3] * qr[0]}
		};

		cout << "calculatedQ: " << "\t"<< calculatedQ[0] << "\t" << calculatedQ[1] << "\t" << calculatedQ[2] << "\t" << calculatedQ[3] << endl;

		if (!isnan(calculatedQ[1])) {

			dBodySetQuaternion(mainEntity->components[1]->GetRigidBodyID(), calculatedQ);

			vec3 localUp = vec3(mainEntity->components[1]->GetRigidBodyTrans()[0][1], mainEntity->components[1]->GetRigidBodyTrans()[1][1], mainEntity->components[1]->GetRigidBodyTrans()[2][1]);
			
			//vec3 localUp = vec3(0.0f, 1.0f, 0.0f);


			const dReal* qrt = dBodyGetQuaternion(mainEntity->components[1]->GetRigidBodyID());
			cout << "q_____rt: " << "\t" << qrt[0] << "\t" << qrt[1] << "\t" << qrt[2] << "\t" << qrt[3] << endl;

			cout << "localUP:\t" << to_string(localUp) << endl;

			//exit(0);

			cout << "\n\n" << endl;
			return;
		}

		cout << "OW" << endl;

		
	}

}

