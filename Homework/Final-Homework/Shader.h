#pragma once
//#ifndef shader_h
//#define shader_h
//#endif // !shader.hpp
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>


class Shader {
	GLuint vertex, fragment, geometry;
	bool flag = false;
public:
	GLuint Program;
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar* geometryPath = nullptr) {
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		vShaderFile.exceptions(std::ifstream::badbit  | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit  | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// 打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			//gShaderFile.open(geometryPath);
			std::stringstream vShaderStream, fShaderStream;
			// 读取文件
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//gShaderStream << gShaderFile.rdbuf();
			// 关闭文件
			vShaderFile.close();
			fShaderFile.close();
			//gShaderFile.close();
			//将流转换为字符串
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			//geometryCode = gShaderStream.str();

		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		// 编译着色器
		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();
		//const GLchar *gShaderCode = geometryCode.c_str();

		vertex = glCreateShader(GL_VERTEX_SHADER);//顶点调色器
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);//编译

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);//获取编译情况
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);//片元调色器
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);//编译

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER:fragment::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		if (geometryPath != nullptr) {
			flag = true;
			const char * gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);//几何着色器
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);

			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(geometry, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER:geometry::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}
			

		//链接
		this->Program = glCreateProgram();//创建着色器程序
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		if (geometryPath != nullptr)
			glAttachShader(this->Program, geometry);
		glLinkProgram(this->Program);//链接

		glValidateProgram(this->Program);
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);//获取链接情况
			std::cout << "ERROR::SHADER:PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}

	~Shader() {
		glDetachShader(this->Program, vertex);
		glDetachShader(this->Program, fragment);
		// 删除着色器
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (flag == true)
			glDeleteShader(geometry);
		glDeleteProgram(this->Program);

	}
	void Use() {
		glUseProgram(this->Program);
	}
};