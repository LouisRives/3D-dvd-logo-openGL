#pragma once

#include <string>
#include <vector>
#include "../error.h"
#include <array>

typedef unsigned int glenum;
class opengltexture
{
public:
	//default ctor and dtor
	opengltexture()	{};
	~opengltexture() {};

	//set nothing to a texture slot
	void unbind(unsigned int slot = 0);
	//returns name
	void setTexturesName(const std::string& name);
	//returns name
	void setTexturesName(const std::string&& name);
	//binds texture to a slot 
	//no of available slots to bind to is variable
	virtual void bindTextureToSlot(unsigned int slot = 0) = 0;

	const std::string& getName() { return m_textureName; }
	unsigned int getId()const { return m_rendererID; }
	unsigned int GetTextureSlot() { return m_presentTextureSlot; }

protected:
	unsigned int m_rendererID;
	std::string m_textureName;
	int m_width, m_hieght, m_BPP;
	unsigned int m_presentTextureSlot;
	static unsigned int presentlyBounded;
	static unsigned int  presentTextureType;
};

//class inheriting from opengl texture 
//all functions are specific for 2D textures
	class opengl2Dtexture : public opengltexture
	{
	public:
		opengl2Dtexture() = default;
		opengl2Dtexture(unsigned char* data);
		opengl2Dtexture(const std::string& texture_filePath, const std::string& name = "m_texture", unsigned int format = GL_RGBA, unsigned internalFormat = GL_RGBA8);
		opengl2Dtexture(glenum internalFormat, glenum format, int width, int hieght);
		~opengl2Dtexture();

		void bindTextureToSlot(unsigned int slot = 0)override;
	};

//class inheriting from opengl texture 
//all functions are specific for  cubemaps
	class openglCubeMaptexture : public opengltexture
	{
	public:
		openglCubeMaptexture() = default;
		openglCubeMaptexture(std::vector<std::string>& texture_filePaths, std::string& name, GLenum internalFormat = GL_RGBA8);
		openglCubeMaptexture(std::array<opengl2Dtexture, 6> textures);
		openglCubeMaptexture(GLenum format, GLenum internalFormat, int width, int hieght);
		~openglCubeMaptexture();

		void operator=(const openglCubeMaptexture&& texture);
		void bindTextureToSlot(unsigned int slot = 0)override;
	};

