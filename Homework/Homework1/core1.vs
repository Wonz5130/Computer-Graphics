#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 color;
out vec2 TexCoords;
out vec3 ourColor;

void main(){
    gl_Position = vec4(position, 1.0f);
	TexCoords = vec2(texCoords.x, 1.0f-texCoords.y);
	ourColor = color;
}