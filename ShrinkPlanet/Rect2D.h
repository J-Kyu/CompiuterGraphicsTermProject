#pragma once
#include "Graphic.h"
class Rect2D :public Graphic {
public:
	Rect2D() {
		width = 1.0f;
		height = 0.5f;
		color = vec3(0, 0, 0);
	};
	Rect2D(float width,float height,vec3 color) {
		this->width = width;
		this->height = height;
		this->color = color;
	};

protected:

	//virtual void InitGraphic() override final;
	virtual void RenderGraphic() override final;
	virtual void CalVertices() override final;
	virtual void BindElements() override final;
private:
	void get_rect_2d(float, float);
	float width;
	float height;
	vec3 color;
};

