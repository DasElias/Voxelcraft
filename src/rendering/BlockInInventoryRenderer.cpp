#include "BlockInInventoryRenderer.h"

#include "../model/utils/MathUtils.h"
#include "..//model/Frustum.h"

#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace vc {
	BlockInInventoryRenderer::BlockInInventoryRenderer() :
			blockInInventoryProjectionMatrix(10, Frustum::NEAR_PLANE_DISTANCE, Frustum::FAR_PLANE_DISTANCE) {
	}
	void BlockInInventoryRenderer::render(const std::shared_ptr<BlockType>& type, TextureArray& blockTextures, float absXMargin, float absYMargin, float width, float height) {
		using namespace glm;
		blockInInventoryProjectionMatrix.update(10);

		float static ROTATION_ARC = PI / 4;

		absXMargin += width / 2;
		absYMargin += height / 2;

		// the center of the cube should be located in the bottom right corner of the screen
		// since we get the coordinates in the range from 0 to 1, but the vector takes them
		// in NDC (-1/1), we have to convert it
		vec4 clipCoords(absXMargin * 2 - 1, (absYMargin * 2 - 1) * -1, -1, 1);

		// invert projection matrix and compute eye coords
		// since we don't use a view matrix here eye coords = world coords
		mat4x4 invertedProjectionMatrix = inverse(blockInInventoryProjectionMatrix.getProjectionMatrix());
		vec4 eyeCoords = invertedProjectionMatrix * clipCoords;

		//float dim = width * (std::cosf(ROTATION_ARC) + std::sinf(ROTATION_ARC));
		vec4 e = invertedProjectionMatrix * vec4(width, width, -1, 1);

	
		glm::mat4x4 model(1);
		
		// translate
		model = glm::translate(model, {eyeCoords.x, eyeCoords.y, -1});

		// TODO rotate by 22,5 degrees
		//model = glm::rotate(model, ROTATION_ARC, {0, 1, 0});
		//model = glm::rotate(model, ROTATION_ARC, {1, 0, 0});

		// scale
		model = glm::scale(model, { e.x, e.x, e.x });

		


		// compute modelProjectionMatrix and render
		mat4x4 modelProjection = blockInInventoryProjectionMatrix.getProjectionMatrix() * model;

		FreePositionableBlockRenderer::render(type, blockTextures, modelProjection);
	}
}
