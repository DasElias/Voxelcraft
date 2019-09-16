const char* twod_fragment = R"(
#version 400 core

in vec2 pass_texCoords;

out vec4 out_Color;

uniform sampler2D tex;

void main(void) {
	out_Color = texture(tex, pass_texCoords);
	
	if(out_Color.a < 0.5) discard;
	out_Color = vec4(0.5, 0.5, 0.5, 1.0);

}
)";