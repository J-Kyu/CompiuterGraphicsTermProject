#version 430

out vec4 FragColor;	

in vec4 fColorFront;
in vec4 fColorBack;

in vec4 fColor;
in vec4 fPosition;	
in vec4 fNormal;
in vec2 fTexcoord;

layout(location=1) uniform mat4 T;
layout(location=2) uniform int mode;
layout(location=3) uniform vec4 uColor;

uniform int ShadingMode;
uniform mat4 M;
uniform mat4 P;
uniform mat4 V;
uniform mat4 U;

uniform vec3 Ia;
uniform vec3 Il;
uniform vec3 Ka;
uniform vec3 Ks;
uniform vec3 Kd;
uniform vec3 c;
uniform float n;
uniform vec4 LightPos_wc;
uniform int ColorMode;

uniform int ObjectCode;// object code
uniform sampler2D sampler;// sampler2D id

vec4 shading(vec3 LightPos_ec, vec3 vPosition_ec, vec3 vNormal_ec)
{
	vec3 N = normalize(vNormal_ec);
	vec3 L = LightPos_ec - vPosition_ec;
	float d = length(L); L = L/d;
	vec3 V = normalize(vec3(0.0) - vPosition_ec);
	vec3 R = reflect(-L, N);

	float fatt = min(1.0 / (c[0] + c[1]*d + c[2]*d*d), 1.0);

	float cos_theta = max(dot(N,L),0);
	float cos_alpha = max(dot(V,R),0);

	vec3 I = Ia * Ka + fatt * Il * (Kd * cos_theta + Ks * pow(cos_alpha, n))/**vec3(vColor)*/;
	//vec3 I = Ia * Ka + fatt * Il * (Kd * cos_theta);

	return vec4(I,1);
}

void default_shading()
{
	FragColor = fColor;	//fragment is rendered with given color
}

void gouraud_shading()
{
	/*
		//fragment is rendered with light intensity calculated from viewing.vert
		//calculate intesity of shader and interpolate color
	*/
	
	if (ColorMode == 1){
		FragColor = vec4(0,0,1,1);
	}
	else if(ColorMode == 2) {
		float nc = ObjectCode / 255.0;
		FragColor = vec4(nc, nc, nc, 1);
	}
	else if( ColorMode == 7){
		if( gl_FrontFacing ) {
			FragColor = fColorFront;
		}
		else {
			FragColor = fColorBack;
		}

	}
	else {
		if( gl_FrontFacing ) {
			FragColor = fColorFront;
		}
		else {
			FragColor = fColorBack;
		}

		FragColor *= texture(sampler, fTexcoord);
	}
}

void phong_shading()
{
	mat4 VMT = V*M*T;
	mat4 U = transpose(inverse(VMT));
	vec3 fNormal_ec = vec3(normalize(U*fNormal));	//calculate fNormal Vector form eye coordinate
	vec3 fPosition_ec = vec3(VMT * fPosition); //calculate position of vertex from eye coordinate
	vec3 LightPos_ec = vec3(V * LightPos_wc);	//light-position form eye coordinate

	if (ColorMode == 1){
		FragColor = vec4(0,0,1,1);
	}
	else if(ColorMode == 2) {
		float nc = ObjectCode / 255.0;
		FragColor = vec4(nc, nc, nc, 1);
	}
	else if( ColorMode == 7){
		FragColor = shading(LightPos_ec, vec3(fPosition_ec), vec3(fNormal_ec));	//intensity of light
	}
	else {
		FragColor = shading(LightPos_ec, vec3(fPosition_ec), vec3(fNormal_ec));	//intensity of light
		FragColor *= texture(sampler, fTexcoord);
	}
}

void main()
{


	switch(ShadingMode){
		case 0:{
			//phong shading
			phong_shading();
			break;
		}
		case 1:{
			//goraud
			gouraud_shading();
			break;
		}
		case 2:{
			//default
			FragColor = fColor;
			break;
		}
	}
	
	/*switch(2)
	{
	case 0:
		default_shading();
		break;
	case 1:
		gouraud_shading();
		break;
	case 2:
	
		phong_shading();
		break;
	case 3:
		FragColor = uColor; 	//for wireframe
		break;
	}
	*/
}