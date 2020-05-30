#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h> 
#include <iostream>
#include <vector>
#include "Graphic.h"


class EmptyObject {



public:
	EmptyObject(){}
	EmptyObject(Component* cmp) {
		components.push_back(cmp);
	}

	void Init();
	void Activate(int colorMode  = 0);
	void Activate(mat4);
	void Activate(int,mat4,mat4,mat4);
	void AddComponent(Component* cmp);
	void AddChildren(EmptyObject* eo);
	void MoveObject(vec3);
	void RotateObject(float, vec3);
	void RotatingYAxis();
	void RotatingYAxis(float);
	void SetPerspectiveMatrix(mat4);
	void SetViewMatrix(mat4);
	
private:
	
	mat4 perspectiveT = mat4(1.0f);
	mat4 viewT = mat4(1.0f);
	mat4 objectT = mat4(1.0f); //initinal matrix
	mat4 rotateT = mat4(1.0f); //real time rotating matrix
	vector<Component*> components;
	vector<EmptyObject*> children;

};

