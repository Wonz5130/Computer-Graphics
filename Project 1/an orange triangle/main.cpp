#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//
#include "Shader.h"
const GLint WIDTH = 800, HEIGHT = 600;  //�½�����

/*
//�����ɫ�������ж��㶼����ʱ�뻭��
const GLchar *vertexShaderSource = "#version 330 core\n"  //�汾�ͺ�
"layout(location = 0) in vec3 position;\n"  //��ɫ��λ�ò�����Ϣ��vector ������3 ����Ϣ��location = 0 ��ʾ�� 1 ��������= 1 ��ʾ�� 2 ������
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"  //��λ�����������������һά���ﵽ�����ƶ�Ŀ�ģ�
"}";

//��Ե��ɫ��
const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"  //�ĸ�����
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 0.7f);\n"  //�� = ���и�ֵ
"}";
*/


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //�õ����°�� OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // must for Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  //��Ϊ GL_TRUE���ı䴰�ڣ��ݺ�Ȼ��
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL B16112011", nullptr,
		nullptr);  //�������ָĳ��Լ���ѧ��

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// next two lines are for mac retina display
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);  //��ȡ���ڴ�С

	glfwMakeContextCurrent(window);  //�����½��ܶ� window

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);  //�ӣ�0,0����ʼ���㣬ֱ�� WIDTH �� HEIGHT

	//vs �Ƕ����ɫ����frag �Ǳ�Ե��ɫ��
	Shader ourShader = Shader("core.vs", "core.frag");  //�ļ����·��

	/*
	//ע�Ͳ���
	//import and compile the shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);  //�����ɫ��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);  //����

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);  //�����Ƿ���ɵ�λ��
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout <<
			"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//Ƭ����ɫ��
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);  //����

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);  //�����Ƿ���ɵ�λ��
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout <<
			"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//create the program and link the program
	GLuint shaderProgram = glCreateProgram();  //������ɫ������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);  //����

	//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout <<
			"ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//ע�Ͳ���
	*/

	//now the verte information comes below

	GLfloat vertices[] = {
		//position
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	//the date should be transfered to the memory on the Graphics Card,�����Դ�
	GLuint VAO, VBO;  //VAO:Vertex Array Object   VBO:Vertex Buffer Object������
	glGenVertexArrays(1, &VAO);  //���� VAO
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);  //�赱ǰֱ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //VAO �� VBO �ɶԳ���
	// transfer the data��������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //��̬���ʣ��������޸�
	//set the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat), (GLvoid *)0);  //0:��Ӧ��ɫ���� location ��ֵ��3:��Ӧ vec3 ��������GL_FLOAT:�����ͣ�GL_FALSE:��3*sizeof(GLfloat):��Ӧ Buffer �ﴫ�����ݣ�(GLvoid*)0:�ӵ� 0 ��λ�ÿ�ʼ
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		//6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));  //1:��Ӧ��ɫ���� color ��ֵ��3:��Ӧ vec3 ��������GL_FLOAT:�����ͣ�GL_FALSE:��6*sizeof(GLfloat):ÿ�ο�Խ 6 ����(GLvoid*) (3 * sizeof(GLfloat)):�ӵ� 0 ��λ�ÿ�ʼ
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//��ͼ
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();  //�������¼�ϵͳ��ȡ����������/���Ȳ���
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //���ڱ�����ɫ��RGB�����һ����͸����
		glClear(GL_COLOR_BUFFER_BIT);

		//Bind the shader
		//glUseProgram(shaderProgram);  //ʹ�õ�ɫ��,��ע��
		ourShader.Use();

		//Draw the triangle
		glBindVertexArray(VAO);  //ʹ�� VAO��ֱ�Ӱ�
		glDrawArrays(GL_TRIANGLES, 0, 3);  //�������Σ��ӵ� 0 �����ݿ�ʼ���������һ�����ݣ��� 3 ��������
		glBindVertexArray(0);

		glfwSwapBuffers(window);  //����˫����л�����ʾһ������һ���ڻ������������
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}