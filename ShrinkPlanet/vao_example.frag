#version 430
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

in vec4 fColor;
out vec4 FragColor;
layout(location = 2) uniform int draw_mode;

void main(){

	switch(draw_mode){
		case 0:{
			FragColor = fColor;
			break;
		}
		case 1:{
			FragColor = vec4(0,0,0,1);
			break;
		}
	}
}