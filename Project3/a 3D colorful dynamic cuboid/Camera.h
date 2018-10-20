//
//  Camera.h
//  Course 3
//
//  Created by rui huang on 10/18/17.
//  Copyright © 2017 rui huang. All rights reserved.
//

#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat YAW         = -90.0f;
const GLfloat PITCH       = 0.0f;
const GLfloat SPEED       = 6.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM        = 45.0f;

// An abstract camera class that processes input and calculates
    //the corresponding Eular Angles, Vectors and Matrices for OpenGL
class Camera
{
public:
    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = 
		glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw 
		= YAW, GLfloat pitch = PITCH):front(glm::vec3(0.0f, 0.0f 
		, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
    {
        this->position = position;  //相机的起点
        this->worldUp = up;  //向前的量
        this->yaw = yaw;  //仰角：飞机上下动
        this->pitch = pitch;  //飞机左右动
        this->updateCameraVectors();  //更新：建立坐标系
    }
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX
		, GLfloat upY, GLfloat upZ, GLfloat yaw = YAW, GLfloat 
		pitch = PITCH):front(glm::vec3(0.0f, 0.0f, -1.0f))
		, movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
    {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }
    
    void ProcessKeyboard( Camera_Movement direction, GLfloat deltaTime)
    {
		//乘以 deltaTime：消除电脑性能，控制时间一样长
        GLfloat velocity = this->movementSpeed * deltaTime;
        //处理键盘
		//向前：加上向前
        if (direction == FORWARD) {
            this->position += this->front * velocity;
        }
        //向后：减去向前
        if (direction == BACKWARD) {
            this->position -= this->front * velocity;
        }
        
        if (direction == LEFT) {
            this->position -= this->right * velocity;
        }
        
        if (direction == RIGHT) {
            this->position += this->right * velocity;
        }
    }
    
    void ProcessMouseMovement( GLfloat xOffset, GLfloat yOffset, GLboolean constrainPith = true)
    {
        xOffset *= this->mouseSensitivity;
        yOffset *= this->mouseSensitivity;
        
        this->yaw   += xOffset;
        this->pitch += yOffset;
        
        if (constrainPith) {
			//设置界限 < 90.0f，超过 90 度就会失效，因为直角是 90 度
            if (this->pitch >89.0f) {
                this->pitch = 89.0f;
            }
            if (this->pitch < -89.0f) {
                this->pitch = -89.0f;
            }
            
        }
        this->updateCameraVectors();
    }
    
    void ProcessMouseScroll( GLfloat yOffset)
    {
    }
    
    glm::mat4 GetViewMatrix()
    {
		//lookAt：观察矩阵：位置；目标；上向量
        return glm::lookAt( this->position, this->position+this->front, this->up);
    }
    
    GLfloat GetZoom()
    {
        return this->zoom;
    }
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    GLfloat yaw;
    GLfloat pitch;
    
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;
    void updateCameraVectors()
    {
        glm::vec3 front;
		//极坐标系下的：x、y、z轴
        front.x = cos( glm::radians(this->pitch)) * cos( glm::radians(this->yaw));  //x 轴向前
        front.y = sin( glm::radians(this->pitch));  //y 轴向上
        front.z = cos( glm::radians(this->pitch)) * sin( glm::radians(this->yaw));  //z 轴向右
        this->front = glm::normalize( front );  //向前
        
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));  //向右：向前 × 向上
        
        this->up = glm::normalize(glm::cross(this->right, this->front));  //向上：向右 × 向前
        
    }
};