#include "Cylinder.h"

void Cylinder::CalVertices() {
	get_cylinder_3d(vertices, side_idx, top_idx, bottom_idx);
}

void Cylinder::get_cylinder_3d(vector<GLfloat>& p, vector<size_t>& side_idx, vector<size_t>& top_idx, vector<size_t>& bottom_idx ) {

	GLfloat half_height = height / 2;
	GLfloat theta, x, z;
	//p.resize(3 * (2 * n + 4));
	p.resize((2 * (n + 2) + 2 * (n + 1)) * 3);
	normals.resize((2*(n+2)+2*(n+1))*3);

	FPUSH_VTX3_AT(p,0, 0, half_height, 0);
	FPUSH_VTX3_AT(normals, 0, 0, 1.0f, 0);
	top_idx.push_back(0);

	FPUSH_VTX3_AT(p, n + 2 + 2 * (n + 1), 0, -half_height, 0);
	FPUSH_VTX3_AT(normals, n + 2 + 2 * (n + 1), 0, -1.0f, 0);
	bottom_idx.push_back((n + 2) + 2 * (n + 1));
	


	for (int i = 0; i <= n; i++) {

		theta = (GLfloat)(2.0 * M_PI * i / n);
		x = radius * sin(theta);
		z = radius * cos(theta);

		//top
		FPUSH_VTX3_AT(p, i + 1, x, half_height, z);
		FPUSH_VTX3_AT(normals, i+1,0, 1 ,0);
		top_idx.push_back(i+1);

		//side
		FPUSH_VTX3_AT(p,n+2+2*i, x, half_height, z);
		FPUSH_VTX3_AT(p, n + 3 + 2 * i, x, -half_height, z);

		FPUSH_VTX3_AT(normals, (n+2)+2*i, x / radius, 0, z / radius);
		FPUSH_VTX3_AT(normals, (n + 3) + 2 * i, x / radius, 0, z / radius);

		side_idx.push_back((n+2)+2*i);
		side_idx.push_back((n + 3) + 2*i);

		//bottom
		FPUSH_VTX3_AT(p, (2 * (n + 2) + 2 * (n + 1))-1 -i, x, -half_height, z);
		FPUSH_VTX3_AT(normals, (2 * (n + 2) + 2 * (n + 1)) - 1 - i, 0, -1, 0);
		//FPUSH_VTX3_AT(normals, n + 2 + 2 * (n + 1) + 1 + i, 0, -1, 0);
		bottom_idx.push_back((n + 2) + 2 * (n + 1)+1+i);


	}
}



void Cylinder::BindElements() {

	/*
		3 elements array buffer is neccessary to draw cone
	*/

	element_buffs = new GLuint[3];

	glGenBuffers(3, element_buffs);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(size_t) * side_idx.size(), side_idx.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(size_t) * top_idx.size(), top_idx.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(size_t) * bottom_idx.size(), bottom_idx.data(), GL_STATIC_DRAW);

}



void Cylinder::RenderGraphic() {


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[0]);
	glDrawElements(drawing_mode[0], side_idx.size(), GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[1]);
	glDrawElements(drawing_mode[1], top_idx.size(), GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[2]);
	glDrawElements(drawing_mode[2], bottom_idx.size(), GL_UNSIGNED_INT, NULL);

}
