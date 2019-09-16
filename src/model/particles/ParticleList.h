#pragma once

#include <Eigen/Core>
#include <functional>
#include <unordered_map>
#include <vector>

#include "Particle.h"
#include "ParticleTexture.h"

namespace vc {
	namespace model {

		typedef std::unordered_map<ParticleTexture, std::vector<Particle>,
				std::function<int(const ParticleTexture&)>, std::function<bool(const ParticleTexture&, const ParticleTexture&)>> ParticleMap;

		class ParticleList {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			private:
				static float const DENSITY;
				static float const MAX_DISTANCE;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				ParticleMap particles;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ParticleList() = default;
				ParticleList(const ParticleList&) = delete;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void update(float delta, glm::vec3 playerPosition);
				void add(Particle& p);
				const ParticleMap& get() const;
		};
	}
}
