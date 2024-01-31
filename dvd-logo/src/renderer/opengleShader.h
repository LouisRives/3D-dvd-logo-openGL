#ifndef OPENGL_SHADER_H
#define  OPENGL_SHADER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <optional>

#include "../../vendor/glm/glm.hpp"

	class openglShader
		{
		public:
			//creates 2 to 3 shader objects stored in filepath.  shader parse function has a format for seperating different shaders
			//could contain vertex, fragment/pixel, and grometry shader
			openglShader(const char* filePath, const char* name, bool geometryShaderEnabled = false, bool transFormFeed = false, const char* varyings = "");
			//creates shader objects from strings
			openglShader(std::string vertex, std::string fragment);
			//creates shader vertex and fragment shader objects from given paths
			openglShader(const char* vertex_shader_path, const char* pixel_shader_path, const char* name);
			//creates shader vertex, geometry and fragment shader objects from given paths
			openglShader(const char* vertex_shader_path, const char* pixel_shader_path, const char* geometry_shader_path, const char* name);
			//default constructor
			openglShader(){}
			//deletes shader
			~openglShader();

			//sets shader program as the currently used shader by opengl
			void Bind()const ;
			//sets currentl used shader to nothing
			void Unbind()const ;
			//set uniform matrix 4 with reference
			void setUniformMat4(const char* name, const glm::mat4& data, unsigned int count = 1);
			//set uniform matrix 4 with pointer
			void setUniformMat4(const char* name, const glm::mat4* data, unsigned int count = 1);
			//set uniform vector 4 with reference
			void setUniformVec4(const char* name, const glm::vec4& data, unsigned int count = 1);
			//set uniform vector 3 with reference
			void setUniformVec3(const char* name, const glm::vec3& data, unsigned int count = 1);
			//set uniform texture slot
			void setTextureSampler(const char* name, int r_data, unsigned int count = 1);
			//set uniform texture slot with pointer
			void setTextureSampler(const char* name, int* r_data, unsigned int count = 1);
			//set uniform float value with pointer
			void setUniformFloat(const char* name, float* data, unsigned int count = 1);
			//sets uniform float value
			void setUniformFloat(const char* name, float data);

			bool operator!=(int nullOrNot);
			const std::string& getShaderFilePath() { return m_shader_filePath; }
			unsigned int getRendererId() { return m_rendererID; }

		private:
			//reads shader from file
			std::string ParseSingleShader(const char* path);
			//creates shader
			unsigned int createShader(std::string& vs, std::string& fs, bool transformFeed = false, const char* varyings = "");
			//compiles shader in glsl
			unsigned int compileShader(std::string& shader, unsigned int shaderType);
			//creates program with geometry shader included
			unsigned int createShader(std::string& vs, std::string& fs, std::string& gs, bool transformFeed = false, const char* varyings = "");
			//parse shaders from a single file
			std::tuple<std::string, std::string, std::string> ParseShader(const char* filePath, bool geometryShaderEnabled);
		private:
			std::string m_name;
			unsigned int m_rendererID;
			std::string m_shader_filePath;
			//stores unifor indexs for better performance
			std::unordered_map <std::string, int> uniform_Locations_cache;
		protected:
			//returns uniform indexes
			//if a uniform index has been obtained before it is retrieved from uniform_Locations_cache
			int getUniformLocation(const char* name);
	};

#endif
