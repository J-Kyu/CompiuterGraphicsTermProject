#include "Sphere.h"


void Sphere::CalVertices() {
	get_sphere_3d(vertices);
}


void Sphere::get_sphere_3d(vector<GLfloat>& p) {

	double theta0;
	double theta1;
	double y0;
	double rst0;
	double y1;
	double rst1;

	double phi0;
	double phi1;

	double cp0;
	double sp0;
	double cp1;
	double sp1;

	float vx0, vy0, vz0, vx1, vy1, vz1;
	float vx2, vy2, vz2, vx3, vy3, vz3;


	for (int i = 1; i <= subh; i++) {

		if (i == 1) {
			//init angles
			theta0 = M_PI * (i - 1) / subh;
			y0 = r * cos(theta0);
			rst0 = r * sin(theta0);
		}
		else {
			//old angles
			theta0 = theta1;
			y0 = y1;
			rst0 = rst1;
		}

		//new angles 
		theta1 = M_PI * i / subh;

		y1 = r * cos(theta1);
		rst1 = r * sin(theta1);

		for (int j = 1; j <= suba; j++) {


			if (j == 1 || j == suba) {
				//init angles & final angle
				phi0 = 2 * (M_PI) * (j - 1) / suba;
				phi1 = 2 * M_PI * j / suba;

				cp0 = cos(phi0);
				sp0 = sin(phi0);
				cp1 = cos(phi1);
				sp1 = sin(phi1);


				FSET_VTX3(vx0, vy0, vz0, sp0 * rst0, y0, cp0 * rst0);
				FSET_VTX3(vx1, vy1, vz1, sp0 * rst1, y1, cp0 * rst1);
				FSET_VTX3(vx2, vy2, vz2, sp1 * rst0, y0, cp1 * rst0);
				FSET_VTX3(vx3, vy3, vz3, sp1 * rst1, y1, cp1 * rst1);

			}
			else {
				//new angles 
				phi1 = 2 * M_PI * j / suba;

				cp1 = cos(phi1);
				sp1 = sin(phi1);
				//resuse old points
				FSET_VTX3(vx0, vy0, vz0, vx2, vy2, vz2);
				FSET_VTX3(vx1, vy1, vz1, vx3, vy3, vz3);
				FSET_VTX3(vx2, vy2, vz2, sp1 * rst0, y0, cp1 * rst0);
				FSET_VTX3(vx3, vy3, vz3, sp1 * rst1, y1, cp1 * rst1);

			}


			if (i < subh) {
				FPUSH_VTX3(p, vx0, vy0, vz0);
				FPUSH_VTX3(p, vx1, vy1, vz1);
				FPUSH_VTX3(p, vx3, vy3, vz3);
				FPUSH_VTX3(normals, vx0 / r, vy0 / r, vz0 / r);
				FPUSH_VTX3(normals, vx1 / r, vy1 / r, vz1 / r);
				FPUSH_VTX3(normals, vx3 / r, vy3 / r, vz3 / r);
			}

			if (1 < i) {
				FPUSH_VTX3(p, vx3, vy3, vz3);
				FPUSH_VTX3(p, vx2, vy2, vz2);
				FPUSH_VTX3(p, vx0, vy0, vz0);
				FPUSH_VTX3(normals, vx3 / r, vy3 / r, vz3 / r);
				FPUSH_VTX3(normals, vx2 / r, vy2 / r, vz2 / r);
				FPUSH_VTX3(normals, vx0 / r, vy0 / r, vz0 / r);
			}
		}
	}
}


void Sphere::BindElements() {
	/*
		Elements is not necessary for this object
	*/
}

void Sphere::RenderGraphic() {
	//cout << "Draw: "<< vertices.size() <<"\t"<< colors.size() << endl;
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}