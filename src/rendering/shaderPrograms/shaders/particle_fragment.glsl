const char* particle_fragment = R"(

#version 400 core


in vec2 pass_textureCoords1;
in vec2 pass_textureCoords2;
in float pass_blendFactor;

out vec4 out_Color;

uniform sampler2D tex;

void main(void) {
	vec4 texColor1 = texture(tex, pass_textureCoords1);
	vec4 texColor2 = texture(tex, pass_textureCoords2);
	
	 out_Color = mix(texColor1, texColor2, pass_blendFactor);
	//out_Color = vec4(pass_textureCoords1.x, pass_textureCoords1.y, 0.0, 1.0);

}

)";