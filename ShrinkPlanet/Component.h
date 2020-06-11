#pragma once
#include <time.h> 

#include <GL/glew.h> 
#include <GL/glut.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h> 
#include <iostream>
#include <vector>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "LoadShaders.h"
#include "loadobj.h"
#include "tiny_obj_loader.h"
#define dDOUBLE
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
//#include "Graphic.h"

using namespace tinyobj;

using namespace std;
using namespace glm;

class Component {

public:

	Component() {};

	int objectCode;
	void SetTopT(mat4*);

	virtual dGeomID GetRigidGeomID();
	virtual dBodyID GetRigidBodyID();
	virtual mat4 GetRigidBodyTrans();
	
	virtual void RotateRigidBody(float,vec3);
	virtual void SetRigidBodyTrans(mat4);

	virtual void ActivateComponent(mat4);
	virtual void ActivateComponent(int,mat4,mat4,mat4);
	virtual void InitComponent();

	virtual void CopyGraphic(Component*);
	virtual void TurnOnObjValid();
	virtual void LoadTexture(const char* basedir , const vector<real_t>& texcoords, GLint min_filter= GL_LINEAR_MIPMAP_LINEAR, GLint mag_filter= GL_LINEAR_MIPMAP_LINEAR);
	virtual void LoadObj(const char* filename, const char* basedir, attrib_t& attrib, real_t scale);
	virtual vector<tinyobj::real_t> GetVertices();
	virtual void SetKinematic(bool);

	virtual void CheckCollision();



	vector<tinyobj::real_t> vertices;
	vector<GLfloat> colors;
	vector<GLfloat> normals;
	vector<vector<size_t>> vertex_map;
	vector<vector<size_t>> material_map;
	vector<shape_t> shapes;
	vector<material_t> materials;
	vector<real_t> texcoords;
	map<string, size_t> texmap;
protected:
	mat4* topT = new mat4(1.0f);
};

