#pragma once
#include "Component.h"
#include "Cylinder.h"

class Axis {
public:
	Axis() {}
	Axis(Cylinder* cy) {
		axis = cy;
		axis->InitGraphic();
	}

	Graphic* axis;

	mat4 perspectiveT = mat4(1.0f);
	mat4 viewT = mat4(1.0f);
	mat4 objectT = mat4(1.0f); //initinal matrix
	mat4 rotateT = mat4(1.0f); //real time rotating matrix

};

class Coordinate :	public Component {

public:
		Coordinate() { 
			InitCoordinate();
		}


		virtual void ActivateComponent(mat4) override final;
		virtual void ActivateComponent(int, mat4, mat4, mat4) override final;


		Axis* xAxis;
		Axis* yAxis;
		Axis* zAxis;
private:
	void InitCoordinate();
};



