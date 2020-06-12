#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h> 
#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <GL/glew.h> 
#include <GL/glut.h> 

#include "Component.h"


class EmptyObject {



public:
	EmptyObject(){}

	void Init();
	void Activate(int colorMode  = 0);
	void Activate(mat4);
	void Activate(int,mat4,mat4,mat4);

	void AddChildren(EmptyObject* eo);
	void MoveObject(vec3);
	void RotateObject(float, vec3);
	void RotatingYAxis(float);
	void RotatingYAxis();
	void SetPerspectiveMatrix(mat4);
	void SetViewMatrix(mat4);
	void SetObjectT(mat4);
	mat4 GetObjectT();
	mat4 GetRotateT();

	vector<EmptyObject*> children;

	Component* graphic;
	Component* rigidbody;
	Component* coordinate;


private:
	
	mat4 perspectiveT = mat4(1.0f);
	mat4 viewT = mat4(1.0f);
	mat4 objectT = mat4(1.0f); //initinal matrix
	mat4 rotateT = mat4(1.0f); //real time rotating matrix


};

