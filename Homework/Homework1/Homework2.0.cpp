#include <iostream>

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
int i = 1;
//the date should be transfered to the memory on the Graphics Card,传到显存
GLuint VAO, VBO, VAO1, VBO1;  //VAO:Vertex Array Object   VBO:Vertex Buffer Object传数据
//transfer the index
GLuint EBO, EBO1;

/*
void transfer(GLuint VAO, GLuint VBO, GLfloat vertices[], int m,  unsigned int indices[], int n) {
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
}
*/

void transfer() {
	//set the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(GLfloat), (GLvoid *)0);  //0:对应调色器里 location 的值；3:对应 vec3 三个量；GL_FLOAT:浮点型；GL_FALSE:；5*sizeof(GLfloat):对应 Buffer 里传的数据；(GLvoid*)0:从第 0 个位置开始
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));  //1:对应调色器里 color 的值；2:对应纹理 vec2 两个量；GL_FLOAT:浮点型；GL_FALSE:；5*sizeof(GLfloat):每次跨越 5 个；(GLvoid*) (3 * sizeof(GLfloat)):从第 0 个位置开始
	glEnableVertexAttribArray(1);
}

//二维:S T   三维:S T R
//i:整型    f:浮点型
void parameter() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //横向坐标
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //纵向坐标
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //FILTER:滤波器
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //GL_NEAREST:选取靠的最近的点,关注点清晰度可以,但边缘会模糊   GL_LINEAR:整体看效果稍微模糊
}

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

	GLfloat vertices[] = {
		0.0f, 0.5f, 0.0f,      1.0f, 1.0f,  //右上角
		0.0f, -0.5f, 0.0f,     1.0f, 0.0f,  //右下角
		-1.0f, -0.5f, 0.0f,    0.0f, 0.0f,  //左下角
		-1.0f, 0.5f, 0.0f,     0.0f, 1.0f   //左上角
	};
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	GLfloat vertices1[] = {
		1.0f, 0.5f, 0.0f,      1.0f, 1.0f,  //右上角
		1.0f, -0.5f, 0.0f,     1.0f, 0.0f,  //右下角
		-0.0f, -0.5f, 0.0f,    0.0f, 0.0f,  //左下角
		-0.0f, 0.5f, 0.0f,     0.0f, 1.0f   //左上角
	};
	unsigned int indices1[] = {
		0,1,3,
		1,2,3
	};

	//the date should be transfered to the memory on the Graphics Card,传到显存
	glGenVertexArrays(1, &VAO);  //创建 VAO
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);  //设当前直线
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //VAO 和 VBO 成对出现
	// transfer the data：传数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //静态访问，几乎不修改
	//set the attribute
	transfer();
	//transfer(VAO, VBO, vertices, 20, indices, 6);

	//第 2 个
	glGenVertexArrays(1, &VAO1);  //创建 VAO
	glGenBuffers(1, &VBO1);
	glBindVertexArray(VAO1);  //设当前直线
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);  //VAO 和 VBO 成对出现
	// transfer the data：传数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);  //静态访问，几乎不修改
	//set the attribute
	transfer();
	//transfer(VAO1, VBO1, vertices1, 20, indices1, 6);

	//transfer the index
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &EBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint texture, texture1;  //生成纹理
	int width, height;

	glGenTextures(1, &texture);  //生成
	glBindTexture(GL_TEXTURE_2D, texture); //绑定 2D 纹理

	//二维:S T   三维:S T R
	//i:整型    f:浮点型
	parameter();

	unsigned char* image = SOIL_load_image("1.jpg",
		&width, &height, 0, SOIL_LOAD_RGBA);  //读取它的 RGBA 信息

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, image);  //专门贴纹理, 指定传多大的文件空间, 第一个 0 表示第 0 层, 按照 RGBA 的顺序, 第二个 0 不用管, 存成 unsigned_byte 的格式
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap:图片金字塔
	glBindTexture(GL_TEXTURE_2D, 0);

	//第 2 个
	glGenTextures(1, &texture1);  //生成
	glBindTexture(GL_TEXTURE_2D, texture1); //绑定 2D 纹理

	unsigned char* image1 = SOIL_load_image("6.jpg",
		&width, &height, 0, SOIL_LOAD_RGBA);  //读取它的 RGBA 信息
	
	parameter();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, image1);  //专门贴纹理, 指定传多大的文件空间, 第一个 0 表示第 0 层, 按照 RGBA 的顺序, 第二个 0 不用管, 存成 unsigned_byte 的格式
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap:图片金字塔
	glBindTexture(GL_TEXTURE_2D, 0);

	//画图
	while (!glfwWindowShouldClose(window) && (i++) < 500000)
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

		//glfwSwapBuffers(window);  //调用双面进行画，显示一个，另一个在画，画面更流畅

		//第 2 个
		glActiveTexture(GL_TEXTURE0);  //第 0 个位置,对应的就是 frag 里面的 uniform, 运行过程中传
		//glBindTexture(GL_TEXTURE_2D, texture1);  //需要

		if (i % 1000 < 500) {
			glBindTexture(GL_TEXTURE_2D, texture);  //需要
			glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);  //对应 frag 里面的 ourTexture0, 在 Program 中找 ourTexture0
		}
		else {
			glBindTexture(GL_TEXTURE_2D, texture1);  //需要
			glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture0"), 0);  //对应 frag 里面的 ourTexture0, 在 Program 中找 ourTexture0
		}

		//Draw the triangle
		glBindVertexArray(VAO1);  //使用 VAO，直接绑定

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
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

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);
	glDeleteTextures(1, &texture1);

	glfwTerminate();
	return 0;
}