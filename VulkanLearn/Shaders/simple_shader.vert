#version 450

layout (location = 0) in vec2 postion;
layout (location = 1) in vec3 color;

//layout (location = 0) out vec3 frag_color;

layout(push_constant) uniform Push{
	mat2 transform;
	vec2 offset;
	vec3 color;
} push;

void main()
{
	gl_Position = vec4(push.transform * postion + push.offset, 0.5, 1.0);
}