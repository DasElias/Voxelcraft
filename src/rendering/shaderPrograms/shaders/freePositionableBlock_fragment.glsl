const char* freePositionableBlock_fragment = R"(
#version 410 core

in vec3 pass_texCoords;

out vec4 out_Color;

uniform float opacity;
uniform sampler2DArray tex;

void main(void) {
	vec4 color = texture(tex, pass_texCoords);

	float finalOpacity = min(opacity, color.a);

	color.a = finalOpacity;
	out_Color = color;
	
}

)";