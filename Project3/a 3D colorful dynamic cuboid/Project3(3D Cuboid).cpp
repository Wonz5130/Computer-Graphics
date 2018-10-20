#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//Shader
#include "Shader.h"

// SOIL2
//Linux 用的是 \, 但是 / 都可以用
#include "SOIL2/SOIL2.h"  
#include "SOIL2/stb_image.h"

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  //需要什么变换，就导入什么文件，具体可以去官网看
#include <glm/gtc/type_ptr.hpp>

//#include "Camera.h"  //当前引用，所以用""

const GLint WIDTH = 800, HEIGHT = 600;  //新建窗口

/*
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
*/

//初始化一个相机
//Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));

bool keys[1024];  //存放获取的所有键盘操作，先存下来再进行操作

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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

	/*
	// Set the required callback function
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	*/

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);  //从（0,0）开始画点，直到 WIDTH 和 HEIGHT
	
	//glEnable(GL_CULL_FACE);  //只显示一半
	//glEnable(GL_DEPTH_TEST);  //深度测试
	//glDepthFunc(GL_LESS);  //深度信息小于当期信息，就把进行测试

	/*
	//启动透明度混合，固定不能改，alpha 线性混合：设置当前为 α ，其他就为 1- α
	glEnable(GL_BLEND);
	//表示把渲染的图像融合到目标区域。也就是说源的每一个像素的alpha都等于自己的alpha，
	//目标的每一个像素的alpha等于1减去该位置源像素的alpha。因此不论叠加多少次，亮度是不变的。
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/

	//vs 是顶点调色器，frag 是边缘调色器
	Shader ourShader = Shader("core1.vs", "core1.frag");  //文件相对路径

	//now the verte information comes below
	/*
	GLfloat vertices[] = {
		//
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};*/
	float vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};
	/*
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	*/

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
		6 * sizeof(GLfloat), (GLvoid *)0);  //0:对应调色器里 location 的值；3:对应 vec3 三个量；GL_FLOAT:浮点型；GL_FALSE:；5*sizeof(GLfloat):对应 Buffer 里传的数据；(GLvoid*)0:从第 0 个位置开始
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));  //0:对应调色器里 location 的值；3:对应 vec3 三个量；GL_FLOAT:浮点型；GL_FALSE:；5*sizeof(GLfloat):对应 Buffer 里传的数据；(GLvoid*)0:从第 0 个位置开始
	glEnableVertexAttribArray(1);

	/*
	//transfer the index
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//画图
	while (!glfwWindowShouldClose(window))
	{
		/*
		GLfloat currentFrame = glfwGetTime();  //屏幕刚画出来的时间
		deltaTime = currentFrame - lastTime;
		lastTime = currentFrame;
		*/
		glfwPollEvents();  //把所有事件系统都取过来：键盘/鼠标等操作
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //窗口背景颜色，RGB，最后一个是透明度
		glClear(GL_COLOR_BUFFER_BIT);
		//Bind the shader
		//glUseProgram(shaderProgram);  //使用调色器,不注释
		ourShader.Use();
		//glm 从 0.9.9 版本起，默认会将矩阵类型初始化为一个零矩阵（所有元素均为 0）
		glm::mat4 transform = glm::mat4(1.0f);  //初始化 4 * 4 单位矩阵

		//旋转
		//GLM 希望它的角度是弧度制，radians 将角度转化为弧度制
		//glfwGetTime()：让图形一直变换，做一个类型转换，用 static_cast<GLfloat>，设为 GLfloat 型
		//glm::vec3(1.0f, 1.0f, 1.0f)，分别绕 x 轴、y 轴、z 轴进行旋转，如果都为 1.0f，就是绕和向量 (1,1,1) 转
		transform = glm::rotate(transform, glm::radians(20.0f) * static_cast<GLfloat>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));

		//缩放，x、y、z 都缩放到原来的 0.5 倍
		transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

		//平移
		//transform = 

		GLuint transLoc = glGetUniformLocation(ourShader.Program, "transform");  //到 vs 找到那个 transform 变量
		//Matrix4fv：4维矩阵，fv：浮点类型
		//transLoc：变量 uniform 的位置
		//1：代表只传入一个矩阵
		//GL_FALSE：不对矩阵进行置换，即不交换矩阵的行和列。GLM 的默认布局就是列主序，所以并不需要置换矩阵
		//最后：直接给出 transform 矩阵数组，这里我们要把矩阵转换成数组的格式传递。
		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(transform));  //glUniformMatrix4fv：四个坐标  glUniform4fv：三个坐标

		//Draw the triangle
		glBindVertexArray(VAO);  //使用 VAO，直接绑定

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawArrays(GL_TRIANGLES, 0, 36);  //画三角形，从第 0 个数据开始画，到最后一个数据（第 3 个）结束
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //解绑定
		glBindVertexArray(0);

		glfwSwapBuffers(window);  //调用双面进行画，显示一个，另一个在画，画面更流畅
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//lDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

/*
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);  //设定关闭窗口
	}

	if(key >= 0 && key < 1024){
		if(action == GLFW_PRESS)
		{
		    keys[key] = true;  //键盘按下去了，就设置为 true，即为1
		}else if (action == GLFW_RELEASE)
		{
			keys[key] = false;  //键盘松开，设为 false
		}
}
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

}
void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}
*/