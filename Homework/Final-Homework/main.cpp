//
//  main.cpp
//  Course 4
//
//  Created by rui huang on 10/27/17.
//  Copyright © 2017 rui huang. All rights reserved.
//


#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Shader.h"

#include "SOIL2/SOIL2.h"
#include "SOIL2/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Model.h"
#include "Mesh.h"
#include "Light.h"
const GLint WIDTH = 800, HEIGHT = 800;
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
//
GLuint loadCubemap(vector<const GLchar*> faces);
void DoMovement();

Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
bool firstMouse = true;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL B16112011", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Set the required callback function
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glDepthFunc(GL_LESS);
	Shader shader("shaders/modelLoad.vs", "shaders/modelLoad.frag");
	Shader lightShader("shaders/core.vs", "shaders/core.frag");
	Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.frag");
	Shader volumeShader("shaders/shadow_volume.vs", "shaders/shadow_volume.frag", "shaders/shadow_volume.geom");
	Shader floor("shaders/plane.vs", "shaders/plane.frag");
	//Shader floor("shaders/silhouette.vs", "shaders/silhouette.gs");
	// Load models
	Model ourModel((GLchar *)"models/nanosuit.obj");
	Light lightModel = Light();

	glm::vec3 LightPos1 = glm::vec3(1.0f, 1.5f, 0.0f);
	//glm::vec3 LightPos1 = glm::vec3(15.0f, 1.5f, 2.5f);  //横向光源
	//glm::vec3 LightPos2 = glm::vec3(0.0f, 5.0f, 0.0f);  //第一个：左右  第二个：上下  第三个：前后
	//glm::vec3 LightPos3 = glm::vec3(1.3f, 0.0f, 0.0f);

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	GLfloat plainVertices[] = {
		15.0f, 0.0f, 15.0f,     //1.0f, 0.0f, 0.0f,
		-15.0f, 0.0f, 15.0f,    //1.0f, 0.0f, 0.0f,
		15.0f, 0.0f, -15.0f,    //1.0f, 0.0f, 0.0f,
		-15.0f, 0.0f, -15.0f,   //1.0f, 0.0f, 0.0f

	};

	unsigned int indices[] = {
	0,1,3,
	1,2,3
	};

	GLuint VBO[1], VAO[1];
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plainVertices), plainVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("skybox/right.jpg");
	faces.push_back("skybox/left.jpg");
	faces.push_back("skybox/top.jpg");
	faces.push_back("skybox/bottom.jpg");
	faces.push_back("skybox/back.jpg");
	faces.push_back("skybox/front.jpg");
	GLuint cubemapTexture = loadCubemap(faces);


	while (!glfwWindowShouldClose(window)) {

		// Get the delta time between frames
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastTime;
		lastTime = currentFrame;
		//LightPos1 = glm::rotate(LightPos1, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
		//LightPos2 = glm::rotate(LightPos2, 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));

		// Check and call events
		glfwPollEvents();
		DoMovement();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);

		// Draw skybox first
		glDepthMask(GL_FALSE);// 关掉深度
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		//view = camera.GetViewMatrix();
		//glm::mat4(glm::mat3(camera.GetViewMatrix()));
		//glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		//projection = glm::perspective(glm::radians(camera.GetZoom()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// depth
		//view = camera.GetViewMatrix();
		glDepthMask(GL_TRUE);
		lightShader.Use();
		view = camera.GetViewMatrix();
		//projection = glm::perspective(glm::radians(camera.GetZoom()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		model = glm::mat4(1.0f);
		model = glm::translate(model, LightPos1);
		//model = glm::translate(model, LightPos1 + glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//lightModel.Draw(lightShader);
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		lightModel.Draw(lightShader);

		/*
		// 第二个光源
		model = glm::mat4(1.0f);
		model = glm::translate(model, LightPos2);
		//model = glm::translate(model, LightPos2 + glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		lightModel.Draw(lightShader);
		*/

		/*
		// 第三个光源
		model = glm::mat4(1.0f);
		//model = glm::translate(model, LightPos3);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		lightModel.Draw(lightShader);
		*/


		// model
		view = camera.GetViewMatrix();
		glDepthMask(GL_TRUE);
		// 关闭颜色缓存写入
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		shader.Use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.0f, -10.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(shader.Program, "ViewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(shader.Program, "LightPos1"), LightPos1.x, LightPos1.y, LightPos1.z);
		//glUniform3f(glGetUniformLocation(shader.Program, "LightPos2"), LightPos2.x, LightPos2.y, LightPos2.z);
		//glUniform3f(glGetUniformLocation(shader.Program, "LightPos"), LightPos3.x, LightPos3.y, LightPos3.z);
		glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 64.0f);
		ourModel.Draw(shader);
		glDepthMask(GL_TRUE);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		// floor
		floor.Use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, -20.0f));
		glUniformMatrix4fv(glGetUniformLocation(floor.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(floor.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(floor.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// shadow volume
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glEnable(GL_DEPTH_CLAMP);
		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glClearStencil(1);
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		// zfail
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		volumeShader.Use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(volumeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(volumeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(volumeShader.Program, "ProjMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(glGetUniformLocation(volumeShader.Program, "LightPosition"), (LightPos1).x, (LightPos1).y, (LightPos1).z);
		ourModel.Draw(volumeShader);
		glDisable(GL_DEPTH_CLAMP);
		glDisable(GL_STENCIL_TEST);

		// ambient light
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		floor.Use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, -20.0f));
		glUniform1f(glGetUniformLocation(floor.Program, "ambientStrength"), 0.1f);
		glUniform1f(glGetUniformLocation(floor.Program, "diffuseStrength"), 0.0f);
		glUniform1f(glGetUniformLocation(floor.Program, "specularStrength"), 0.0f);
		glUniformMatrix4fv(glGetUniformLocation(floor.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(floor.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(floor.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(floor.Program, "objectColor"), 1.0f, 0.5f, 0.3f);
		glUniform3f(glGetUniformLocation(floor.Program, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(floor.Program, "lightPos"), LightPos1.x, LightPos1.y, LightPos1.z);
		glUniform3f(glGetUniformLocation(floor.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glStencilFunc(GL_ALWAYS, 1, 0xff);
		shader.Use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniform1f(glGetUniformLocation(shader.Program, "ambientStrength"), 0.4f);
		glUniform1f(glGetUniformLocation(shader.Program, "diffuseStrength"), 0.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "specularStrength"), 0.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(shader.Program, "ViewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(shader.Program, "LightPos1"), LightPos1.x, LightPos1.y, LightPos1.z);
		glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 64.0f);
		ourModel.Draw(shader);

		// diffuse light
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_STENCIL);
		glStencilFunc(GL_EQUAL, 1, 0xff);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
		shader.Use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniform1f(glGetUniformLocation(shader.Program, "ambientStrength"), 0.0f);
		glUniform1f(glGetUniformLocation(shader.Program, "diffuseStrength"), 0.6f);
		glUniform1f(glGetUniformLocation(shader.Program, "specularStrength"), 0.9f);
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(shader.Program, "ViewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(shader.Program, "LightPos"), LightPos1.x, LightPos1.y, LightPos1.z);
		glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 64.0f);
		ourModel.Draw(shader);

		glStencilFunc(GL_EQUAL, 1, 0xff);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
		floor.Use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, -20.0f));
		glUniform1f(glGetUniformLocation(floor.Program, "ambientStrength"), 0.0f);
		glUniform1f(glGetUniformLocation(floor.Program, "diffuseStrength"), 0.6f);
		glUniform1f(glGetUniformLocation(floor.Program, "specularStrength"), 0.9f);
		glUniformMatrix4fv(glGetUniformLocation(floor.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(floor.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(floor.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(floor.Program, "objectColor"), 1.0f, 0.5f, 0.3f);
		glUniform3f(glGetUniformLocation(floor.Program, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(floor.Program, "lightPos"), LightPos1.x, LightPos1.y, LightPos1.z);
		glUniform3f(glGetUniformLocation(floor.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);

		glEnable(GL_CULL_FACE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDisable(GL_CULL_FACE);


		glDisable(GL_STENCIL_TEST);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			keys[key] = true;

		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	// Process the camera direction
	camera.ProcessMouseMovement(xOffset, yOffset);
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

//Skybox
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_LINEAR整体看效果稍微模糊
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//边界处采用纹理边缘自己的的颜色，和边框无关
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
