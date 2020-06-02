#include "Cube.h"

void Cube::CalVertices() {
	get_box_3d(vertices);

	cout << vertices[0] << endl;
}

void Cube::get_box_3d(vector<GLfloat>& p) {
	static const GLfloat box_vertices[] = {
		0.5f, 0.5f,-0.5f,	-0.5f,-0.5f,-0.5f,	-0.5f, 0.5f,-0.5f,
		0.5f, 0.5f,-0.5f,	 0.5f,-0.5f,-0.5f,	-0.5f,-0.5f,-0.5f,
	   -0.5f,-0.5f,-0.5f,	-0.5f,-0.5f, 0.5f,	-0.5f, 0.5f, 0.5f,
	   -0.5f,-0.5f,-0.5f,  -0.5f, 0.5f, 0.5f,	-0.5f, 0.5f,-0.5f,
		0.5f,-0.5f, 0.5f,  -0.5f,-0.5f,-0.5f,	 0.5f,-0.5f,-0.5f,
		0.5f,-0.5f, 0.5f,	-0.5f,-0.5f, 0.5f,	-0.5f,-0.5f,-0.5f,
	   -0.5f, 0.5f, 0.5f,	-0.5f,-0.5f, 0.5f,	 0.5f,-0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,	-0.5f, 0.5f, 0.5f,	 0.5f,-0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,	 0.5f,-0.5f,-0.5f,	 0.5f, 0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,	 0.5f, 0.5f, 0.5f,	 0.5f,-0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,	 0.5f, 0.5f,-0.5f,	-0.5f, 0.5f,-0.5f,
		0.5f, 0.5f, 0.5f,	-0.5f, 0.5f,-0.5f,	-0.5f, 0.5f, 0.5f
	};

	p.resize(sizeof(box_vertices) / sizeof(GLfloat));
	memcpy(p.data(), box_vertices, sizeof(box_vertices));
	size_t n = p.size() / 3;
	for (int i = 0; i < n; i++) {
		p[3 * i + 0] *= lx;
		p[3 * i + 1] *= ly;
		p[3 * i + 2] *= lz;
	}

	normals.resize(n * 3);
	float* cursor = normals.data();

	for (int i = 0; i < 6; i++, cursor += 3) { cursor[0] = 0;	cursor[1] = 0;	 cursor[2] = -1; }
	for (int i = 0; i < 6; i++, cursor += 3) { cursor[0] = -1;	cursor[1] = 0;	 cursor[2] = 0; }
	for (int i = 0; i < 6; i++, cursor += 3) { cursor[0] = 0;	cursor[1] = -1;	 cursor[2] = 0; }
	for (int i = 0; i < 6; i++, cursor += 3) { cursor[0] = 0;	cursor[1] = 0;	 cursor[2] = 1; }
	for (int i = 0; i < 6; i++, cursor += 3) { cursor[0] = 1;	cursor[1] = 0;	 cursor[2] = 0; }
	for (int i = 0; i < 6; i++, cursor += 3) { cursor[0] = 0;	cursor[1] = 1;	 cursor[2] = 0; }
	cout << "CUBE DONE" <<endl;
}

void Cube::BindElements() {
	/*
		Elements is not necessary for this object
	*/
}

void Cube::RenderGraphic() {

	//glUniformMatrix4fv(1, 1, GL_FALSE, trans_mat);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
