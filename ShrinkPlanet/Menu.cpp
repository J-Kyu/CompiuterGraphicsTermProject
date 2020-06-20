#include "Menu.h"
#include "time.h"


Menu* Menu::GetInstance() {
	if (instance == nullptr) {
		instance = new Menu();
	}
	return instance;
}


void Menu::Activate() {

	startObj->Activate();
	endObj->Activate();
}

void Menu::Activate(mat4 p, mat4 v, int color_mode) {

	srand(clock());


	if (isShown == true) {
		startObj->SetPerspectiveMatrix(p);
		startObj->SetViewMatrix(v);

		endObj->SetPerspectiveMatrix(p);
		endObj->SetViewMatrix(v);


		

		startObj->RotatingAxis(vec3(0.0,-1.0f,0.0f));
		endObj->RotatingAxis(vec3(0.0, -1.0f, 0.0f));

		startObj->Activate(color_mode);
		endObj->Activate(color_mode);
	}
}
