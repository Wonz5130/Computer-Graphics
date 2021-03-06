#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

//SOIL2
//Linux 用的是 \, 但是 / 在 Windows 和 Linux 下都可以用
#include "SOIL2/SOIL2.h"  
#include "SOIL2/stb_image.h"

const GLint WIDTH = 800, HEIGHT = 600;  //新建窗口 
int i = 1;
//the date should be transfered to the memory on the Graphics Card,传到显存
GLuint VAO, VBO, VAO1, VBO1;  //VAO:Vertex Array Object, 顶点数组对象   VBO:Vertex Buffer Object, 顶点缓冲对象  传数据
//transfer the index
GLuint EBO, EBO1;  //创建索引缓冲对象

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
	//设置顶点属性指针
	//第一个参数指定我们要配置的顶点属性。location 的值。
	//第二个参数指定顶点属性的大小。
	//第三个参数指定数据的类型，这里是 GL_FLOAT。
	//第四个参数：是否希望数据被标准化，即所有数据都会被映射到 0 到 1 之间。
	//第五个参数叫做步长，即连续的顶点属性组之间的间隔。
	//最后一个参数：位置数据在缓冲中起始位置的偏移量。
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));  //0:对应调色器里 location 的值；3:对应 vec3 三个量；GL_FLOAT:vec 都是浮点型；8*sizeof(GLfloat):步长为 8，每次跨越 8 个；(GLvoid *)(3 * sizeof(GLfloat)):从第 3 个位置开始
	glEnableVertexAttribArray(0);
	//设置纹理属性指针
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//设置颜色属性指针
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(2);
}

void transfer1() {
	//set the attribute
	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	//设置纹理属性指针
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

//二维:S T   三维:S T R
//i:整型    f:浮点型
//纹理环绕方式
void parameter() {
	//第一个参数指定了纹理目标：我们使用的是 2D 纹理，因此纹理目标是 GL_TEXTURE_2D。
	//第二个参数需要我们指定设置的选项与应用的纹理轴。我们打算配置的是WRAP选项，并且指定S和T轴。
	//最后一个参数需要我们传递一个环绕方式。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //2D 纹理，横向坐标，环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //2D 纹理，纵向坐标，环绕方式
	//纹理过滤
    //GL_NEAREST:选取靠的最近的点，关注点清晰度可以，但边缘会模糊。
	//GL_LINEAR:整体看效果稍微模糊。
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //缩小的情况指定纹理
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //放大的情况指定纹理
}

int main()
{
	glfwInit();  //初始化
	//OpenGL 版本, 用的是新版的 OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//窗口设置
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //我们使用的是核心模式 (Core-profile)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // must for Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  //改为 GL_TRUE，改变窗口，纵横比会变
	//创建一个窗口对象
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL B16112011", nullptr,
		nullptr);  //窗口名字改成自己的学号, 最后两个参数暂时忽略
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// next two lines are for mac retina display，获取实际像素
	//为 glViewport 获取窗口的大小
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);  //获取窗口大小

	//设置当前的窗口上下文
	glfwMakeContextCurrent(window);  //可以新建很多 window

	//初始化 GLEW
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}

	//调用 glViewport 函数来设置窗口维度
	glViewport(0, 0, screenWidth, screenHeight);  //从（0,0）开始画点，直到 WIDTH 和 HEIGHT

	//vs 是顶点调色器，frag 是边缘调色器
	Shader ourShader = Shader("core1.vs", "core1.frag");  //文件相对路径

    //使用纹理坐标更新顶点数据
	GLfloat vertices[] = {
		//color              //position              //texture
		-0.1f, 0.5f, 0.0f,   -0.1f, 0.5f, 0.0f,      1.0f, 1.0f,  //右上角
		-0.1f, -0.5f, 0.0f,  -0.1f, -0.5f, 0.0f,     1.0f, 0.0f,  //右下角
		-0.9f, -0.5f, 0.0f,  -0.9f, -0.5f, 0.0f,     0.0f, 0.0f,  //左下角
		-0.9f, 0.5f, 0.0f,    -0.9f, 0.5f, 0.0f,      0.0f, 1.0f   //左上角
	};
	//索引数组
	unsigned int indices[] = {
		0,1,3,  //第一个三角形
		1,2,3  //第二个三角形
	};

	GLfloat vertices1[] = {
		//position             //texture
		0.9f, 0.5f, 0.0f,      1.0f, 1.0f,  //右上角
		0.9f, -0.5f, 0.0f,     1.0f, 0.0f,  //右下角
		0.1f, -0.5f, 0.0f,     0.0f, 0.0f,  //左下角
		0.1f, 0.5f, 0.0f,      0.0f, 1.0f   //左上角
	};

	//the date should be transfered to the memory on the Graphics Card,传到显存
	glGenVertexArrays(1, &VAO);  //创建 VAO, 顶点数组对象
	glGenBuffers(1, &VBO);  //创建 VBO, 顶点缓冲对象
	glBindVertexArray(VAO);  //绑定VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //把新创建的缓冲绑定到 GL_ARRAY_BUFFER 目标上
	// transfer the data：传数据, 把用户定义的顶点数据复制到缓冲的内存中
	//第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到 GL_ARRAY_BUFFER 目标上。
	//第二个参数指定传输数据的大小 (以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
	//第三个参数是我们希望发送的实际数据。
	//第四个参数指定了我们希望显卡如何管理给定的数据。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //静态访问，数据几乎不修改
	//set the attribute
	//设置顶点属性指针,设置顶点属性指针
	transfer();
	//transfer(VAO, VBO, vertices, 20, indices, 6);

	//第 2 个
	glGenVertexArrays(1, &VAO1);  //创建 VAO1, 顶点数组对象
	glGenBuffers(1, &VBO1);  //创建 VBO1, 顶点缓冲对象
	glBindVertexArray(VAO1);  //绑定VAO1
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);  //把新创建的缓冲绑定到 GL_ARRAY_BUFFER 目标上
	// transfer the data：传数据, 把用户定义的顶点数据复制到缓冲的内存中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);  //静态访问，几乎不修改
	//set the attribute
	//设置顶点属性指针,设置顶点属性指针
	transfer1();
	//transfer(VAO1, VBO1, vertices1, 20, indices1, 6);

	//transfer the index
	glGenBuffers(1, &EBO);
	//把索引复制到缓冲里供 OpenGL 使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//把用户定义的数据复制到当前绑定缓冲中
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);  //解绑 VBO
	glBindVertexArray(0);  //解绑 VAO

	glGenBuffers(1, &EBO1);
	//把索引复制到缓冲里供 OpenGL 使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);  //解绑 VBO1
	glBindVertexArray(0);  //解绑 VAO1

	GLuint texture, texture1;
	int width, height;

	glGenTextures(1, &texture);  //生成纹理
	glBindTexture(GL_TEXTURE_2D, texture); //绑定 2D 纹理

	//纹理环绕方式
	parameter();

	unsigned char* image = SOIL_load_image("1.jpg",
		&width, &height, 0, SOIL_LOAD_RGBA);  //读取它的 RGBA 信息，0 不知道是什么意思
	//第一个参数指定了纹理目标。
	//第二个参数为纹理指定多级渐远纹理的级别。第 0 层。
	//第三个参数告诉 OpenGL 我们希望把纹理储存为何种格式。
	//第四个和第五个参数设置最终的纹理的宽度和高度。
	//下个参数应该总是被设为 0 。
	//第七第八个参数定义了源图的格式和数据类型。
	//最后一个参数是真正的图像数据。
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, image);  //专门贴纹理, 指定传多大的文件空间, 第一个 0 表示第 0 层, 按照 RGBA 的顺序, 第二个 0 不用管, 存成 unsigned_byte 的格式
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap:图片金字塔，1/4。
	glBindTexture(GL_TEXTURE_2D, 0);  //解绑纹理

	//第 2 个
	glGenTextures(1, &texture1);  //生成
	glBindTexture(GL_TEXTURE_2D, texture1); //绑定 2D 纹理

	unsigned char* image1 = SOIL_load_image("6.jpg",
		&width, &height, 0, SOIL_LOAD_RGBA);  //读取它的 RGBA 信息
	
	//纹理环绕方式
	parameter();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, image1);  //专门贴纹理, 指定传多大的文件空间, 第一个 0 表示第 0 层, 按照 RGBA 的顺序, 第二个 0 不用管, 存成 unsigned_byte 的格式
	glGenerateMipmap(GL_TEXTURE_2D);  //Mipmap:图片金字塔
	glBindTexture(GL_TEXTURE_2D, 0);  //解绑纹理

	//画图
	while (!glfwWindowShouldClose(window) && (i++))
	{
		glfwPollEvents();  //把所有事件系统都取过来：键盘/鼠标等操作
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //设置清空屏幕所用的颜色：RGB，最后一个是透明度
		glClear(GL_COLOR_BUFFER_BIT);  //使用清空屏幕所用的颜色

		//Bind the shader，使用着色器程序
		ourShader.Use();

		glActiveTexture(GL_TEXTURE0);  //第 0 个位置,对应的就是 frag 里面的 uniform, 运行过程中传
		glBindTexture(GL_TEXTURE_2D, texture);  //绑定纹理
		//设置每个着色器采样器属于哪个纹理单元
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture0"), 0);  //对应 frag 里面的 ourTexture0, 在 Program 中找 ourTexture0
		//Draw the rectangle
		glBindVertexArray(VAO);  //使用 VAO，直接绑定
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  //绑定索引
		//第一个参数指定了我们绘制的模式。
		//第二个参数是我们打算绘制顶点的个数。6 个索引。
		//第三个参数是索引的类型。
		//最后一个参数里我们可以指定 EBO 中的偏移量。索引是从 0 开始。
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  //应用纹理，自动把纹理赋值给片段着色器的采样器
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //解绑 EBO
		glBindVertexArray(0);  //解绑 VAO

		//第 2 个
		glActiveTexture(GL_TEXTURE0);  //第 0 个位置,对应的就是 frag 里面的 uniform, 运行过程中传
		
		//纹理变换
		if (i % 1000 < 500) {
			glBindTexture(GL_TEXTURE_2D, texture);  //绑定纹理
			//每个着色器采样器属于哪个纹理单元
			glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture0"), 0);  //对应 frag 里面的 ourTexture0, 在 Program 中找 ourTexture0
		}
		else {
			glBindTexture(GL_TEXTURE_2D, texture1);  //绑定纹理
			//每个着色器采样器属于哪个纹理单元
			glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);  //对应 frag 里面的 ourTexture0, 在 Program 中找 ourTexture0
		}

		//Draw the rectangle
		glBindVertexArray(VAO1);  //使用 VAO1，直接绑定
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);  //绑定索引，传递了 GL_ELEMENT_ARRAY_BUFFER 当作缓冲目标
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  //应用纹理, 自动把纹理赋值给片段着色器的采样器
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  //解绑定
		glBindVertexArray(0);

		glfwSwapBuffers(window);  //调用双面进行画,显示一个,另一个在画,画面更流畅
	}
	glDeleteVertexArrays(1, &VAO);  //销毁 VAO
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture);

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);
	glDeleteTextures(1, &texture1);

	glfwTerminate();  //清理所有的资源并正确地退出应用程序
	return 0;
}