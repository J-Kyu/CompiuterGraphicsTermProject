#include "Coordinate.h"



void Coordinate::ActivateComponent(mat4 m) {
	xAxis->axis->ActivateComponent(m);
	yAxis->axis->ActivateComponent(m);
	zAxis->axis->ActivateComponent(m);

}
void Coordinate::ActivateComponent(int colorMode, mat4 p , mat4 v , mat4 m) {
	xAxis->axis->ActivateComponent(colorMode,p,v,m* xAxis->objectT);
	yAxis->axis->ActivateComponent(colorMode, p, v, m* yAxis->objectT);
	zAxis->axis->ActivateComponent(colorMode, p, v, m* zAxis->objectT);
}




void Coordinate::InitCoordinate() {



	GLfloat radian = M_PI / 180;
	GLfloat cylinderHeight = 2.0f;
	GLfloat cylinderRadius = 0.05f;


	mat4 m1(1.0f);

	xAxis = new Axis(new Cylinder(cylinderRadius, cylinderHeight, 8));
	m1 = translate(m1, vec3(cylinderHeight / 2, 0.0f, 0.0f));
	m1 = rotate(m1, (GLfloat)(90 * radian), vec3(0.0f, 0.0f, 1.0f));
	xAxis->objectT = m1;


	m1 = mat4(1.0f);
	zAxis = new Axis(new Cylinder(cylinderRadius, cylinderHeight, 8));
	m1 = translate(m1, vec3(0.0f, 0.0f, cylinderHeight / 2));
	m1 = rotate(m1, (GLfloat)(90 * radian), vec3(1.0f, 0.0f, 0.0f));
	zAxis->objectT = (m1);


	m1 = mat4(1.0f);
	yAxis = new Axis(new Cylinder(cylinderRadius, cylinderHeight, 8));
	m1 = translate(m1, vec3(0.0f, cylinderHeight / 2, 0.0f));
	yAxis->objectT = (m1);



	xAxis->axis->SetColor(1.0f, 0.0f, 0.0f);
	yAxis->axis->SetColor(0.0f, 1.0f, 0.0f);
	zAxis->axis->SetColor(0.0f, 0.0f, 1.0f);

}