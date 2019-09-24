#include "BasicInHandRenderer.h"
#include "../model/Frustum.h"
#include "../model/utils/TimeUtils.h"
#include "../model/utils/MathUtils.h"
#include "../model/Player.h"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

using namespace vc;
using namespace egui;

namespace vc {
	const float BasicInHandRenderer::MODEL_SCALE = 0.02f;

	BasicInHandRenderer::BasicInHandRenderer() :
			blockInHandProjectionMatrix(10, Frustum::NEAR_PLANE_DISTANCE, Frustum::FAR_PLANE_DISTANCE){

	}

	BasicInHandRenderer::~BasicInHandRenderer() {
	}

	void BasicInHandRenderer::onBlockPlaced() {
		blockPlacedStartTime = getMilliseconds();
	}

	void BasicInHandRenderer::onItemChanged() {
		itemInHandChangedTime = getMilliseconds();
	}

	Level* BasicInHandRenderer::getCurrentLevel() {
		return p_currentLevel;
	}

	void BasicInHandRenderer::setCurrentLevel(Level* p_currentLevel) {
		if(this->p_currentLevel != nullptr) {
			this->p_currentLevel->getBlockPlaceEventManager().removeEventHandler(blockPlaceEventHandler);
			this->p_currentLevel->getBlockBreakEventManager().removeEventHandler(blockBreakEventHandler);
			this->p_currentLevel->getPlayer().getItemInHandChangeManager().removeEventHandler(itemInHandChangeEventHandler);
		}

		this->p_currentLevel = p_currentLevel;

		if(p_currentLevel != nullptr) {
			p_currentLevel->getBlockPlaceEventManager().addEventHandler(blockPlaceEventHandler);
			p_currentLevel->getBlockBreakEventManager().addEventHandler(blockBreakEventHandler);
			p_currentLevel->getPlayer().getItemInHandChangeManager().addEventHandler(itemInHandChangeEventHandler);
		}
	}

	float BasicInHandRenderer::getBlockPlace_percent() {
		long long currentTime = getMilliseconds();
		long long deltaTime = currentTime - blockPlacedStartTime;

		float animationDuration = Player::BLOCK_PLACE_DELAY_MS * 2;

		if(deltaTime < animationDuration / 2) {
			return interpolateLinear(float(deltaTime), 0, 0, animationDuration / 2, 1);
		} else if (deltaTime < animationDuration) {
			return interpolateLinear(deltaTime - (animationDuration / 2), 0, 1, animationDuration / 2, 0);
		} else {
			return 0;
		}
	}

	float BasicInHandRenderer::getItemChange_percent() {
		long long currentTime = getMilliseconds();
		long long deltaTime = currentTime - itemInHandChangedTime;

		float animationDuration = 500;

		if(deltaTime < animationDuration / 2) {
			return interpolateLinear(float(deltaTime), 0, 0, animationDuration / 2, 1);
		} else if (deltaTime < animationDuration) {
			return interpolateLinear(deltaTime - (animationDuration / 2), 0, 1, animationDuration / 2, 0);
		} else {
			return 0;
		}
	}

	glm::mat4x4 BasicInHandRenderer::getModelProjectionMatrix() {
		using namespace glm;

		blockInHandProjectionMatrix.update(10);

		// the center of the cube should be located in the bottom right corner of the screen
		// => (1/-1) in normalized device coordinates
		vec4 clipCoords(0.95f, -1, -1, 1);

		// invert projection matrix and compute eye coords
		// since we don't use a view matrix here eye coords = world coords
		mat4x4 invertedProjectionMatrix = inverse(blockInHandProjectionMatrix.getProjectionMatrix());
		vec4 eyeCoords = invertedProjectionMatrix * clipCoords;

		mat4x4 model(1);
		model = glm::translate(model, vec3(eyeCoords.x, eyeCoords.y, -1));
		model = glm::translate(model, vec3(0, -getItemChange_percent() * 0.05f, 0));
		model = glm::scale(model, vec3(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE));
		model = glm::rotate(model, 0.2f, vec3(1, 0, 0));
		model = glm::rotate(model, PI / 4.0f, vec3(0, 1, 0));
		model = glm::rotate(model, 0.2f, vec3(0, 0, 1));


		vec3 transl(1, -1, 1);
		model = glm::translate(model, transl);
		model = glm::rotate(model, - PI / 2 * getBlockPlace_percent(), vec3(1, 0, 0));
		model = glm::rotate(model, PI / 2 * getBlockPlace_percent(), vec3(0, 0, 1));
		model = glm::translate(model, transl * -1.0f);


		return blockInHandProjectionMatrix.getProjectionMatrix() * model;

	}

}
