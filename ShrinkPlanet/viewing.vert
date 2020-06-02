#version 430

in vec4 vPosition;
in vec4 vNormal;
in vec4 vColor;
in vec2 vTexcoord; // texture coordinates

out vec4 fColor;
out vec4 fNormal;
out vec4 fPosition;

out vec4 fColorFront;
out vec4 fColorBack;
out vec2 fTexcoord;

uniform mat4 T;	// Additional transformation performed before modeling transformation
uniform int ColorMode;
uniform int ShadingMode;
uniform mat4 M;
uniform mat4 P;
uniform mat4 V;
// uniform mat4 U;
// uniform mat4 VM;

//I=IaKa+cIl(Kdcos(theta)+Kscos^n(alpha))
uniform vec3 Ia = vec3(1.0,1.0, 1.0); //ambient light intensity
uniform vec3 Il = vec3(1.0,1.0,1.0); //incident light intensity
uniform vec3 Ka; //ambient coefficient
uniform vec3 Ks = vec3(0.8,0.8,0.8); //specular coefficent
uniform vec3 Kd; //diffuse coefficients
uniform vec3 c; //attenutation factor of f_{att}
uniform float n = 1.0; //shiningness factor
uniform vec4 LightPos_wc = vec4(10, 10, 3, 1);	//light position with respect to the world coordinate system

vec4 shading(vec3 LightPos_ec, vec3 vPosition_ec, vec3 vNormal_ec)
{
	/*
		*_ec: coordinate from eye-coordinate system
		shading function is to get intensity of light
	*/
	vec3 N = normalize(vNormal_ec);
	vec3 L = LightPos_ec - vPosition_ec;
	float d = length(L); L = L/d; //L becomes unit vector
	vec3 V = normalize(vec3(0.0) - vPosition_ec); //origin of eye position is 0, V is view Position
	vec3 R = reflect(-L, N);	//reflection vector of L by N

	float fatt = min(1.0 / (c[0] + c[1]*d + c[2]*d*d), 1.0);

	float cos_theta = max(dot(N,L),0);	//cos an angle of L and N or N and R
	float cos_alpha = max(dot(V,R),0);	//cos an alpha of angle of V and R
	//if cos_theta is smaller than 0, it means the light does not reach the vertex. thus we set the value to 0 (it barely touches the surface)
	//also if cos_alpha is smaller than 0, it means, our view point(eye coordinate) cannot see the vertex, thus set the value to 0 (at least we are looking at it horizontally)

	vec3 I = Ia * Ka + fatt * Il * (Kd * cos_theta + Ks * pow(cos_alpha, n))/**vec3(vColor)*/;
	//vec3 I = Ia * Ka + fatt * Il * (Kd * cos_theta);


	return vec4(I,1);
}

void default_shading()
{
	//gl_Position = P*V*M*T * vPosition;
	gl_Position = P*V* M *T * vPosition;
	fColor = vColor;
}

void gouraud_shading()
{
	/*
		renders a polygon surface by linear interpolating intensity value acroos the surface	
	*/

	mat4 VMT = V*M*T;
	// this is trasformation of VMT for normal vector (we show VMT in normal vector)
		//n' = (VMT^-1)^T*n
	mat4 U = transpose(inverse(VMT));
	vec3 vNormal_ec = vec3(normalize(U*vNormal));

	vec3 vPosition_ec = vec3(VMT * vPosition);	
	vec3 LightPos_ec = vec3(V * LightPos_wc);

	gl_Position = P * vec4(vPosition_ec, 1);	//gl_Position is clipped Coordinate

	fColorFront = shading(LightPos_ec, vPosition_ec,vNormal_ec);	 //front shaidng
	fColorBack = shading(LightPos_ec,vPosition_ec,-vNormal_ec);		//back shading
	fColor = vColor;
	fTexcoord = vTexcoord;
}

void phong_shading()
{
	/*
		Interpolate the normal vector and then apply the illuminateion modle to each surface point
	*/

	//this part will be done in fragment........not necessary...
	mat4 VMT = V*M*T;
	mat4 U = transpose(inverse(VMT));
	vec3 vNormal_ec = vec3(normalize(U*vNormal));
	vec3 vPosition_ec = vec3(VMT * vPosition);
	vec3 LightPos_ec = vec3(V * LightPos_wc);

	gl_Position = P * vec4(vPosition_ec, 1);	//set gl_Position

	fNormal = vNormal; //save novrmal vector vertex for fragment
	fPosition = vPosition;	//save position vertex for fragment
	fTexcoord = vTexcoord;
}

void main()
{
	fColor = vColor;


	switch(ShadingMode){
		case 0:{
			//phong shading
			phong_shading();
			break;
		}
		case 1:{
			//default
			gl_Position = V*M*T*vPosition;
			fColor = vColor;
			break;
		}
	}


	switch(2)
	{
	case 0:	// use vertex color data
	case 3:	// use constant user color uColor
		default_shading();
		break;
	case 1:
		gouraud_shading();
		break;
	case 2:
		phong_shading();
		break;
	}
}