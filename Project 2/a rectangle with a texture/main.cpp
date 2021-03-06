﻿#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//
#include "Shader.h"

// SOIL2
//Linux 用的是 \, 但是 / 都可以用
#include "SOIL2/SOIL2.h"  
#include "SOIL2/stb_image.h"

const GLint WIDTH = 800, HEIGHT = 600;  //新建窗口

/*
//顶点调色器，所有顶点都是逆时针画的
const GLchar *vertexShaderSource = "#version 330 core\n"  //版本型号
"layout(location = 0) in vec3 position;\n"  //调色器位置参数信息，vector 向量，3 个信息，location = 0 表示进 1 个参数，= 1 表示进 2 个参数
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0f);\n"  //四位，齐次坐标器（增加一维，达到矩阵移动目的）
"}";

//边缘调色器
const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"  //四个参数
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 0.7f);\n"  //用 = 进行赋值
"}";
*/


int main()
{
	glfwInit();
	//OpenGL 版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//窗口设置
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //用的是新版的 OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // must for Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  //改为 GL_TRUE，改变窗口，纵横比会变
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL B16112011", nullptr,
		nullptr);  //窗口名字改成自己的学号

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// next two lines are for mac retina display
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);  //获取窗口大小

	glfwMakeContextCurrent(window);  //可以新建很多 window

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);  //从（0,0）开始画点，直到 WIDTH 和 HEIGHT

	//vs 是顶点调色器，frag 是边缘调色器
	Shader ourShader = Shader("core1.vs", "core1.frag");  //文件相对路径

	/*
	//注释部分
	//import and compile the shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);  //顶点调色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);  //编译

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);  //编译是否完成的位置
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout <<
			"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//片段着色器
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);  //编译

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);  //编译是否完成的位置
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout <<
			"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//create the program and link the program
	GLuint shaderProgram = glCreateProgram();  //创建着色器程序
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);  //链接

	//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout <<
			"ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//注释部分
	*/

	//now the verte information comes below

	/*GLfloat vertices[] = {
		//position               //color
		-0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,       0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,        0.0f, 0.0f, 1.0f
	};*/
	GLfloat vertices[] = {
		//first triangle
		//0.5f, 0.5f, 0.0f,  //top right
		//0.5f, -0.5f, 0.0f,  //bottom right
		//-0.5f, -0.5f, 0.0f,  //top left
		//second triangle
		//0.5f, -0.5f, 0.0f,
		//-0.5f, -0.5f, 0.0f,
		//-0.5f, 0.5f, 0.0f
		//
		0.5f, 0.5f, 0.0f,      1.0f, 1.0f,  //右上角
		0.5f, -0.5f, 0.0f,     1.0f, 0.0f,  //右下角
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f,  //左下角
		-0.5f, 0.5f, 0.0f,     0.0f, 1.0f   //左上角
	};
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	//the date should be transfered to the memory on the Graphics Card,传到显存
	GLuint VAO, VBO;  //VAO:Vertex Array Object   VBO:Vertex Buffer Object传数据
	glGenVertexArrays(1, &VAO);  //创建 VAO
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);  //设当前直线
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //VAO 和 VBO 成对出现
	// transfer the data：传数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //静态访问，几乎不修改
	//set the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(GLfloat), (GLvoid *)0);  //0:对应调色器里 location 的值；3:对应 vec3 三个量；GL_FLOAT:浮点型；GL_FALSE:；5*sizeof(GLfloat):对应 Buffer 里传的数据；(GLvoid*)0:从第 0 个位置开始
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));  //1:对应调色器里 color 的值；2:对应纹理 vec2 两个量；GL_FLOAT:浮点型；GL_FALSE:；5*sizeof(GLfloat):每次跨越 5 个；(GLvoid*) (3 * sizeof(GLfloat)):从第 0 个位置开始
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		//3 * sizeof(GLfloat), (GLvoid *)0);  //0:对应调色器里 location 的值；3:对应 vec3 三个量；GL_FLOAT:浮点型；GL_FALSE:；3*sizeof(GLfloat):对应 Buffer 里传的数据；(GLvoid*)0:从第 0 个位置开始
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		//6 * sizeof(GLfloat), (GLvoid *)0);  //0:对应调色器里 location 的值；3:对应 vec3 三个量；GL_FLOAT:浮点型；GL_FALSE:；3*sizeof(GLfloat):对应 Buffer 里传的数据；(GLvoid*)0:从第 0 个位置开始
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		//6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));  //1:对应调色器里 color 的值；3:对应 vec3 三个量；GL_FLOAT:浮点型；GL_FALSE:；6*sizeof(GLfloat):每次跨越 6 个；(GLvoid*) (3 * sizeof(GLfloat)):从第 0 个位置开始
	//glEnableVertexAttribArray(1);


	//transfer the index
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint texture;  //生成纹理
	int width, height;

	glGenTextures(1, &texture);  //生成
	glBindTexture(GL_TEXTURE_2D, texture); //绑定 2D 纹理

	//二维:S T   三维:S T R
	//i:整型    f:浮点型
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //横向坐标
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //纵向坐标
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //FILTER:滤波器
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //GL_NEAREST:选取靠的最近的点,关注点清晰度可以,但边缘会模糊   GL_LINEAR:整体看效果稍微模糊

	unsigned char* image = SOIL_load_image("2.jpg", 
		&width, &height, 0, SOIL_LOAD_RGBA);  //读取它的 RGBA 信息

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
		GL_UNSIGNED_BYTE, image);  //专门贴纹理, 指定传多大的文件空间, 第一个 0 表示第 0 层, 按照 RGBA 的顺序, 第二个 0 不用管, 存成 unsigned_byte 的格式
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap:图片金字塔
	glBindTexture(GL_TEXTURE_2D, 0);


	//画图
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();  //把所有事件系统都取过来：键盘/鼠标等操作
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //窗口背景颜色，RGB，最后一个是透明度
		glClear(GL_COLOR_BUFFER_BIT);

		//Bind the shader
		//glUseProgram(shaderProgram);  //使用调色器,不注释
		ourShader.Use();

		glActiveTexture(GL_TEXTURE0);  //第 0 个位置,对应的就是 frag 里面的 uniform, 运行过程中传
		glBindTexture(GL_TEXTURE_2D, texture);  //需要
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture0"), 0);  //对应 frag 里面的 ourTexture0, 在 Program 中找 ourTexture0
		//Draw the triangle
		glBindVertexArray(VAO);  //使用 VAO，直接绑定

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);  //画三角形，从第 0 个数据开始画，到最后一个数据（第 3 个）结束
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //解绑定
		glBindVertexArray(0);

		glfwSwapBuffers(window);  //调用双面进行画，显示一个，另一个在画，画面更流畅
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture);

	glfwTerminate();
	return 0;
}

/*
#version 330 core
in vec2 texCoords;
out vec4 color;
void main() {
	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
};
*/

/*
//三角形

#include <iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
const GLint WIDTH = 800, HEIGHT = 600;


const GLchar *vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"//3个信息（浮点型）
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z,1.0f);\n"//记录传入的顶点
"}";

const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 0.7f);\n"
"}";



int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // must for Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//GL_FALSE窗口不可以变大变小
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", nullptr,
		nullptr);
	// next two lines are for mac retina display
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);//获取窗口大小
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);//从（0,0）开始在窗口中画

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);//顶点调色器
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);//编译

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//获取编译情况
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//片元调色器
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);//编译

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER:fragment::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//链接
	GLuint shaderProgram = glCreateProgram();//创建着色器程序
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//链接
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);//获取链接情况
		std::cout << "ERROR::SHADER:PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//所有顶点都是逆时针画，三角形的中心坐标是（0,0）
	//the verte information comes below
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	//the data should be transfered to the memory on the Graphics Card
	GLuint VAO, VBO;//vertexArrayObject：存数据,vertexBufferObject：传数据
	glGenVertexArrays(1, &VAO);//创建VAO
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//transfer the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//set the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//0:对应的location=0；3:3个变量；GL_FLOAT：浮点型；(GLvoid*) 0：从vertices数组中的第一个开始
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);




	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();//有无事件（点击、键盘操作）
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//rgb和透明度
		glClear(GL_COLOR_BUFFER_BIT);

		//Bind the shader
		glUseProgram(shaderProgram);
		//Darw the triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);//画面更流畅，显示这一帧的同时画下一帧
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
*/