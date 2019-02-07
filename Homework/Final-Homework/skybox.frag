#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;  // ��������ͼ���������

void main()
{
	color = texture(skybox, TexCoords);
}