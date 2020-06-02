#pragma once
#include "Graphic.h"


class Cube : public Graphic {
	public:
		Cube() {
			cout << "Cube Constructor " << endl;
			lx = 1.0f;
			ly = 1.0f;
			lz = 1.0f;
		}
		Cube(GLfloat lx, GLfloat ly, GLfloat lz) {
			cout << "Cube Constructor " << endl;
			this -> lx = lx;
			this -> ly = ly;
			this -> lz = lz;
		}
	protected:
		virtual void BindElements() override;
		virtual void CalVertices() override;
		virtual void RenderGraphic() override;
	private:
		GLfloat lx;
		GLfloat ly;
		GLfloat lz;
		void get_box_3d(vector<GLfloat>& p);
};

