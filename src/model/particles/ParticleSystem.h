#pragma once

#include <string>
#include "ParticleType.h"
#include "../Level.h"

namespace vc {
	namespace model {
		class ParticleSystem {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				float pps;
				ParticleType particleType;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ParticleSystem(float pps, float speed, float gravity, float lifeLength, float scale, std::string texturePath, int textureStages, bool isTextureAdditive = false);
				ParticleSystem(const ParticleSystem&) = delete;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			private:
				void emitParticle(const glm::vec3& systemCenter, Level& target);
			public:
				void generateParticles(const glm::vec3& systemCenter, float delta, Level& target);

		};

	}
}
