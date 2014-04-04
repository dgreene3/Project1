#version 400

in vec2 UV;

out vec4 FragColor;

uniform sampler2D floorSampler;

void main() {

	FragColor = texture(floorSampler, UV);

}