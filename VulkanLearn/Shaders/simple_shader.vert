#version 450

layout (location = 0) in vec3 postion;
layout (location = 1) in vec3 color;

layout (location = 0) out vec3 frag_color;

layout(push_constant) uniform Push{
	mat4 transform;
	vec3 color;
} push;

void main()
{
	//gl_Position = vec4(push.transform * postion + push.offset, 0.5, 1.0);
	gl_Position = push.transform * vec4(postion, 1.0);
	frag_color = color;
}