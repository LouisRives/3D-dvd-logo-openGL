#include "object.h"

#include <iostream>
#include <string>

namespace Models
{
	bool LoadObj(tinyobj::attrib_t& attrib, std::vector<tinyobj::shape_t>& shapes, std::vector<tinyobj::material_t>& materials, const char* filename, const char* basepath, bool triangulate) {

		std::string err;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename,
			basepath, triangulate);

		std::cout << "[ERRORS:] " << err << std::endl;

		if (!ret) {
			printf("Failed to load/parse .obj.\n");
			return false;
		}

		return true;
	}
}

MeshObject::MeshObject(std::string objectPath, std::string basePath, std::string Texture, openglShader& GPUprogram)
:program(GPUprogram)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	Models::LoadObj(attrib, shapes, materials, objectPath.c_str(), basePath.c_str(), true);

	std::vector<float> vertex;
	std::vector<float> normals;
	std::vector<float> texCoords;


	for (auto var : shapes)
	{
		int indexOffset = 0;
		for (int i = 0; i < var.mesh.num_face_vertices.size(); i++)
		{
			int ii = var.mesh.num_face_vertices[i];


			for (int v = 0; v < ii; v++)
			{
				auto j = var.mesh.indices[indexOffset + v];

				vertex.push_back(attrib.vertices[(3 * j.vertex_index) + 0]);
				vertex.push_back(attrib.vertices[(3 * j.vertex_index) + 1]);
				vertex.push_back(attrib.vertices[(3 * j.vertex_index) + 2]);

				normals.push_back(attrib.normals[(3 * j.normal_index) + 0]);
				normals.push_back(attrib.normals[(3 * j.normal_index) + 1]);
				normals.push_back(attrib.normals[(3 * j.normal_index) + 2]);

				texCoords.push_back(attrib.texcoords[(2 * j.texcoord_index) + 0]);
				texCoords.push_back(attrib.texcoords[(2 * j.texcoord_index) + 1]);
			}

			indexOffset += ii;
		}



	}

	Hsize = vertex.size() * sizeof(float);
	Hsize2 = normals.size() * sizeof(float);
	Hsize3 = texCoords.size() * sizeof(float);

	Vbo = new vertexBuffer(vertex.data(), Hsize + Hsize2 + Hsize3, GL_STATIC_DRAW);
	Vbo->Bind();
	Vbo->updateBuffer(normals.data(), Hsize2, Hsize);
	Vbo->updateBuffer(texCoords.data(), Hsize3, Hsize + Hsize2);

	countH1 = vertex.size() / 3.0f;

	Vao = new vertexArray(true);
	Vao->Bind();
	Vbo->Bind();

	Layout.flush();
	Layout.push(3, DataType::FLOAT, false, 0);
	Layout.push(3, DataType::FLOAT, false, (const void*)Hsize);
	Layout.push(2, DataType::FLOAT, false, (const void*)(Hsize + Hsize2));

	Vao->addBuffer(Layout, *Vbo);

	this->Texture = opengl2Dtexture(Texture, Texture);
	this->Texture.bindTextureToSlot(2);
}

MeshObject::~MeshObject()
{
    delete Vbo;
    delete Vao;
}

void MeshObject::draw(glm::mat4 Model, int slot)
{
    program.Bind();
    program.setUniformVec3("color", glm::vec3(0.0f));
    program.setUniformMat4("model", Model);
    Texture.bindTextureToSlot(slot);
    program.setTextureSampler("_MainTex", Texture.GetTextureSlot());

    Vao->Bind();
	Vbo->Bind();
	GLcall(glDrawArrays(GL_TRIANGLES, 0, countH1));
}
