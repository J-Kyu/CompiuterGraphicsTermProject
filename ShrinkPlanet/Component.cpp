#include "Component.h"


void Component::ActivateComponent(mat4) {
	cout << "No Component Found" << endl;
}

void Component::ActivateComponent(int,mat4, mat4, mat4) {
	cout << "No Component Found" << endl;
}


void Component::InitComponent() {
	cout << "No Component Found" << endl;
}

/*
void Component::LoadTexture(const char* basedir, vector<real_t> texcoords_out, map<string, size_t> texmap_out, const vector<real_t> texcoords, const vector<shape_t> shapes, const vector<material_t> materials, GLint min_filter = GL_LINEAR_MIPMAP_LINEAR, GLint mag_filter = GL_LINEAR_MIPMAP_LINEAR) {
	cout << "This object is not Graphic Component....Please use this function on TYPE Graphic" << endl;
}
*/
void Component::LoadTexture(const char* basedir, const vector<real_t>& texcoords, GLint min_filter , GLint mag_filter ) {
	cout << "This object is not Graphic Component....Please use this function on TYPE Graphic" << endl;
}

void Component::LoadObj(const char* filename, const char* basedir, attrib_t& attrib, real_t scale) {
	cout << "This object is not Graphic Component....Please use this function on TYPE Graphic" << endl;
}

vector<GLfloat> Component::GetVertices() {
	vector<GLfloat> temp;
	cout << "This object is not Graphic Component....Please use this function on TYPE Graphic" << endl;
	return  temp;
}

//void Component::SetEO(EmptyObject* eo) {
//	eo = eo;
//}

void Component::SetTopT(mat4* t) {
	topT = t;
}

dBodyID Component::GetRigidBodyID() {

	cout << "This object is not Rigidbody Component....Please use this function on TYPE Rigidbody" << endl;
	return NULL;
}

mat4 Component::GetRigidBodyTrans() {
	cout << "This object is not Rigidbody Component....Please use this function on TYPE Rigidbody" << endl;
	return 	mat4(0);
}

void Component::RotateRigidBody(float angle, vec3 vc) {
	cout << "This object is not Rigidbody Component....Please use this function on TYPE Rigidbody" << endl;
	return;

}

void Component::SetRigidBodyTrans(mat4) {
	cout << "This object is not Rigidbody Component....Please use this function on TYPE Rigidbody" << endl;
}

void Component::CheckCollision() {
	cout << "This is RigidBody World Virtual Clss.....should not be called" << endl;
}


void Component::CopyGraphic(Component*) {
	cout << "This is not Graphic Type Object...Component Type.....should not be called" << endl;
}


void Component::TurnOnObjValid() {
	cout << "This is not Graphic Type Object...Component Type.....should not be called" << endl;
}

void Component::SetKinematic(bool) {
	cout << "This is not Rigidbody Type Object...Component Type.....should not be called" << endl;
}

dGeomID Component::GetRigidGeomID() {

	cout << "This object is not Rigidbody Component....Please use this function on TYPE Rigidbody" << endl;
	return NULL;
}