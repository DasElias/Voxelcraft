#include "PlayerPosition.h"
#include "utils/MathUtils.h"
#include <glm/vec3.hpp>

namespace vc::model {
	vc::model::PlayerPosition::PlayerPosition(float x, float y, float z) :
			glm::vec3(x, y, z) {
	}


	int PlayerPosition::getIntX() const {
		return vc::model::vcround(x);
	}

	int PlayerPosition::getIntY() const {
		return vc::model::vcroundY(y);
	}

	int PlayerPosition::getIntZ() const {
		return vc::model::vcround(z);
	}

}

