const char* freePositionableBlock_fragment = R"(
#version 410 core

in vec3 pass_texCoords;

out vec4 out_Color;

uniform sampler2DArray tex;

void main(void) {
	out_Color = texture(tex, pass_texCoords);
	
}

)";