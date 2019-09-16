const char* blocks_fragment = R"(

#version 400 core


in GS_OUT {
	float visibility;
	vec3 texCoords;
	vec3 normal;
} fs_in;

out vec4 out_Color;

uniform sampler2DArray tex;

uniform vec4 fogColor;


void main(void) {
	vec4 textureColor = texture(tex, fs_in.texCoords);
	out_Color = mix(fogColor, textureColor, fs_in.visibility);
		
}

)";