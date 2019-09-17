#include "PlayerPosition.h"
#include "utils/MathUtils.h"
#include <glm/vec3.hpp>

namespace vc {
	PlayerPosition::PlayerPosition(float x, float y, float z) :
			glm::vec3(x, y, z) {
	}


	int PlayerPosition::getIntX() const {
		return vcround(x);
	}

	int PlayerPosition::getIntY() const {
		return vcroundY(y);
	}

	int PlayerPosition::getIntZ() const {
		return vcround(z);
	}

}

