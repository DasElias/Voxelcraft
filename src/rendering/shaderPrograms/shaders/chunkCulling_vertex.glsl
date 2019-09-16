const char* chunkCulling_vertex = R"(

#version 400 core

in vec3 vertexPosition;

uniform vec3 chunkPosition;
uniform mat4 projectionViewMatrix;

uniform float heightInPercent;

void main(void) {
	vec3 computedVertexPosition = vec3(vertexPosition.x, vertexPosition.y * heightInPercent, vertexPosition.z);

	gl_Position = projectionViewMatrix * vec4(computedVertexPosition + chunkPosition, 1);
	

}
)";