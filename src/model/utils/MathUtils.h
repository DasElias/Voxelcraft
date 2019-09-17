#pragma once

#include <glm/vec3.hpp>

namespace vc {
	float const PI = 3.14159265359f;

	int vcround(float value);
	int vcroundY(float value);

	glm::ivec3 convertWorldToChunkCoordinates(glm::ivec3 coords);
	glm::ivec3 convertWorldToChunkCoordinates(glm::vec3 coords);
	glm::ivec3 convertWorldToChunkCoordinates(float x, float y, float z);
	int convertWorldToChunkValue(float value);

	glm::ivec3 convertWorldToInChunkCoordinates(glm::ivec3 coords);
	glm::ivec3 convertWorldToInChunkCoordinates(glm::vec3 coords);
	glm::ivec3 convertWorldToInChunkCoordinates(float x, float y, float z);
	int convertWorldToInChunkValue(float value);

	int convertChunkToWorldValue(int chunk, int inChunk);

	float interpolateLinear(float x, float x1, float y1, float x2, float y2);

	int sgn(float value);
	float toRadiands(float deg);
	float toDegrees(float rad);
}
