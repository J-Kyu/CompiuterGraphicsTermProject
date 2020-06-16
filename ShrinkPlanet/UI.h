#pragma once
#include "EmptyObject.h"
//#include "Graphic.h"
#include "Rect2D.h"
class UI
{

public:
	UI() {
	
	}

	static UI* GetInstance();

	void Activate(mat4 p, mat4 v, int color_mode, vec3,vec3);
	void PopScoreBoard();
	void GenerateScoreBoard();
	void IsSatelliteOn(bool);
private:

	static UI* instance;
	bool isSatellite = false;

	mat4 objectT;
	vector<EmptyObject*> scoreBoards;


};

