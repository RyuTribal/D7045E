#version 460

in vec4 fragColor;

layout(location = 0) out vec4 outColor;

void main(){
	outColor = fragColor;
}