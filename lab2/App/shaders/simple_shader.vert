#version 460

layout (location = 0) in vec3 a_coords;
layout(location = 1) in vec4 a_colors;

out vec4 fragColor;

void main(){

	fragColor = a_colors;
	gl_Position = vec4(a_coords, 1.0);
}