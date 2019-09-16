#include "Particle.h"

#include <Eigen/Geometry>

#include "../utils/MathUtils.h"
namespace vc::model {

	vc::model::Particle::Particle(glm::vec3 position, glm::vec3 velocity, float gravity,
			float lifeLength, float rotation, float scale, ParticleTexture texture) :

			position(position),
			velocity(velocity),
			gravity(gravity),
			lifeLength(lifeLength),
			rotation(rotation),
			scale(scale),
			texture(texture) {
	}

	void vc::model::Particle::updateTex(float delta) {
		float progress = (elapsedTime / lifeLength) * texture.getStages();

		int texIndex1 = floorf(progress);
		int texIndex2 = (texIndex1 + 1 >= texture.getStages()) ? texIndex1 : texIndex1 + 1;

		texBlendFactor = fmodf(progress, 1);
		offsetTex1 = getTexOffset(texIndex1);
		offsetTex2 = getTexOffset(texIndex2);
	}

	glm::vec2 vc::model::Particle::getTexOffset(int index) {
		return glm::vec2(0, (texture.getWidth() / texture.getHeight()) * index);
	}

	bool vc::model::Particle::update(float delta, glm::vec3 playerPosition) {
		velocity.y += GRAVITY * gravity * delta;

		glm::vec3 change = velocity;
		change *= delta;
		position += change;

		distanceFromCamera = (playerPosition - position).norm();

		updateTex(delta);

		elapsedTime += delta;
		return elapsedTime < lifeLength;
	}

	glm::vec3 vc::model::Particle::getPosition() const {
		return position;
	}

	void vc::model::Particle::setPosition(glm::vec3 position) {
		this->position = position;
	}

	float vc::model::Particle::getRotation() const {
		return rotation;
	}

	float vc::model::Particle::getScale() const {
		return scale;
	}

	ParticleTexture vc::model::Particle::getTexture() const {
		return texture;
	}

	glm::vec2 vc::model::Particle::getOffsetTex1() const {
		return offsetTex1;
	}

	glm::vec2 vc::model::Particle::getOffsetTex2() const {
		return offsetTex2;
	}

	float vc::model::Particle::getTexBlendFactor() const {
		return texBlendFactor;
	}

	float vc::model::Particle::getDistanceFromCamera() const {
		return distanceFromCamera;
	}

	glm::mat4x4 vc::model::Particle::generateModelMatrix(glm::mat4x4 viewMatrix) const {
		using namespace Eigen;

		Matrix4f transformationMatrix = Affine3f(Translation3f(position)).matrix();

		transformationMatrix(0, 0) = viewMatrix(0, 0);
		transformationMatrix(0, 1) = viewMatrix(1, 0);
		transformationMatrix(0, 2) = viewMatrix(2, 0);
		transformationMatrix(1, 0) = viewMatrix(0, 1);
		transformationMatrix(1, 1) = viewMatrix(1, 1);
		transformationMatrix(1, 2) = viewMatrix(2, 1);
		transformationMatrix(2, 0) = viewMatrix(0, 2);
		transformationMatrix(2, 1) = viewMatrix(1, 2);
		transformationMatrix(2, 2) = viewMatrix(2, 2);


		transformationMatrix *= Affine3f(AngleAxisf(toRadiands(rotation), Vector3f::UnitZ())).matrix();
		transformationMatrix *= Affine3f(Scaling(scale)).matrix();

		return transformationMatrix;
	}

}
