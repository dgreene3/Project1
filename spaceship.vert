#version 400

layout (location = 0) in vec4 VertexPosition;


uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;


void main() {
	//calculate position
	vec4 pos = worldMatrix * VertexPosition; 
	pos = viewMatrix * pos;
	gl_Position = projMatrix * pos;
}