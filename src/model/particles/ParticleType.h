#pragma once

#include <string>
#include "ParticleTexture.h"

namespace vc {
	namespace model {
		class ParticleType {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				float speed;
				float gravity;
				float lifeLength;
				float scale;
				ParticleTexture texture;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ParticleType(std::string texFileName, float speed, float gravity, float lifeLength, float scale, int stages, bool useAdditiveBlending = false);
				ParticleType(const ParticleType&) = delete;


			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				float getGravity() const;
				float getLifeLength() const;
				float getScale() const;
				float getSpeed() const;
				const ParticleTexture& getTexture() const;
		};

	}
}
