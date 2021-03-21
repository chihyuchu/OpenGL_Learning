#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 u_Color;

out vec3 our_Color;
uniform	mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	our_Color = u_Color;
};

#shader fragment
#version 330 core

in vec3 our_Color;
out vec4 color;
uniform float brightness;

void main()
{	
	//color = vec4(brightness * u_Color.x, brightness * u_Color.y, brightness * u_Color.z, 1.0f);
	color = brightness * vec4(our_Color, 1.0f);
};