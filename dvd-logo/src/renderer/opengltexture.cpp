#include "opengltexture.h"
#include "../error.h"
#include <iostream>
#include "../stb_image/stb_image.h"

	unsigned int opengltexture::presentlyBounded = NULL;
	unsigned int  opengltexture::presentTextureType = NULL;

	opengl2Dtexture::opengl2Dtexture(unsigned char* data)
	{
		GLcall(glGenTextures(1, &m_rendererID));
		GLcall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 768, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		GLcall(glBindTexture(GL_TEXTURE_2D, 0));


		if (presentlyBounded != NULL)
		{
			GLcall(glBindTexture(presentTextureType, presentlyBounded));
		}
	}

	opengl2Dtexture::opengl2Dtexture(const std::string& texture_filePath, const std::string& name /*= "m_texture"*/, unsigned int format, unsigned internalFormat)
		:opengltexture()
	{
		m_width = 0; m_hieght = 0; m_presentTextureSlot = 0; m_textureName = name;
		unsigned char* tex_data = nullptr;
		if (texture_filePath.find("png") != std::string::npos || texture_filePath.find(".png") != std::string::npos)
		{
			 stbi_set_flip_vertically_on_load(1);
		}
		tex_data = stbi_load(texture_filePath.c_str(), &m_width, &m_hieght, &m_BPP, 4);//TODO chek the meaning of bits per pixel
	
		if (!tex_data)
		{
			std::cerr << "[ERROR:] failed to load texture:  " << name << std::endl;
		}

		GLcall(glGenTextures(1, &m_rendererID));
		GLcall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLcall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_hieght, 0, format, GL_UNSIGNED_BYTE, tex_data));
		GLcall(glBindTexture(GL_TEXTURE_2D, 0));

	
		if (tex_data)
		{
			stbi_image_free(tex_data);
		}

		if (presentlyBounded != NULL)
		{
			GLcall(glBindTexture(presentTextureType, presentlyBounded));
		}
	}
	
	opengl2Dtexture::opengl2Dtexture(glenum internalFormat, glenum format, int width, int hieght)
		:opengltexture()
	{
		m_width = width; m_hieght = hieght; m_presentTextureSlot = 0;
		GLcall(glGenTextures(1, &m_rendererID));
		GLcall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

		float borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//GLcall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		//GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		//GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

		GLcall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_hieght, 0, format, GL_UNSIGNED_BYTE, NULL));
		GLcall(glBindTexture(GL_TEXTURE_2D, 0));

		if (presentlyBounded != NULL)
		{
			GLcall(glBindTexture(presentTextureType, presentlyBounded));
		}
	}

	opengl2Dtexture::~opengl2Dtexture()
	{
		//GLcall(glDeleteTextures(1, m_rendererID));
	}

	void opengltexture::setTexturesName(const std::string& name)
	{
		m_textureName = name;
	}

	void opengltexture::setTexturesName(const std::string&& name)
	{
		m_textureName = name;
	}

	void opengl2Dtexture::bindTextureToSlot(unsigned int slot /*= 0*/)
	{
		presentlyBounded = m_rendererID;
		presentTextureType = GL_TEXTURE_2D;

		m_presentTextureSlot =  slot;
		GLcall(glActiveTexture((GL_TEXTURE0 + slot)));
		GLcall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
	}

	void opengltexture::unbind(unsigned int slot /*= 0*/)
	{
		if (presentlyBounded == this->m_rendererID)
		{
			presentlyBounded = NULL;
			presentTextureType = NULL;
		}

		GLcall(glActiveTexture((GL_TEXTURE0 + slot)));
		GLcall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	openglCubeMaptexture::openglCubeMaptexture(std::vector<std::string>& texture_filePaths, std::string& name, GLenum internalFormat)
	{
		GLcall(glGenTextures(1, &m_rendererID));
		GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));

		m_width = 0; m_hieght = 0; m_presentTextureSlot = 0; m_textureName = name;
		unsigned char* tex_data = nullptr;
		int j = 0;

		for (int i = 0; i < 6; i++)
		{
			if (texture_filePaths[i].find("png") != std::string::npos || texture_filePaths[i].find(".png") != std::string::npos)
			{
				stbi_set_flip_vertically_on_load(1);
			}

			tex_data = stbi_load(texture_filePaths[i].c_str(), &m_width, &m_hieght, &m_BPP, 4);//TODO chek the meaning of bits per pixel

			GLcall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_width, m_hieght, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));	
		}
		GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

		if (tex_data)
		{
			stbi_image_free(tex_data);
		}

		if (presentlyBounded != NULL)
		{
			GLcall(glBindTexture(presentTextureType, presentlyBounded));
		}
	}


	
	openglCubeMaptexture::openglCubeMaptexture(std::array<opengl2Dtexture, 6> textures)
	{
		GLcall(glGenTextures(1, &m_rendererID));
		GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));

		m_width = 0; m_hieght = 0; m_presentTextureSlot = 0;
		unsigned char* data = (unsigned char*)malloc(1280 * 1280 * 4);
		int j = 0;

		for (int i = 0; i < 6; i++)
		{
			textures[i].bindTextureToSlot(2);
			GLcall(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

			GLcall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, 1280, 1280, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		}
		GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));


		if (presentlyBounded != NULL)
		{
			GLcall(glBindTexture(presentTextureType, presentlyBounded));
		}
	}


	openglCubeMaptexture::openglCubeMaptexture(GLenum format, GLenum internalFormat, int width, int hieght)
	{
		GLcall(glGenTextures(1, &m_rendererID));
		GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));

		m_width = width; m_hieght = hieght; m_presentTextureSlot = 0;
		unsigned char* tex_data = nullptr;
		int j = 0;

		for (int i = 0; i < 6; i++)
		{
			GLcall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_width, m_hieght, 0, format, GL_UNSIGNED_BYTE, nullptr));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLcall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		}
		GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

		if (presentlyBounded != NULL)
		{
			GLcall(glBindTexture(presentTextureType, presentlyBounded));
		}
	}

	void openglCubeMaptexture::operator=(const openglCubeMaptexture&& texture)
	{
		(*this).m_BPP = texture.m_BPP;
		(*this).m_hieght = texture.m_hieght;
		(*this).m_presentTextureSlot = texture.m_presentTextureSlot;
		(*this).m_rendererID = texture.m_rendererID;
		(*this).m_textureName = texture.m_textureName;
		(*this).m_width = texture.m_width;
	}

	void openglCubeMaptexture::bindTextureToSlot(unsigned int slot)
	{
		presentlyBounded = m_rendererID;
		presentTextureType = GL_TEXTURE_CUBE_MAP;

		m_presentTextureSlot = slot;
		GLcall(glActiveTexture((GL_TEXTURE0 + slot)));
		GLcall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID));
	}

	openglCubeMaptexture::~openglCubeMaptexture()
	{

	}
