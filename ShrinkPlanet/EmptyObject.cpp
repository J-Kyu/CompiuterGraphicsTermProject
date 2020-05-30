#include "EmptyObject.h"


void EmptyObject::Init() {
	for (int i = 0; i < components.size(); i++) {
		components[i]->InitComponent();
	}

	for (int i = 0; i < children.size(); i++) {
		children[i]->Init();
	}
}

void EmptyObject::Activate(int colorMode){
	/*
	Top paretn Activate
	*/
	for (int i = 0; i < components.size(); i++) {
		components[i]->ActivateComponent(colorMode,perspectiveT,viewT,objectT* rotateT);
	}
	for (int i = 0; i < children.size(); i++) {
		children[i]->Activate(colorMode,perspectiveT,viewT,objectT*rotateT);
	}

}



void EmptyObject::Activate(int colorMode,mat4 p,mat4 v, mat4 parentT) {
	/*
	Chil Object Activate
	*/
	for (int i = 0; i < components.size(); i++) {
		/*components[i]->ActivateComponent(*objectT);*/
		components[i]->ActivateComponent(colorMode,p, v, parentT * objectT * rotateT);
	}
	for (int i = 0; i < children.size(); i++) {
		children[i]->Activate(colorMode,p,v,parentT * objectT * rotateT * objectT);
	}
}


void EmptyObject::Activate(mat4 parentT) {
	mat4 I(1.0f);
	Activate(0,I, I, parentT);

}

void EmptyObject::AddComponent(Component* cmp) {
	components.push_back(cmp);
}

void EmptyObject::AddChildren(EmptyObject* eo) {
	children.push_back(eo);
}

void EmptyObject::MoveObject(vec3 pos) {
	(objectT) = translate(objectT, pos);

}

void EmptyObject::RotateObject(float angle,vec3 axis) {
	GLfloat radian = M_PI / 180;
	(objectT) = rotate(objectT, radian*angle, axis);
}

void EmptyObject::RotatingYAxis() {
	rotateT = mat4(1.0f);
	GLfloat radian = M_PI / 180;
	mat4 T(1.0f);
	srand(clock());

	float theta = 0.001 * clock();

	rotateT = rotate(rotateT, theta, vec3(0.0f,1.0f,0.0f));
}

void EmptyObject::RotatingYAxis(float angle) {

	rotateT = mat4(1.0f);
	GLfloat radian = M_PI / 180;
	srand(clock());
	//std::cout << glm::to_string(objectT) << std::endl;
	float theta = 0.001 * clock();

	rotateT = rotate(rotateT, theta*angle, vec3(0.0f, 1.0f, 0.0f));
}


void EmptyObject::SetPerspectiveMatrix(mat4 T) {
	perspectiveT =  T;
}

void EmptyObject::SetViewMatrix(mat4 T) {
	viewT = T;
}

