#pragma once

#include <glm/vec3.hpp>

namespace vc {
	namespace model {
		class PlayerPosition : public glm::vec3 {
			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				PlayerPosition(float x, float y, float z);
				PlayerPosition(const PlayerPosition&) = default;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				int getIntX() const;
				int getIntY() const;
				int getIntZ() const;
		};
	}
}
