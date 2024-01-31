#include "opengleShader.h"
#include <fstream>
#include <sstream>
#include "../error.h"


	openglShader::openglShader(const char* filePath, const char* name, bool geometryShaderEnabled, bool transFormFeed, const char* varyings)
		:m_rendererID(0), m_shader_filePath((std::string)filePath), uniform_Locations_cache(NULL), m_name(name)
	{
		if (!geometryShaderEnabled)
		{
			auto [vs, fs, gs] = ParseShader(filePath, geometryShaderEnabled);
			m_rendererID = createShader(vs, fs);
		}
		else
		{
			auto [vs, fs, gs] = ParseShader(filePath, geometryShaderEnabled);
			m_rendererID = createShader(vs, fs, gs, transFormFeed, varyings);
		}

	}

	openglShader::openglShader(std::string vertex, std::string fragment)
	{
		m_rendererID = createShader(vertex, fragment);
	}

	openglShader::openglShader(const char* vertex_shader_path, const char* pixel_shader_path, const char* name)
	{
		auto vs = ParseSingleShader(vertex_shader_path);
		auto fs = ParseSingleShader(pixel_shader_path);

		m_rendererID = createShader(vs, fs);
	}

	openglShader::openglShader(const char* vertex_shader_path, const char* pixel_shader_path, const char* geometry_shader_path, const char* name)
	{
		auto vs = ParseSingleShader(vertex_shader_path);
		auto fs = ParseSingleShader(pixel_shader_path);
		auto gs = ParseSingleShader(geometry_shader_path);
		m_rendererID = createShader(vs, fs, gs);
	}

	openglShader::~openglShader()
	{

	}

	void openglShader::Bind()const 
	{
		GLcall(glUseProgram(m_rendererID));
	}

	void openglShader::Unbind()const
	{
		GLcall(glUseProgram(0));
	}

	void openglShader::setUniformMat4(const char* name, const glm::mat4& data, unsigned int count)
	{
		Bind();
		GLcall(glUniformMatrix4fv(getUniformLocation(name), (int)count, GL_FALSE, &data[0][0]));
	}	

	void openglShader::setUniformMat4(const char* name, const glm::mat4* data, unsigned int count)
	{
		float f = (*data).operator[](3).z;
		std::cout << f << std::endl;

		GLcall(glUniformMatrix4fv(getUniformLocation(name), (int)count, GL_FALSE, &((*(data))[0][0]) ));
	}

	void openglShader::setUniformVec3(const char* name, const glm::vec3& data, unsigned int count)
	{
		Bind();
		GLcall(glUniform3fv(getUniformLocation(name), (int)count, &data[0]));
	}

	void openglShader::setUniformVec4(const char* name, const glm::vec4& data, unsigned int count)
	{
		GLcall(glUniform4fv(getUniformLocation(name), (int)count, &data[0]));
	}

	void openglShader::setTextureSampler(const char* name, int r_data, unsigned int count)
	{
		Bind();
		GLcall(glUniform1iv(getUniformLocation(name), (int)count, &r_data));
	}

	void openglShader::setTextureSampler(const char* name, int* r_data, unsigned int count)
	{
		GLcall(glUniform1iv(getUniformLocation(name), (int)count, r_data));
	}

	bool openglShader::operator!=(int nullOrNot)
	{
		if(m_rendererID != NULL && m_shader_filePath[0] != std::string::npos) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void openglShader::setUniformFloat(const char* name, float data)
	{
		GLcall(glUniform1f(getUniformLocation(name), data));
	}


	void openglShader::setUniformFloat(const char* name, float* data, unsigned int count)
	{
		GLcall(glUniform1fv(getUniformLocation(name), count, data));
	}

	std::tuple<std::string, std::string, std::string> openglShader::ParseShader(const char* filePath, bool geometryShaderEnabled)
	{
		std::ifstream stream(filePath);
		if (stream.fail())
		{
			std::cerr << "file error" << std::endl;
		}

		enum class shaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
		};


		std::string line;
		std::stringstream ss[3];
		shaderType type = shaderType::NONE;
		
		if(!geometryShaderEnabled)
		{ 
			while (getline(stream, line))
			{
				if (line.find("#SHADER") != std::string::npos)
				{
					if (line.find("VERTEX") != std::string::npos)
					{
						type = shaderType::VERTEX;
					}

					else if (line.find("PIXEL") != std::string::npos || line.find("FRAGMENT") != std::string::npos)
					{
						type = shaderType::FRAGMENT;
					}
				}

				else
				{
					ss[(int)type] << line << '\n';
				}

			}
			return {ss[0].str(), ss[1].str(), ss[2].str()};
		
		}

		else
		{
			while (getline(stream, line))
			{
				if (line.find("#SHADER") != std::string::npos)
				{
					if (line.find("VERTEX") != std::string::npos)
					{
						type = shaderType::VERTEX;
					}

					else if (line.find("PIXEL") != std::string::npos || line.find("FRAGMENT") != std::string::npos)
					{
						type = shaderType::FRAGMENT;
					}

					else if (line.find("GEOMETRY") != std::string::npos)
					{
						type = shaderType::GEOMETRY;
					}
				}

				else
				{
					ss[(int)type] << line << '\n';
				}

			}
			return { ss[0].str(), ss[1].str(), ss[2].str() };
		}
	}

	std::string openglShader::ParseSingleShader(const char* path)
	{
		std::ifstream stream;
		stream.open(path);

		if (stream.fail())
		{
			std::cerr << " file error " << std::endl;
		}

		std::string shader;
		std::string line;

		while (getline(stream, line))
		{
			shader += line;
			shader += "\n";
		}

		return shader;
	}


	unsigned int openglShader::compileShader(std::string& shader, unsigned int shaderType)
	{
		GLcall(unsigned shader_obj = glCreateShader(shaderType));
		const char* src = shader.c_str();
		GLcall(glShaderSource(shader_obj, 1, &src, nullptr));
		GLcall(glCompileShader(shader_obj));

		int status = 0;
		GLcall(glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &status));

		if (status == GL_FALSE)
		{
			int length;
			GLcall(glGetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &length));

			char* message = (char*)alloca(sizeof(char) * length);
			GLcall(glGetShaderInfoLog(shader_obj, length, &length, message));

			std::cout << m_name + " error -> "<< (shaderType == GL_VERTEX_SHADER ? "[vertex shader error: ]" : (shaderType == GL_GEOMETRY_SHADER ? "[geometry shader error: ]"  : "[fragment shader error: ]")) << message << std::endl;
		}

		return shader_obj;
	}


	unsigned int openglShader::createShader(std::string& vs, std::string& fs, bool transformFeed, const char* varyings)//this varyings method only supports 1 varying feedback
	{
		unsigned int program = glCreateProgram();
		unsigned int m_vs = compileShader(vs, GL_VERTEX_SHADER);
		unsigned int m_fs = compileShader(fs, GL_FRAGMENT_SHADER);

		GLcall(glAttachShader(program, m_vs));
		GLcall(glAttachShader(program, m_fs));

		GLcall(glLinkProgram(program));

		int linkStatus;
		GLcall(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));

		if (linkStatus == GL_FALSE)
		{
			int length;
			GLcall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(int));
			GLcall(glGetProgramInfoLog(program, length, &length, message));
			std::cout << m_name + " error -> linker error" << message << std::endl;

			GLcall(glDeleteShader(m_vs));
			GLcall(glDeleteShader(m_fs));

			GLcall(glDetachShader(program, m_vs));
			GLcall(glDetachShader(program, m_fs));
		}

		GLcall(glValidateProgram(program));

		return program;
	}

	unsigned int openglShader::createShader(std::string& vs, std::string& fs, std::string& gs, bool transformFeed, const char* varyings)
	{
		unsigned int program = glCreateProgram();
		unsigned int m_vs = compileShader(vs, GL_VERTEX_SHADER);
		unsigned int m_fs = compileShader(fs, GL_FRAGMENT_SHADER);
		unsigned int m_gs = compileShader(gs, GL_GEOMETRY_SHADER);

		GLcall(glAttachShader(program, m_vs));
		GLcall(glAttachShader(program, m_fs));
		GLcall(glAttachShader(program, m_gs));

		
		GLcall(glLinkProgram(program));

		int linkStatus;
		GLcall(glGetProgramiv(program, GL_LINK_STATUS, &linkStatus));

		if (linkStatus == GL_FALSE)
		{
			int length;
			GLcall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(int));
			GLcall(glGetProgramInfoLog(program, length, &length, message));
			std::cout << m_name + " error -> linker error" << message << std::endl;

			GLcall(glDeleteShader(m_vs));
			GLcall(glDeleteShader(m_fs));
			GLcall(glDeleteShader(m_gs));

			GLcall(glDetachShader(program, m_vs));
			GLcall(glDetachShader(program, m_fs));
			GLcall(glDetachShader(program, m_gs));
		}

		GLcall(glValidateProgram(program));

		return program;
	}

	int openglShader::getUniformLocation(const char* name)
	{
		if (uniform_Locations_cache.find(name) != uniform_Locations_cache.end())
		{
			return  uniform_Locations_cache[name];
		}

		GLcall(int location = glGetUniformLocation(m_rendererID, name));
		std::cout << (location < 0 || location == -1? name : "") << (location < 0 ? " is not a valid uniform" : "") << std::endl;
		uniform_Locations_cache[name] = location;
		return location;
	}

