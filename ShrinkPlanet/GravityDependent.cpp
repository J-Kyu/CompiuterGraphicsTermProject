#include "GravityDependent.h"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>
#define _USE_MATH_DEFINES


void GravityDependent::Activate() {
	mainEntity->Activate();
}

void GravityDependent::Activate(mat4 p, mat4 v, int color_mode) {

	if (attractor == NULL) {
		cout << "Attracor is not assigned   !" << endl;
		exit(0);
	}
	CalculateRigidbody();
	mainEntity->SetPerspectiveMatrix(p);
	mainEntity->SetViewMatrix(v);
	mainEntity->Activate(color_mode);
	//apply rigid body topT*rdT
}

void GravityDependent::CalculateRigidbody() {

	//set rigidbody transform based on EO topT

	mainEntity->SetObjectT(mainEntity->components[1]->GetRigidBodyTrans());


	cout << to_string(mainEntity->GetObjectT()) << endl;
	mat4 mainEOT = mainEntity->GetObjectT();
	mat4 attEOT = attractor->GetObjectT();

	vec3 attractorPos = vec3(attEOT[3][0], attEOT[3][1], attEOT[3][2]);
	vec3 depedentPos = vec3(mainEOT[3][0], mainEOT[3][1], mainEOT[3][2]);
	

	vec3 gravityUp = vec3(depedentPos[0]- attractorPos[0], depedentPos[1] - attractorPos[1], depedentPos[2] - attractorPos[2]);
	
	gravityUp = normalize(gravityUp);


	if (!isnan(gravityUp.x)) {

		
	
		
		//from to
		/*
			Quaternion targetRotation = Quaternion.FromToRotation(localUp,gravityUp) * body.rotation;
			//body.rotation = Quaternion.Slerp(body.rotation, targetRotation,500f*Time.deltaTime);
			body.rotation = targetRotation;
			//https://stackoverflow.com/questions/51549366/what-is-the-math-behind-fromtorotation-unity3d
			//https://stackoverflow.com/questions/50081475/opengl-local-up-and-right-from-matrix-4x4
			//https://answers.unity.com/questions/1668856/whats-the-source-code-of-quaternionfromtorotation.html
			quaternion (w,x,y,z) 순이다...그리고 cross으로 인해서 axis가 음수가 될 수 있는데.....이거 주의하자!
		*/

		vec3 localUp = vec3(mainEOT[0][1], mainEOT[1][1], mainEOT[2][1]);


		vec3 axis = normalize(cross(localUp,gravityUp));


		
		//float phi = angle(localUp, gravityUp);
		float dotPhi = dot(localUp, gravityUp);
		float phi = acos(dotPhi);


		float rad = phi / 2 ;

		axis *= sin(rad);

		dQuaternion targetQ = { cos(rad),axis[0],axis[1],axis[2] };


		//const dReal* qr = dBodyGetQuaternion(mainEntity->components[1]->GetRigidBodyID());

		quat qr = quat_cast(mainEOT);




		dQuaternion calculatedQ = {
			{ targetQ[0] * qr[0] - targetQ[1] * qr[1] - targetQ[2] * qr[2] - targetQ[3] * qr[3]},
			{ targetQ[0] * qr[1] + targetQ[1] * qr[0] + targetQ[2] * qr[3] - targetQ[3] * qr[2]},
			{ targetQ[0] * qr[2] - targetQ[1] * qr[3] + targetQ[2] * qr[0] + targetQ[3] * qr[1]},
			{ targetQ[0] * qr[3] + targetQ[1] * qr[2] - targetQ[2] * qr[1] + targetQ[3] * qr[0]}
		};

		dBodySetQuaternion(mainEntity->components[1]->GetRigidBodyID(), calculatedQ); //--> 이건 eo 의 rotation에 정립
		//gravity
		double gravityCoe = -13;
		dBodyAddForce(mainEntity->components[1]->GetRigidBodyID(), (dReal)gravityUp.x * gravityCoe, (dReal)gravityUp.y * gravityCoe, (dReal)gravityUp.z * gravityCoe);

		//mat4 rdT = mainEntity->components[1]->GetRigidBodyTrans();
		//cout << to_string(rdT * mainEOT) << endl;

		//mainEntity->SetObjectT(rdT);
		//mainEntity->components[1] -> SetRigidBodyTrans(mainEntity->GetObjectT());
		//apply to rigd body

		 

		//vec3 forward = (float)RigidBodyWorld::dsElapsedTime() * normalize(vec3(rdT[2][0], rdT[2][1], rdT[2][2]));
		//dBodySetPosition(mainEntity->components[1]->GetRigidBodyID(), localOri[0] + forward.x, localOri[1] + forward.y, localOri[2] + forward.z);


		
	}
	else {
		cout << "gravityUp.x nan " << endl;
	}
}


