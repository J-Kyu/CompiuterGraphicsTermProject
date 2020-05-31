#include "Graphic.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/gtx/string_cast.hpp"

void Graphic::ActivateGraphic( mat4 p, mat4 v, mat4 m) {
	mat4 I(1.0f);

	SetShaderValue(program, "M", m);
	SetShaderValue(program, "V", v);
	SetShaderValue(program, "P", p);
	SetShaderValue(program, "T", I);

	glUniform1i(glGetUniformLocation(program, "ColorMode"), 7);

	glUniform1f(glGetUniformLocation(program, "n"), 10.0f);
	glUniform3f(glGetUniformLocation(program, "Ka"), 0.3f, 0.3f, 0.3f);
	glUniform3f(glGetUniformLocation(program, "Kd"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(program, "Ks"), 0.8f, 0.8f, 0.8f);

	RenderGraphic();
}

void Graphic::ActivateComponent(int colorMode,mat4 p, mat4 v, mat4 m) {
	if (!is_obj_valid) {

		//cout << "obj is not valid" << endl;
		ActivateGraphic(p, v, m);
		return;
	}

	mat4 I(1.0f);

	SetShaderValue(program, "M", m);
	SetShaderValue(program, "V", v);
	SetShaderValue(program, "P", p);
	SetShaderValue(program, "T", I);


	//cout << colorMode << endl;

	glUniform1i(glGetUniformLocation(program, "ColorMode"), colorMode);
	glUniform1i(glGetUniformLocation(program, "ObjectCode"), kyu);


	glBindVertexArray(vao);
	auto& _shapes = shapes;
	auto& _materials = materials;
	auto& _vertex_map = vertex_map;
	auto& _material_map = material_map;
	auto& _texmap = texmap;


	for (size_t i = 0; i < _shapes.size(); ++i) {
		for (size_t j = 0; j < _material_map[i].size(); ++j) {

			int m_id = _material_map[i][j];
			//int m_id =-1;
			if (m_id < 0) {
				glUniform1f(glGetUniformLocation(program, "n"), 10.0f);
				glUniform3f(glGetUniformLocation(program, "Ka"), 0.3f, 0.3f, 0.3f);
				glUniform3f(glGetUniformLocation(program, "Kd"), 1.0f, 1.0f, 1.0f);
				glUniform3f(glGetUniformLocation(program, "Ks"), 0.8f, 0.8f, 0.8f);
			}
			else {
				glUniform1f(glGetUniformLocation(program, "n"), _materials[m_id].shininess);
				glUniform3fv(glGetUniformLocation(program, "Ka"), 1, _materials[m_id].ambient);
				glUniform3fv(glGetUniformLocation(program, "Kd"), 1, _materials[m_id].diffuse);
				//glUniform3fv(glGetUniformLocation(program, "Ks"), 1, _materials[m_id].specular);
				glUniform3f(glGetUniformLocation(program, "Ks"), 0.8f, 0.8f, 0.8f);

				auto texitem = _texmap.find(_materials[m_id].diffuse_texname);
				if (texitem != _texmap.end()) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, texitem->second);
					glUniform1i(glGetUniformLocation(program, "sampler"), 0);
				}
			}
			glDrawArrays(GL_TRIANGLES, _vertex_map[i][j], _vertex_map[i][j + 1] - _vertex_map[i][j]);
		}
	}

	//RenderGraphic();
}

void Graphic::ActivateComponent(mat4 t) {
	mat4 I(1.0f);
	ActivateComponent(0,I,I,t);
}

void Graphic::InitComponent() {
	InitGraphic();
}

void Graphic::InitGraphic() {
	/*
		get verttices
		cal color
		cal nomral
	*/
	

	program = BuildProgram();
	GetVertices();
	CalColor(colors, vertices);
	//SetColor(1.0f, 0.5f, 0.0f);

	RegisterVertexArray();
	BindCBuffersCV();
}





void Graphic::RegisterVertexArray() {
	/*
		generate vao
	*/

	glGenVertexArrays(1, &vao);
}


void Graphic::BindCBuffersCV() {
	/*
		bind vertex array
		and bind bffers for vertex and colors
	*/

	glBindVertexArray(vao);
	glGenBuffers(4, vbo);
	BindBuffer(vbo[0], vertices, program, "vPosition", 3);
	BindBuffer(vbo[1], normals, program, "vNormal", 3);
	BindBuffer(vbo[2], colors, program, "vColor", 3);
	BindBuffer(vbo[3], texcoords, program, "vTexcoord", 2);


}


int Graphic::BuildProgram() {
	ShaderInfo shaders[] = {
		
		{GL_VERTEX_SHADER,"viewing.vert"},
		{GL_FRAGMENT_SHADER,"viewing.frag"},
		{GL_NONE,NULL}
		

	};
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	return program;
}

void Graphic::SetColor(GLfloat r, GLfloat g, GLfloat b) {
	colors.resize(vertices.size());


	for (int i = 0; i < colors.size() / 3; i++) {
		colors[i * 3] = r;
		colors[i * 3 + 1] = g;
		colors[i * 3 + 2] = b;
	}
}


void Graphic::BindBuffer(GLint buffer, vector<GLfloat>& vec, int program, const GLchar* attri_name, GLint attri_size) {

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vec.size(), vec.data(), GL_STATIC_DRAW);

	GLuint location = glGetAttribLocation(program, attri_name);
	glVertexAttribPointer(location, attri_size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
}



void Graphic::CalColor(vector<GLfloat>& c, vector<GLfloat>& p) {
	GLfloat offset = 0.1f;
	GLfloat max_val[3] = { -INFINITY,-INFINITY,-INFINITY };
	GLfloat min_val[3] = { INFINITY,INFINITY,INFINITY };

	int n = (int)(p.size() / 3);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 3; j++) {
			GLfloat val = p[i * 3 + j];
			if (max_val[j] < val) {
				max_val[j] = val;
			}
			else if (min_val[j] > val) {
				min_val[j] = val;
			}
		}
	}
	GLfloat width[3] = {
		max_val[0] - min_val[0],
		max_val[1] - min_val[1],
		max_val[2] - min_val[2]
	};

	c.resize(p.size());
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 3; j++) {
			int k = i * 3 + j;
			c[k] = fminf((p[k] - min_val[j]) / width[j] + offset, 1.0f);
		}
	}

}

void Graphic::LoadTexture(const char* basedir, const vector<real_t>& attTexCords, GLint min_filter , GLint mag_filter ) {
	is_tex_valid = LoadTexture(basedir, texcoords, texmap, attTexCords, shapes, materials, min_filter, mag_filter);
}

bool Graphic::LoadTexture(const char* basedir, vector<real_t>& texcoords_out, map<string, size_t>& texmap_out, const vector<real_t>& texcoords, const vector<shape_t>& shapes, const vector<material_t>& materials, GLint min_filter = GL_LINEAR_MIPMAP_LINEAR, GLint mag_filter = GL_LINEAR_MIPMAP_LINEAR) {

	//1. Compute valid texture coordinates for each vertex.
	size_t total_num_of_vertices = 0;
	size_t num_of_shapes = shapes.size();
	for (size_t s = 0; s < num_of_shapes; ++s) {
		total_num_of_vertices += shapes[s].mesh.indices.size();
	}

	texcoords_out.resize(total_num_of_vertices * 2);

	real_t* texcoords_dst_ptr = texcoords_out.data();
	const real_t* texcoords_src_ptr = texcoords.size() > 0 ? texcoords.data() : NULL;
	for (size_t s = 0; s < num_of_shapes; ++s) {
		const mesh_t& mesh = shapes[s].mesh;
		size_t num_of_faces = mesh.indices.size() / 3;
		for (size_t f = 0; f < num_of_faces; ++f) {
			// Get indices to the three vertices of a triangle.
			int idx[3] = {
			mesh.indices[3 * f + 0].texcoord_index,
			mesh.indices[3 * f + 1].texcoord_index,
			mesh.indices[3 * f + 2].texcoord_index
			};
			// Compute and copy valid texture coordinates.
			real_t tc[3][2];
			if (texcoords_src_ptr != NULL) {
				if (idx[0] < 0 || idx[1] < 0 || idx[2] < 0) {
					fprintf(stderr, "Invalid texture coordinate index\n");
					return false;
				}
				for (size_t i = 0; i < 3; ++i) {
					memcpy(tc[i], texcoords_src_ptr + idx[i] * 2, sizeof(real_t) * 2);
					tc[i][1] = 1.0f - tc[i][1];// flip the t coordinate.
				}
			}
			else {
				tc[0][0] = tc[0][1] = 0;
				tc[1][0] = tc[1][1] = 0;
				tc[2][0] = tc[2][1] = 0;
			}
			memcpy(texcoords_dst_ptr, tc, sizeof(real_t) * 6);
			texcoords_dst_ptr += 6;
		}
	}



	// 2.Make a texture object for each material.

	GLuint texture_id;
	size_t num_of_materials = materials.size();
	for (size_t m = 0; m < num_of_materials; ++m) {
		const material_t& mat = materials[m];
		const string& texname = mat.diffuse_texname;
		if (texname.empty()) continue;
		if (MAP_FIND(texmap_out, texname)) continue;
		// Open the texture image file.
		string full_texpath = texname;
		if (!has_file(full_texpath.c_str()))
		{
			full_texpath = basedir + texname;
			if (!has_file(full_texpath.c_str())) {
				fprintf(stderr, "Failed to find %s\n", texname.c_str());
				return false;
			}
		}
		// Generate a texture object.
		texture_id = generate_tex(full_texpath.c_str(), min_filter, mag_filter);
		if (texture_id < 0) {
			return false;
		}
		// Register the texture id.
		texmap_out[texname] = texture_id;
	}

	return true;

}



bool Graphic::has_file(const char* filepath) {
	FILE* fp;
	if (fopen_s(&fp, filepath, "rb") == 0) {
		fclose(fp);
		return true;
	}
	return false;
}

GLuint Graphic::generate_tex(const char* tex_file_path, GLint min_filter, GLint mag_filter) {

	int width, height, num_of_components;
	unsigned char* image = stbi_load(tex_file_path, &width, &height, &num_of_components, STBI_default);
	if (!image) {
		fprintf(stderr, "Failed to open %s\n", tex_file_path);
		return false;
	}
	// Generate a texture object and set its parameters.
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	bool is_supported = true;
	switch (num_of_components) {
		case 3: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			break;
		}

		case 4: {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			break;
		}
		default: {
			is_supported = false;
			break;
		}
	}

	if (IS_MIPMAP(min_filter) || IS_MIPMAP(mag_filter)) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	// Release the loaded image data.
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (!is_supported) {
		fprintf(stderr, "Unsupported image format: %d components\n", num_of_components);
		glDeleteTextures(1, &texture_id);
		texture_id = -1;
	}

	return texture_id;


}


void Graphic::SetShaderValue(int program, const char* attrib_name, mat4 value) {
	int location = glGetUniformLocation(program, attrib_name);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

void Graphic::LoadObj(const char* filename, const char* basedir, attrib_t& attrib, real_t scale) {
	is_obj_valid = load_obj(filename, basedir, vertices, normals, vertex_map, material_map, attrib, shapes, materials, scale);
}


vector<GLfloat> Graphic::GetVertices() {
	return vertices;
}

void Graphic::CalVertices() {
	cout << "GetVertices: None graphic object is assigned...." << endl;
}


void Graphic::BindElements() {
	cout << "BindElements: No element founds in your object" << endl;
}

void Graphic::RenderGraphic() {
	cout << "RenderGraphic: None graphic object is assigned..." << endl;

}
