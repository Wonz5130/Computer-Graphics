#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;  // 立方体贴图纹理采样器

void main()
{
	color = texture(skybox, TexCoords);
}