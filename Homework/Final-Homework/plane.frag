#version 430
in vec3 Position;
in vec3 Normal;
//in vec3 ourColor;

//out vec4 color;

uniform vec4 LightPosition;
uniform float LightIntensity;
uniform vec3 u_color;

layout( location = 0 ) out vec4 Ambient;
layout( location = 1 ) out vec4 DiffSpec;

void main() {
    vec3 toLight = normalize(vec3(LightPosition) - Position);
	vec3 toV = normalize(vec3(-Position));
	vec3 r = reflect( -toLight, Normal );
	//color = vec4(ourColor, 0.7f);
	//color = vec4(1.0f, 0.0f, 0.0f, 0.1f);

	float diffuse = max(0.0f, dot(Normal, toLight));
	float specular = pow(max(0.0, dot(r, Normal)), 640.0);

	Ambient = vec4(u_color, 1.0f) * 0.2f *LightIntensity;
    DiffSpec =vec4(u_color, 1.0f)*(diffuse+specular)*LightIntensity+Ambient;
}