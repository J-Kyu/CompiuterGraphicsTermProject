#pragma once
#include "Graphic.h"

class Sphere :public Graphic
{
public:
	Sphere() {
		cout << "Sphere Constructor " << endl;
		this->r = 1.0f;
		this->subh = 16;
		this->suba = 16;
	}
	Sphere(float r, int subh, int suba) {
		cout << "Sphere Constructor " << endl;
		this->r = r;
		this->subh = subh;
		this->suba = suba;
	}
protected:

	//virtual void InitGraphic() override;
	virtual void RenderGraphic() override final;
	virtual void CalVertices() override final;
	virtual void BindElements() override final;
private:
	void get_sphere_3d(vector<GLfloat>& p);
	float r;
	int subh;
	int suba;
};

