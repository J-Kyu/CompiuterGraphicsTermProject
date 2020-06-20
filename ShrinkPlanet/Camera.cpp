#include "Camera.h"



mat4 Camera::GetViewing() {
	if (isSatellite) {
		mat4 m = satellite->GetObjectMatrix();

		vec3 satellEye = vec3(m[3][0], m[3][1], m[3][2] );
		vec3 satellUp = vec3(0,0,1);

		return lookAt(satellEye, center, satellUp);
	}
	else {
		return lookAt(eye, center, up);
	}
}

mat4 Camera::GetProjection(GLfloat aspect) {
	mat4 P(1.0f);

	switch (projectionMode) {
		case ORTHOGRAPHIC: {
			P = Parallel((double)zoomFactor * xRight, aspect, zNear, zFar);
			break;
		}
		case PERSPECTIVE: {
			P = perspective(zoomFactor*fovy, aspect, zNear, zFar);
			break;
		}
	}

	return P;
}

mat4 Camera::Parallel(double r, double aspect, double n, double f) {
	double l = -r;
	double width = 2 * r;
	double height = width / aspect;
	double t = height / 2;
	double b = -t;
	return ortho(l, r, b, t, n, f);
}



void Camera::Motion(int x, int y) {

	int modifiers = glutGetModifiers();
	int is_alt_active = modifiers & GLUT_ACTIVE_ALT;
	int is_ctrl_active = modifiers & GLUT_ACTIVE_CTRL;
	int is_shift_active = modifiers & GLUT_ACTIVE_SHIFT;

	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	GLfloat dx = 1.0f * (x - mouse_pos[0]) / w;
	GLfloat dy = -1.0f * (y - mouse_pos[1]) / h;


	if (button_pressed[GLUT_LEFT_BUTTON] == GLUT_DOWN) {
		if (is_alt_active) {
			vec4 disp(eye - center, 1);

			GLfloat alpha = 2.0f;
			mat4 V = GetViewing();
			mat4 Rx = rotate(mat4(1.0f), alpha * dy, vec3(transpose(V)[0]));
			mat4 Ry = rotate(mat4(1.0f), -alpha * dx, vec3(0, 1, 0));
			mat4 R = Ry * Rx;
			eye = center + vec3(R*disp);
			up = mat3(R) * up;


			
		}
	}


	if (button_pressed[GLUT_MIDDLE_BUTTON] == GLUT_DOWN) {

		if (is_alt_active) {
			mat4 VT = transpose(GetViewing());
			eye += vec3(-dx * VT[0] + -dy * VT[1]);
			center += vec3(-dx*VT[0] + -dy*VT[1]);
		}
	}


	 
	mouse_pos[0] = x;
	mouse_pos[1] = y;
	glutPostRedisplay();
	

}


void Camera::Mouse(int button, int state, int x, int y) {

	button_pressed[button] = state;
	mouse_pos[0] = x;
	mouse_pos[1] = y;

}

void Camera::MouseWheel(int wheel, int dir, int x, int y) {

	int is_alt_active = glutGetModifiers() & GLUT_ACTIVE_ALT;

	if (is_alt_active) {
		vec3 disp = eye - center;
		if (dir > 0) {
			eye = center + 0.95f * disp;
		}
		else {
			eye = center + 1.05f * disp;
		}

	}
	else {
		if (dir > 0) {
			if (zoomFactor > 0.5f){
				zoomFactor *= 0.95f;

			}
		}
		else {
			if (zoomFactor < 4.0f) {
				zoomFactor *= 1.05f;
			}
		}
	}

	glutPostRedisplay();
	
}

void Camera::ViewSatellite(bool is) {
	isSatellite = is;
}


void Camera::ResetEye(){

	this->eye = origEyePos;
	this->center = vec3(0, 0, 0);
	this->up = vec3(0, 1, 0);
	this->zoomFactor = 3.0f;
	this->zNear = 0.01f;
	this->zFar = 100.0f;
	this->fovy = ((float)(M_PI / 180.0 * (30.0f)));
	this->xRight = (1.2f);
}

vec3 Camera::GetEye() {

	if (isSatellite) {
		mat4 m = satellite->GetObjectMatrix();

		return vec3(m[3][0], m[3][1], m[3][2]);
		
	}
	
	return this->eye;
}

vec3 Camera::GetUp() {

	if (isSatellite) {
		mat4 m = satellite->GetObjectMatrix();

		return normalize(vec3(m[1][0],m[1][1],m[1][2]));
	}
	return this->up;
}

void Camera::SwitchProjection() {
	if (projectionMode == ORTHOGRAPHIC) {
		projectionMode = PERSPECTIVE;
		return;
	}

	projectionMode = ORTHOGRAPHIC;
	return;
}