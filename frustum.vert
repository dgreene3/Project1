#version 400

layout (location = 0) in vec4 VertexPosition;


uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform mat4 inverseProj;
uniform mat4 inverseView;


void main() {

	//convert to frustum
	vec4 pos = inverseProj * VertexPosition;
	pos = inverseView * pos;

	//draw frustum
	pos = worldMatrix * pos; 
	pos = viewMatrix * pos;
	gl_Position = projMatrix * pos;

}