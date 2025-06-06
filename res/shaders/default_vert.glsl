#version 450 core

layout(location = 0) in vec4 aPos;

uniform mat4 uModel;                        
uniform float uCameraW;                       
uniform mat4 uView;                         
uniform mat4 uProj;  

void main()
{
	vec4 p4 = uModel * aPos;

	float wDiff = uCameraW - p4.w;

	vec3 p3 = p4.xyz / wDiff;

	gl_Position = uProj * uView * vec4(p3, 1.0);
}