#pragma once
#include "GLAD/include/glad/glad.h"
#include <string>
#include <iostream>
#include <vector>
#include "../vendor/glm/glm.hpp"
#include "renderer/buffers.h"
#include "renderer/opengleShader.h"
#include "renderer/opengltexture.h"
#include "renderer/vertexArray.h"

#include "../vendor/tiny/tiny_obj_loader.h"

namespace Models
{
	bool LoadObj(tinyobj::attrib_t& attrib, std::vector<tinyobj::shape_t>& shapes, std::vector<tinyobj::material_t>& materials, const char* filename, const char* basepath = NULL, bool triangulate = true);

	class modelObject
	{
	public:
		modelObject();
		~modelObject();

	private:
	};

}


class MeshObject
{
//Data
vertexArray* Vao;
vertexBuffer* Vbo;
opengl2Dtexture Texture;
openglShader& program;
BufferLayout Layout;

unsigned int Hsize = 0;
unsigned int Hsize2 = 0;
unsigned int Hsize3 = 0;

int countH1 = 0;

public:
	MeshObject() = default;

    MeshObject(std::string objectPath, std::string basePath, std::string Texture, openglShader& GPUprogram);
	~MeshObject();
	void draw(glm::mat4 Model, int slot);
};
