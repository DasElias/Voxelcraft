#include "MathUtils.h"
#include "../Chunk.h"
#include <cmath>
#include <math.h>

namespace vc::model {
	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	int vcround(float value) {
		int signum = sgn(value);
		return int((signum == -1) ? -1 * ceil(abs(value)) : floor(value));
	}

	int vcroundY(float value) {
		return int(floor(abs(value)));
	}

	glm::ivec3 convertWorldToChunkCoordinates(glm::ivec3 coords) {
		return convertWorldToChunkCoordinates(float(coords.x), float(coords.y), float(coords.z));
	}

	glm::ivec3 convertWorldToChunkCoordinates(glm::vec3 coords) {
		return convertWorldToChunkCoordinates(coords.x, coords.y, coords.z);
	}

	glm::ivec3 convertWorldToChunkCoordinates(float x, float y, float z) {
		return glm::ivec3(
			vcround(x / Chunk::CHUNK_SIZE_FLOAT),
			vcround(y / Chunk::CHUNK_SIZE_FLOAT),
			vcround(z / Chunk::CHUNK_SIZE_FLOAT)
			);
	}

	int convertWorldToChunkValue(float value) {
		return vcround(value / Chunk::CHUNK_SIZE_FLOAT);
	}



	glm::ivec3 convertWorldToInChunkCoordinates(glm::ivec3 coords) {
		return convertWorldToInChunkCoordinates(float(coords.x), float(coords.y), float(coords.z));
	}

	glm::ivec3 convertWorldToInChunkCoordinates(glm::vec3 coords) {
		return convertWorldToInChunkCoordinates(coords.x, coords.y, coords.z);
	}

	glm::ivec3 convertWorldToInChunkCoordinates(float x, float y, float z) {
		return glm::ivec3(
			convertWorldToInChunkValue(x),
			convertWorldToInChunkValue(y),
			convertWorldToInChunkValue(z)
		);
	}

	int convertWorldToInChunkValue(float value) {
		if(value < 0) return (Chunk::CHUNK_SIZE - 1) - int(fmod(abs(value + 1), Chunk::CHUNK_SIZE_FLOAT));
		else return int(fmod(abs(value), Chunk::CHUNK_SIZE_FLOAT));
	}



	int convertChunkToWorldValue(int chunk, int inChunk) {
		return chunk * Chunk::CHUNK_SIZE + inChunk;
	}



	float interpolateLinear(float x, float x1, float y1, float x2, float y2) {
		return y1 + ((y2 - y1) / (x2 - x1)) * (x - x1);
	}

	int sgn(float val) {
	    return (float(0) < val) - (val < float(0));
	}

	float toRadiands(float deg) {
		return deg * (PI / 180.0f);
	}

	float toDegrees(float rad) {
		return rad * (180.0f / PI);
	}

}

