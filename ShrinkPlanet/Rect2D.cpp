#include "Rect2D.h"

void Rect2D::CalVertices() {
	get_rect_2d(width,height);
}


void Rect2D::get_rect_2d(float width,float height) {

	GLfloat w2 = width / 2;
	GLfloat h2 = height / 2;


	FPUSH_VTX3(vertices, -w2,-h2,0);
	FPUSH_VTX3(vertices, +w2, -h2,0);
	FPUSH_VTX3(vertices, -w2, +h2,0);


	FPUSH_VTX3(vertices, +w2, -h2,0);
	FPUSH_VTX3(vertices, +w2, +h2,0);
	FPUSH_VTX3(vertices, -w2, +h2,0);
}


void Rect2D::BindElements() {
	/*
		Elements is not necessary for this object
	*/
}

void Rect2D::RenderGraphic() {
	//cout << "Draw: "<< vertices.size() <<"\t"<< colors.size() << endl;
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

