const char* blocks_vertex = R"(

#version 410 core

const int FACING_GM = 0x7;
const int TEX_ID_GM = 0xFFF8;

const float DENSITY = 0.05755;


in vec3 in_position;
in int in_texId_facing;


uniform mat4 projectionViewMatrix;

out VS_OUT {	
	vec4 position0;
	vec4 position1;
	vec4 position2;
	vec4 position3;
	
	float visibility0;
	float visibility1;
	float visibility2;
	float visibility3;
	
	vec3 tex0;
	vec3 tex1;
	vec3 tex2;
	vec3 tex3;
	
	vec3 normal;
} vs_out;

uniform vec2 TEXTURE[24] = vec2[24] (
	vec2(1, 1),
	vec2(1, 0),
	vec2(0, 1),
	vec2(0, 0),
	
	vec2(1, 1),
	vec2(1, 0),
	vec2(0, 1),
	vec2(0, 0),
	
	vec2(1, 0),
	vec2(0, 0),
	vec2(1, 1),
	vec2(0, 1),
	
	vec2(0, 1),
	vec2(1, 1),
	vec2(0, 0),
	vec2(1, 0),
	
	vec2(0, 1),
	vec2(1, 1),
	vec2(0, 0),
	vec2(1, 0),
	
	vec2(1, 1),
	vec2(1, 0),
	vec2(0, 1),
	vec2(0, 0)
);


vec2 getTextureData(int texData, int vertexIndex) {
	return TEXTURE[texData * 4 + vertexIndex];
	
}

uniform vec3 NORMALS[6] = vec3[6](vec3(0, 1, 0),
						vec3(0, 0, -1),
						vec3(0, -1, 0),
						vec3(0, 0, 1),
						vec3(1, 0, 0),
						vec3(-1, 0, 0));


vec3 getNormal(in int normalData) {
	return NORMALS[normalData];	
}

uniform vec3 DATA[24] = vec3[24](
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 1.0, 1.0),
	vec3(1.0, 1.0, 0.0),
	vec3(1.0, 1.0, 1.0),
	
	vec3(0.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(1.0, 0.0, 0.0),
	vec3(1.0, 1.0, 0.0),
	
	vec3(0.0, 0.0, 0.0),
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 0.0, 1.0), 
	vec3(1.0, 0.0, 1.0),
	
	vec3(0.0, 0.0, 1.0),
	vec3(1.0, 0.0, 1.0),
	vec3(0.0, 1.0, 1.0),
	vec3(1.0, 1.0, 1.0),
	
	vec3(0.0, 0.0, 0.0),
	vec3(0.0, 0.0, 1.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 1.0, 1.0),
	
	vec3(1.0, 0.0, 0.0),
	vec3(1.0, 1.0, 0.0),
	vec3(1.0, 0.0, 1.0),
	vec3(1.0, 1.0, 1.0));

vec4 getData(in int normalData, in int vertexIndex) {
	return vec4(DATA[normalData * 4 + vertexIndex], 0);
}


/**
 * This method is responsible for calculating the fog and takes a vec4 with the worldPosition of the vertex
 * as input, which have to be already transformed by the matrices. It returns a value between 0 and 1. 
 */
float computeVisibility(in vec4 worldPosition) {
	float distanceFromCamera = length(vec3(0, 0, 0) - worldPosition.xyz);
	
	// ZWEITE METHODE:
	//logistisches Wachstum: max / (1 + ((max - min) / min) * e^(-DENSITY * distanceFromCamera)
	float visibility = 1 / (1 + ((1 - 0.001) / 0.001) * exp(-DENSITY * distanceFromCamera));
	visibility = 1 - visibility;
	
	clamp(visibility, 0.0, 1.0);
	return visibility;
}


void main(void) {
	int facing = in_texId_facing & FACING_GM;
	int texId = (in_texId_facing & TEX_ID_GM) >> 3;

	
	vec4 vertexPos = vec4(in_position.xyz, 1.0);	
	
	vs_out.position0 = projectionViewMatrix * (vertexPos + getData(facing, 0));
	vs_out.position1 = projectionViewMatrix * (vertexPos + getData(facing, 1));
	vs_out.position2 = projectionViewMatrix * (vertexPos + getData(facing, 2));
	vs_out.position3 = projectionViewMatrix * (vertexPos + getData(facing, 3));
	
	vs_out.visibility0 = computeVisibility(vs_out.position0);
	vs_out.visibility1 = computeVisibility(vs_out.position1);
	vs_out.visibility2 = computeVisibility(vs_out.position2);
	vs_out.visibility3 = computeVisibility(vs_out.position3);
	
	vs_out.tex0 = vec3(getTextureData(facing, 0), texId);
	vs_out.tex1 = vec3(getTextureData(facing, 1), texId);
	vs_out.tex2 = vec3(getTextureData(facing, 2), texId);
	vs_out.tex3 = vec3(getTextureData(facing, 3), texId);
	
	vs_out.normal = getNormal(facing);
	
}

)";