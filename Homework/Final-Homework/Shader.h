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
			// ���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			//gShaderFile.open(geometryPath);
			std::stringstream vShaderStream, fShaderStream;
			// ��ȡ�ļ�
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//gShaderStream << gShaderFile.rdbuf();
			// �ر��ļ�
			vShaderFile.close();
			fShaderFile.close();
			//gShaderFile.close();
			//����ת��Ϊ�ַ���
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			//geometryCode = gShaderStream.str();

		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		// ������ɫ��
		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();
		//const GLchar *gShaderCode = geometryCode.c_str();

		vertex = glCreateShader(GL_VERTEX_SHADER);//�����ɫ��
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);//����

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);//��ȡ�������
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);//ƬԪ��ɫ��
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);//����

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER:fragment::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		if (geometryPath != nullptr) {
			flag = true;
			const char * gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);//������ɫ��
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);

			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(geometry, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER:geometry::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}
			

		//����
		this->Program = glCreateProgram();//������ɫ������
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		if (geometryPath != nullptr)
			glAttachShader(this->Program, geometry);
		glLinkProgram(this->Program);//����

		glValidateProgram(this->Program);
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);//��ȡ�������
			std::cout << "ERROR::SHADER:PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}

	~Shader() {
		glDetachShader(this->Program, vertex);
		glDetachShader(this->Program, fragment);
		// ɾ����ɫ��
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