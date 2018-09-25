#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D ourTexture0;
void main(){
    color = vec4(texture(ourTexture0, TexCoords).rgb, 1.0f);
};