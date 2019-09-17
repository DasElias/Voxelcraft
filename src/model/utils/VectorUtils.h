#pragma once
#include <glm/glm.hpp>

namespace vc {
	void set(glm::vec2& vec, float x, float y);
	void set(glm::vec3& vec, float x, float y, float z);
	void set(glm::vec4& vec, float x, float y, float z, float a);

	void set(glm::ivec2& vec, int x, int y);
	void set(glm::ivec3& vec, int x, int y, int z);
	void set(glm::ivec4& vec, int x, int y, int z, int a);

}

