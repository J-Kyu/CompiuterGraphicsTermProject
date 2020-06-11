#pragma once
#include "Graphic.h"
class Rect2D :public Graphic {
public:
	Rect2D() {};

protected:

	//virtual void InitGraphic() override final;
	virtual void RenderGraphic() override final;
	virtual void CalVertices() override final;
	virtual void BindElements() override final;
private:
	void get_rect_2d(float, float);
	float r;
	int subh;
	int suba;
};

