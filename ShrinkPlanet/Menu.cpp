#include "Menu.h"
#include "time.h"

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


		

		startObj->RotatingYAxis(-1.0f);
		endObj->RotatingYAxis(-1.0f);

		startObj->Activate(color_mode);
		endObj->Activate(color_mode);
	}
}
