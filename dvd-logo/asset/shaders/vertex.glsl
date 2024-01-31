#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 tex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 Posi;
out vec2 ttex;

void main()
{
	ttex = tex;
	vec4 point =  view * model * vec4(vertex, 1.0);

	Posi = model *  vec4(vertex, 1.0);

	gl_Position = projection * point;

}

