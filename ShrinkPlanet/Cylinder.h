#pragma once
#include "Graphic.h"

class Cylinder :public Graphic
{

	public:
		Cylinder() {
			cout << "Cylinder Constructor " << endl;

			this->radius = 1.0f;
			this->height = 3.0f;
			this->n = 36;
		}
		Cylinder(GLfloat radius, GLfloat height, GLint n) {
			cout << "Cylinder Constructor " << endl;

			this -> radius = radius;
			this -> height = height;
			this -> n = n;
		}
	protected:
		virtual void RenderGraphic() override;
		virtual void CalVertices() override;
		virtual void BindElements() override;
	private:
		GLuint drawing_mode[3] = { GL_TRIANGLE_STRIP,GL_TRIANGLE_FAN,GL_TRIANGLE_FAN };

		vector<size_t> side_idx;
		vector<size_t> bottom_idx;
		vector<size_t> top_idx;

		GLfloat radius;
		GLfloat height;
		GLint n;

		void get_cylinder_3d(vector<GLfloat>& p, vector<size_t>& side_idx, vector<size_t>& top_idx, vector<size_t>& bottom_idx);
};