#version 400

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec2 VertexUV;


out vec2 UV;


uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;




void main() {
	//calculate position
	vec4 pos = worldMatrix * VertexPosition; 
	pos = viewMatrix * pos;
	gl_Position = projMatrix * pos;

	UV = VertexUV;
}