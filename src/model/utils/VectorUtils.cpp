#include "VectorUtils.h"

namespace vc {
	void set(glm::vec2& vec, float x, float y) {
		vec.x = x;
		vec.y = y;
	}

	void set(glm::vec3& vec, float x, float y, float z) {
		vec.x = x;
		vec.y = y;
		vec.z = z;
	}

	void set(glm::vec4& vec, float x, float y, float z, float a) {
		vec.x = x;
		vec.y = y;
		vec.z = z;
		vec.a = a;
	}

	void set(glm::ivec2& vec, int x, int y) {
		vec.x = x;
		vec.y = y;
	}

	void set(glm::ivec3& vec, int x, int y, int z) {
		vec.x = x;
		vec.y = y;
		vec.z = z;
	}

	void set(glm::ivec4& vec, int x, int y, int z, int a) {
		vec.x = x;
		vec.y = y;
		vec.z = z;
		vec.a = a;
	}



}

