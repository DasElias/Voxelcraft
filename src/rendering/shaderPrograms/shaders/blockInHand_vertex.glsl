const char* blockInHand_vertex = R"(
#version 410 core

in vec3 position;
in vec2 texCoords;
in int texId;



out vec3 pass_texCoords;

uniform mat4 projMatrix;

void main(void) {
	pass_texCoords = vec3(texCoords, texId);
	
	vec3 vertexPosition_in = position;
	vec4 vertexPosition_out = projMatrix * vec4(vertexPosition_in, 1.0);
	
	
	gl_Position = vertexPosition_out;

}

)";