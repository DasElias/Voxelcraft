const char* skybox_fragment = R"(

#version 400 core

in vec3 pass_textureCoords;

out vec4 out_Color;

uniform samplerCube tex;

void main(void) {
	vec4 tex_Color = texture(tex, pass_textureCoords);
	//out_Color = vec4(1.0, 1.0, 0.0, 1.0);
	out_Color = tex_Color;

}

)";