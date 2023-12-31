#version 460

layout (location = 0) in vec3 a_coords;
layout (location = 1) in vec4 a_colors;
layout (location = 2) in vec2 a_texture_coords;
layout (location = 3) in vec3 a_normals;

uniform mat4 u_CameraView;
uniform mat4 u_CameraProjection;
uniform vec3 u_CameraPos;
uniform mat4 u_Transform;

out vec3 worldSpacePosition;
out vec3 normal;
out vec2 texCoords;
out vec4 objectColor;
out vec3 cameraPosition;

void main() {
    gl_Position = u_CameraProjection * u_CameraView * u_Transform * vec4(a_coords, 1.0);
    worldSpacePosition = vec3(u_Transform * vec4(a_coords, 1.0));
    normal = mat3(transpose(inverse(u_Transform))) * a_normals;
    cameraPosition = u_CameraPos;
    texCoords = a_texture_coords;
}