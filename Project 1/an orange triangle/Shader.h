#pragma once
//#ifndef shader_hpp
//#define shader_hpp
//#endif /* shader_hpp */
#include<string>
#include<fstream>  //���Դ��ļ�
#include<sstream>
#include<iostream>
#include<GL/glew.h>

class Shader {
	GLuint vertex, fragment;
public:
	GLuint Program;
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);

		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//�ļ��ر�˳���� v �� f
			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure a) {
			std::cout <<
				"ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ"
				<< std::endl;
		}
		//����ת��
		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();

		//import and compile the shader
		vertex = glCreateShader(GL_VERTEX_SHADER);  //�������¶���
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);  //����

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);  //�����Ƿ���ɵ�λ��
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout <<
				"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				<< infoLog << std::endl;
		}
		
		//��Ե��ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);  //����

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);  //�����Ƿ���ɵ�λ��
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout <<
				"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				<< infoLog << std::endl;
		}

		//create the program and link the program
		this->Program = glCreateProgram();  //������ɫ������
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);  //����
		
		glValidateProgram(this->Program);  //��ʡ��
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);  //��ȡ�������
			std::cout <<
				"ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
				infoLog << std::endl;
		}
	}

	~Shader() {
		glDetachShader(this->Program, vertex);
		glDetachShader(this->Program, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteProgram(this->Program);
	}
	void Use() {
		glUseProgram(this->Program);
	}
};