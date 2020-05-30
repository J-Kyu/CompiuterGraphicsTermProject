#version 430
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

in vec4 vPosition;
in vec4 vColor;
out vec4 fColor;

void main(){
		gl_Position = P * V * M * vPosition;
	fColor = vColor;
}