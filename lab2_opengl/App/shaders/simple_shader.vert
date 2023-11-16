#version 460

layout (location = 0) in vec3 a_coords;

void main(){
	
	gl_Position = vec4(a_coords, 1.0);

	gl_PointSize = 5.f;
}