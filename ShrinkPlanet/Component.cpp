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
