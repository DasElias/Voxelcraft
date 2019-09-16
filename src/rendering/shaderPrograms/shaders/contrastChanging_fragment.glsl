const char* contrastChanging_fragment = R"(

#version 400 core

in vec2 pass_textureCoords;

out vec4 out_Color;

uniform sampler2D tex;

const float contrast = 0.3;

void main(void) {
	out_Color = texture(tex, pass_textureCoords);
	out_Color.rgb = (out_Color.rgb - 0.5) * (1.0 + contrast) + 0.5;

}
)";