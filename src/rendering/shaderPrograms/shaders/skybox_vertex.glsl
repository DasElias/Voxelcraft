const char* skybox_vertex = R"(

#version 400 core

in vec3 position;

out vec3 pass_textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main(void) {
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
	
	pass_textureCoords = position;

}

)";