#version 460

layout (location = 0) in vec3 a_coords;
layout(location = 1) in vec4 a_colors;

uniform vec3 u_MaterialColor;

uniform mat4 u_CameraMatrix;
uniform mat4 u_Transform;

out vec3 fragColor;

void main(){

	fragColor = u_MaterialColor;
	gl_Position = u_CameraMatrix * u_Transform * vec4(a_coords, 1.0);
}