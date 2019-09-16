#pragma once

#include "ParticleTexture.h"
#include <Eigen/Core>

namespace vc {
	namespace model {
		class Particle {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			private:
				static float const GRAVITY;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				glm::vec3 position = glm::vec3::Zero();
				glm::vec3 velocity = glm::vec3::Zero();
				float gravity;
				float lifeLength;
				float rotation;
				float scale;
				float elapsedTime = 0;

				ParticleTexture texture;

				glm::vec2 offsetTex1 = glm::vec2::Zero();
				glm::vec2 offsetTex2 = glm::vec2::Zero();
				float texBlendFactor;

				float distanceFromCamera;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				Particle(glm::vec3 position, glm::vec3 velocity, float gravity, float lifeLength, float rotation, float scale, ParticleTexture texture);

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			private:
				void updateTex(float delta);
				glm::vec2 getTexOffset(int index);

			public:
				bool update(float delta, glm::vec3 playerPosition);
				glm::vec3 getPosition() const;
				void setPosition(glm::vec3 position);
				float getRotation() const;
				float getScale() const;
				ParticleTexture getTexture() const;
				glm::vec2 getOffsetTex1() const;
				glm::vec2 getOffsetTex2() const;
				float getTexBlendFactor() const;
				float getDistanceFromCamera() const;
				glm::mat4x4 generateModelMatrix(glm::mat4x4 viewMatrix) const;


		};
	}
}
