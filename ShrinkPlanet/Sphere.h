#pragma once
#include "Graphic.h"

class Sphere :public Graphic
{
public:
	Sphere() {
		cout << "Sphere Constructor " << endl;
		this->r = 1.0f;
		this->subh = 8;
		this->suba = 8;
	}
	Sphere(float r, int subh, int suba) {
		cout << "Sphere Constructor " << endl;
		this->r = r;
		this->subh = subh;
		this->suba = suba;
	}
protected:

	//virtual void InitGraphic() override;
	virtual void RenderGraphic() override;
	virtual void CalVertices() override;
	virtual void BindElements() override;
private:
	void get_sphere_3d(vector<GLfloat>& p);
	float r;
	int subh;
	int suba;
};

