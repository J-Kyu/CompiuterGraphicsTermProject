#pragma once
#include "Component.h"

#define FPUSH_VTX3(p,vx,vy,vz) do{p.push_back(vx); p.push_back(vy);p.push_back(vz);}while(0);
#define FSET_VTX3(vx,vy,vz,valx,valy,valz)do{vx=(float)valx;vy=(float)(valy);vz=(float)(valz);}while(0);
#define FPUSH_VTX3_AT(p,i,vx,vy,vz) do{size_t i3 = 3*(i);p[i3+0]=(float)(vx); p[i3+1]=(float)(vy);p[i3+2]=(float)(vz);}while(0);


#define MAP_FIND(map_obj, item)\
((map_obj).find(item) != (map_obj).end())

#define IS_MIPMAP(flag) \
((flag) == GL_LINEAR_MIPMAP_LINEAR || \
(flag) == GL_LINEAR_MIPMAP_NEAREST || \
(flag) == GL_NEAREST_MIPMAP_LINEAR || \
(flag) == GL_NEAREST_MIPMAP_NEAREST)

class Graphic : public Component{
	/*
		this shoud be absract class
		such class like sphere should be assigned to Component variable on EmptyObject
	*/
public:
	Graphic(){};


	bool is_obj_valid = false;
	bool is_tex_valid = false;

	void SetColor(GLfloat, GLfloat, GLfloat);
	virtual void InitGraphic();
	virtual void ActivateComponent(mat4) override final;
	virtual void ActivateComponent(int, mat4, mat4, mat4) override final;


protected:
	virtual void InitComponent() override final;

	virtual void RenderGraphic();

	virtual void CalVertices();
	virtual void BindElements();
	virtual void LoadTexture(const char* basedir, const vector<real_t>& texcoords, GLint min_filter= GL_LINEAR_MIPMAP_LINEAR, GLint mag_filter= GL_LINEAR_MIPMAP_LINEAR) override final;
	virtual void LoadObj(const char* filename, const char* basedir, attrib_t& attrib, real_t scale)override final;
	virtual vector<tinyobj::real_t> GetVertices() override final;
	GLuint* element_buffs;
	GLuint vao;
	GLuint vbo[4];

	vector<tinyobj::real_t> vertices;
	vector<GLfloat> colors;
	vector<GLfloat> normals;
	vector<vector<size_t>> vertex_map;
	vector<vector<size_t>> material_map;
	vector<shape_t> shapes;
	vector<material_t> materials;
	vector<real_t> texcoords;
	map<string, size_t> texmap;

private:
	int program;

	int BuildProgram();

	void CalColor(vector<GLfloat>& c, vector<GLfloat>& p);

	void SetShaderValue(int, const char*, mat4);
	void RegisterVertexArray();
	void BindCBuffersCV();
	void BindBuffer(GLint buffer, vector<GLfloat>& vec, int program, const GLchar* attri_name, GLint attri_size);
	bool LoadTexture(const char* basedir, vector<real_t>& texcoords_out, map<string, size_t>& texmap_out, const vector<real_t>& texcoords, const vector<shape_t>& shapes, const vector<material_t>& materials, GLint min_filter, GLint mag_filter);

	void ActivateGraphic(mat4 p, mat4 v, mat4 m);

	static bool has_file(const char* filepath);
	GLuint generate_tex(const char* tex_file_path, GLint min_filter, GLint mag_filter);
};

