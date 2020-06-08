#include "GravityAttractor.h"


void GravityAttractor::Activate() {
	mainEntity->Activate();
}

void GravityAttractor::Activate(mat4 p, mat4 v, int color_mode) {


	mainEntity->SetPerspectiveMatrix(p);
	mainEntity->SetViewMatrix(v);
	//mainEntity->rigidbody->CheckCollision();
	mainEntity->Activate(color_mode);
}