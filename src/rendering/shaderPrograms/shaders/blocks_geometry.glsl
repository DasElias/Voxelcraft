const char* blocks_geometry = R"(

#version 410 core


layout(points) in;
layout(triangle_strip, max_vertices = 4) out;






	

uniform mat4 projectionViewMatrix;

in VS_OUT {
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
} gs_in[1];

out GS_OUT {
	float visibility;
	vec3 texCoords;
	vec3 normal;
} gs_out;







void main(void) {

	gl_Position = gs_in[0].position0;
	gs_out.visibility = gs_in[0].visibility0;
	gs_out.texCoords = gs_in[0].tex0;
	gs_out.normal = gs_in[0].normal;
	EmitVertex();
	
	gl_Position = gs_in[0].position1;
	gs_out.visibility = gs_in[0].visibility1;
	gs_out.texCoords = gs_in[0].tex1;
	gs_out.normal = gs_in[0].normal;
	EmitVertex();
	
	gl_Position = gs_in[0].position2;
	gs_out.visibility = gs_in[0].visibility2;
	gs_out.texCoords = gs_in[0].tex2;
	gs_out.normal = gs_in[0].normal;
	EmitVertex();
	
	gl_Position = gs_in[0].position3;
	gs_out.visibility = gs_in[0].visibility3;
	gs_out.texCoords = gs_in[0].tex3;
	gs_out.normal = gs_in[0].normal;
	EmitVertex();
	
	EndPrimitive();
}

)";


