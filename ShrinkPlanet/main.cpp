#define dDOUBLE
//#pragma commnet(lib,"winmm.lib");

#define dEpsilon 0.0016
#include <iostream>
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <GL/freeglut.h>
#include <vector>
#include <time.h>
#include "EmptyObject.h"
#include "Sphere.h"
#include "Camera.h"
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <windows.h>
#include <timeapi.h>
#include "glm/gtx/string_cast.hpp"



//#include <Mmsystem.h>


using namespace std;
using namespace glm;

EmptyObject* earthObj;
EmptyObject* moonObj;
EmptyObject* sphereObj;

Component* earthGraphic;
Component* moonGraphic;
Component* sphere;



Camera* mainCamera = new Camera();

static dWorldID ode_world; // simulation world
static dSpaceID ode_space; // collision space
static dJointGroupID ode_contactgroup; // a group of contact joints
static bool pause = false;

static dBodyID ode_gravity_body;
static dGeomID ode_gravity_geom;

static dBodyID ode_sphere_body;
static dGeomID ode_sphere_geom;


static dGeomID ode_plane_geom;

static dBodyID ode_trimesh_body;
static dGeomID ode_trimesh_geom;

static dTriMeshDataID ode_trimesh_data;
static std::vector<dTriIndex> ode_trimesh_index;


void init();
void display();
void Render(int color_mode = 0);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void mouseWheel(int wheel, int dir, int x, int y);
glm::mat4 compute_modelling_transf(dBodyID body);
void ODEInit();
void ODEDisplay();
static void nearCallback(void* data, dGeomID o1, dGeomID o2);


void main(int argc, char** argv)
{

	//Display Setting
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGBA);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutCreateWindow("VAO example");

	//load glew
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	init();
	ODEInit();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMouseWheelFunc(mouseWheel);
	glutMainLoop();

}

void init() {

	sphere = new Sphere();
	earthGraphic = new Graphic();
	moonGraphic = new Graphic();

	earthGraphic->kyu = 1;
	moonGraphic->kyu = 2;
	sphere->kyu = 3;

	earthObj = new EmptyObject(earthGraphic);
	moonObj = new EmptyObject(moonGraphic);
	sphereObj = new EmptyObject(sphere);

	//sphereObj->MoveObject(vec3(1.0f, 2.0f, 0.0f));


	//earthObj->AddChildren(moonObj);



	attrib_t attrib_1;

	earthGraphic->LoadObj("models/earth.obj", "models/", attrib_1, 1.0f);
	glActiveTexture(GL_TEXTURE0);
	earthGraphic->LoadTexture("models/", attrib_1.texcoords);

	attrib_t attrib_2;
	moonGraphic->LoadObj("models/moon.obj", "models/", attrib_2, .3f);
	glActiveTexture(GL_TEXTURE0);
	moonGraphic->LoadTexture("models/", attrib_2.texcoords);

	earthObj->Init();
	moonObj->Init();
	sphereObj->Init();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void ODEInit()
{
	/* ¡¦¡¦ OpenGL initialization ¡¦¡¦ */
	dMatrix3 R;
	dMass m;
	dRSetIdentity(R);

	dInitODE2(0);
	ode_world = dWorldCreate(); // Make a simulation world.
	ode_space = dSimpleSpaceCreate(0); // Make a collision space.
	ode_contactgroup = dJointGroupCreate(0); // Make a contact group.
	//dWorldSetGravity(ode_world, 0, -9.8, 0); // Set gravity.
	//dWorldSetGravity(ode_world, 0, -9.8, 0); // Set gravity.

	// Generate the plane geometry.
	ode_plane_geom = dCreatePlane(ode_space, 0, 1, 0, 0);

	//sphere
	ode_sphere_body = dBodyCreate(ode_world);
	dBodySetPosition(ode_sphere_body, 2, 10, 0);
	dBodySetRotation(ode_sphere_body, R);
	dBodySetLinearVel(ode_sphere_body, 0, 0, 0);
	dBodySetAngularVel(ode_sphere_body, 0, 0, 0);
	ode_sphere_geom = dCreateSphere(ode_space, 0.2f);
	dGeomSetBody(ode_sphere_geom, ode_sphere_body);
	dMassSetSphereTotal(&m, 20, 1.f);
	dBodySetMass(ode_sphere_body, &m);


	/*
	//trim
	ode_trimesh_body = dBodyCreate(ode_world);
	dBodySetPosition(ode_trimesh_body, 0, 1, 0);
	dBodySetRotation(ode_trimesh_body, R);
	dBodySetLinearVel(ode_trimesh_body, 0, 0, 0);
	dBodySetAngularVel(ode_trimesh_body, 0, 0, 0);

	ode_trimesh_geom = dCreateSphere(ode_space, 0.2f);
	dGeomSetBody(ode_trimesh_geom, ode_trimesh_body);
	dMassSetSphereTotal(&m, 20, 1.2f);
	dBodySetMass(ode_trimesh_body, &m);
	*/



	
	//trim
	ode_trimesh_body = dBodyCreate(ode_world);
	dBodySetPosition(ode_trimesh_body, 0, 0, 0);
	dBodySetRotation(ode_trimesh_body, R);
	dBodySetLinearVel(ode_trimesh_body, 0, 0, 0);
	dBodySetAngularVel(ode_trimesh_body, 0, 0, 0);

	int n = (int)(sphere->GetVertices().size() / 3);

	ode_trimesh_index.resize(n);
	for (int i = 0; i < n; ++i) {
		ode_trimesh_index[i] = i;
	}

	ode_trimesh_data = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSingle(ode_trimesh_data, sphere->GetVertices().data(), 3 * sizeof(float), n, ode_trimesh_index.data(), n, 3 * sizeof(dTriIndex));

	ode_trimesh_geom = dCreateTriMesh(ode_space, ode_trimesh_data, 0, 0, 0);
	dGeomSetBody(ode_trimesh_geom, ode_trimesh_body);
	cout << "mass: " << (m.mass) << endl;
	dMassSetTrimeshTotal(&m, 21, ode_trimesh_geom);	//mass  ¾ø´Ù
	dBodySetMass(ode_trimesh_body, &m);	//bug

	
}

void ODEDisplay()
{
	double stepsize = 0.005; // 5ms simulation step size
	double dt = dsElapsedTime();

	int no_of_steps = (int)ceilf(dt / stepsize);
	
	for (int i = 0; !pause && i < no_of_steps; ++i)
	{
		dSpaceCollide(ode_space, 0, &nearCallback);
		dWorldQuickStep(ode_world, stepsize);
		dJointGroupEmpty(ode_contactgroup); // remove all contact joints
	}

	vec3 gravityUp = vec3(moonObj->GetObjectT()[3][0] - earthObj->GetObjectT()[3][0], moonObj->GetObjectT()[3][1] - earthObj->GetObjectT()[3][0], moonObj->GetObjectT()[3][2] - earthObj->GetObjectT()[3][2]);
	
	//gravityUp = normalize(gravityUp);
	double gravityCoe = -1;
	//cout << gravityUp.x << "\t" << gravityUp.y <<"\t" << gravityUp.z << endl;
	dBodyAddForce(ode_sphere_body,gravityUp.x*gravityCoe, gravityUp.y*gravityCoe,gravityUp.z*gravityCoe);

	mat4 Ms = compute_modelling_transf(ode_sphere_body);
	//mat4 Mt = compute_modelling_transf(ode_trimesh_body);

	//earthObj->SetObjectT(Mt);
	sphereObj->SetObjectT(Ms);
	//moonObj->SetObjectT(Ms);
//	cout << glm::to_string(Ms) << endl;
}

glm::mat4 compute_modelling_transf(dBodyID body)
{
	mat4 M(1.0f);
	const dReal* pos = dBodyGetPosition(body);
	const dReal* rot = dBodyGetRotation(body);
	M[3] = vec4(pos[0], pos[1], pos[2], 1.0f);
	for (int i = 0; i < 3; ++i) // for each column
		for (int j = 0; j < 3; ++j) // for each row
			M[i][j] = rot[j * 4 + i];
	return M;
}


static void nearCallback(void* data, dGeomID o1, dGeomID o2)
{
	const int N = 100;
	dContact contact[N];
	int n = dCollide(o1, o2, N, &(contact[0].geom), sizeof(dContact));
	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			contact[i].surface.mode = dContactSoftERP | dContactSoftCFM;
			contact[i].surface.mu = 0.8;
			contact[i].surface.soft_erp = 0.9;
			contact[i].surface.soft_cfm = 0.01;
			dJointID c = dJointCreateContact(ode_world, ode_contactgroup, &contact[i]);
			dBodyID body1 = dGeomGetBody(contact[i].geom.g1);
			dBodyID body2 = dGeomGetBody(contact[i].geom.g2);
			dJointAttach(c, body1, body2);
		}
	}
}

double dsElapsedTime()
{
	static double prev = 0.0;
	double curr =  ::timeGetTime()/ 1000.0;
	if (!prev)
		prev = curr;
	double retval = curr - prev;
	prev = curr;
	if (retval > 1.0) retval = 1.0;
	if (retval < dEpsilon) retval = dEpsilon;
	return retval;
}

void Render(int color_mode) {

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	double aspect = 1.0 * width / height;

	//earthObj->RotatingYAxis(1.0f);
	//moonObj->RotatingYAxis(-3.0f);

	earthObj->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	earthObj->SetViewMatrix(mainCamera->GetViewing());

	moonObj->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	moonObj->SetViewMatrix(mainCamera->GetViewing());

	sphereObj->SetPerspectiveMatrix(mainCamera->GetProjection(aspect));
	sphereObj->SetViewMatrix(mainCamera->GetViewing());

	earthObj->Activate(color_mode);
	moonObj->Activate(color_mode);
	sphereObj->Activate(color_mode);

	if (color_mode != 2) {
		glutSwapBuffers();
	}
}


void display() {
	ODEDisplay();
	Render();

	glFlush();
	glutPostRedisplay();

}

void mouse(int button, int state, int x, int y) {
	mainCamera->Mouse(button, state, x, y);
	Render(2);
	glFlush();

	if (state == GLUT_UP)
	{
		unsigned char res[4];
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		glReadPixels(x, height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
		switch (res[0])
		{
		case 1: printf("The earth is clicked!\n"); break;
		case 2: printf("The moon is clicked!\n"); break;
		}
	}
}

void motion(int x, int y) {
	mainCamera->Motion(x, y);
}


void mouseWheel(int wheel, int dir, int x, int y) {
	mainCamera->MouseWheel(wheel, dir, x, y);
}
