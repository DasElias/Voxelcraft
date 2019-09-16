#include "ParticleType.h"

namespace vc::model {
	ParticleType::ParticleType(std::string texFileName, float speed, float gravity, float lifeLength, float scale, int stages, bool useAdditiveBlending) :
			speed(speed),
			gravity(gravity),
			lifeLength(lifeLength),
			scale(scale),
			texture(texFileName, stages, useAdditiveBlending) {
	}


	float ParticleType::getGravity() const {
		return gravity;
	}

	float ParticleType::getLifeLength() const {
		return lifeLength;
	}

	float ParticleType::getScale() const {
		return scale;
	}

	float ParticleType::getSpeed() const {
		return speed;
	}

	const ParticleTexture& ParticleType::getTexture() const {
		return texture;
	}

}
