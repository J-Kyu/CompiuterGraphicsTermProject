#include "UI.h"



UI* UI::GetInstance() {
	if (instance == nullptr) {
		instance = new UI();
	}
	return instance;
}

void UI::Activate(mat4 p, mat4 v, int color_mode, vec3 cameraPos,vec3 cameraUp) {

	vec3 newPos = vec3((0.9) * cameraPos.x, (0.9) * cameraPos.y, (0.9) * cameraPos.z);
	vec3 front = normalize( vec3(newPos.x - cameraPos.x, newPos.y - cameraPos.y,  newPos.z - cameraPos.z ));
	vec3 right = normalize(cross(front, cameraUp));



	for (int i = 0; i < scoreBoards.size(); i++) {
		scoreBoards[i]->SetPerspectiveMatrix(p);
		scoreBoards[i]->SetViewMatrix(v);

		mat4 t(1.0f);


		t[0][0] = right.x;
		t[0][1] = right.y;
		t[0][2] = right.z;


		t[1][0] = cameraUp.x;
		t[1][1] = cameraUp.y;
		t[1][2] = cameraUp.z;



		t[2][0] = front.x;
		t[2][1] = front.y;
		t[2][2] = front.z;

		t = translate(t, vec3(-4, (-4)+(0.6) * (i), -35));



		scoreBoards[i]->SetObjectT(t);
		scoreBoards[i]->Activate(color_mode);

	}

}

void UI::GenerateScoreBoard() {

	EmptyObject* scoreBoard = new EmptyObject();

	Graphic* rect = new Rect2D();
	scoreBoard->graphic = rect;

	scoreBoard->Init();

	scoreBoards.push_back(scoreBoard);
}

void UI::PopScoreBoard() {
	if (scoreBoards.size() > 0) {
		scoreBoards.pop_back();
	}
}

