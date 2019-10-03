#include "Block.h"

#include <tl/optional.hpp>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

#include "Frustum.h"
#include "Level.h"
#include "MousePicker.h"
#include "Player.h"
#include "utils/MathUtils.h"
#include "utils/VectorUtils.h"


#include <glm/glm.hpp>

namespace vc {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	Block::Block(uint8_t inChunkX, uint8_t inChunkY, uint8_t inChunkZ, const BlockType* blockType, Chunk& c, const TextureOrientation& texOrientation, int8_t metadata) :
		AbstractBlock(inChunkX, inChunkY, inChunkZ),
		blockType(blockType),
		texOrientation(texOrientation),
		chunk(c),
		metadata(metadata) {
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	float Block::checkIntersectionForPlane(const glm::vec3& position, const glm::vec3& spanVector1, const glm::vec3& spanVector2, const glm::vec3& rayStartVector, const glm::vec3& rayDirectionVector) const {
		using namespace glm;
		static float const TOLERANCE = 0.0001f;

		vec3 widthHeightDepth = spanVector1 + spanVector2;
		vec3 normal = normalize(cross(spanVector1, spanVector2));

		float lambdaNumerator = - dot(normal, vec3(-position.x, -position.y, -position.z)) - dot(normal, rayStartVector);
		float lambdaDenominator = dot(normal, rayDirectionVector);
		if(lambdaDenominator != 0) {
			float lambda = lambdaNumerator / lambdaDenominator;
			vec3 intersectionPoint = rayStartVector + (rayDirectionVector * lambda);

			if(intersectionPoint.x >= position.x - TOLERANCE && intersectionPoint.x - TOLERANCE <= position.x + widthHeightDepth.x &&
								intersectionPoint.y >= position.y - TOLERANCE && intersectionPoint.y - TOLERANCE <= position.y + widthHeightDepth.y &&
								intersectionPoint.z >= position.z - TOLERANCE && intersectionPoint.z - TOLERANCE <= position.z + widthHeightDepth.z) {
				vec3 distanceVector = intersectionPoint - rayStartVector;
				float distance = length(distanceVector);

				if(dot(normal, rayDirectionVector) < 0) {
					return distance;
				}
			}
		} else {

		}

		return std::numeric_limits<float>::infinity();
	}

	void Block::setMetadata(int8_t metadata) {
		this->metadata = metadata;
	}

	int8_t Block::getMetadata() const {
		return metadata;
	}

	const BlockType& Block::getBlockType() const {
		return *blockType;
	}

	float Block::getWidth() const {
		return 1;
	}

	float Block::getHeight() const {
		return 1;
	}

	float Block::getDepth() const {
		return 1;
	}

	bool Block::isSolid() const {
		return true;
	}

	const TextureOrientation& Block::getTexOrientation() const {
		return texOrientation;
	}

	tl::optional<std::pair<float, Face>> Block::checkIntersectionAndGetFace() const {
		using namespace glm;

		vec3 startVector = chunk.getLevel().getPlayer().getMousePicker().getRayStart();
		vec3 directionVector = chunk.getLevel().getPlayer().getMousePicker().getRay();

		std::vector<std::pair<float, Face>> intersectionDistances;
		vec3 supportVector;
		vec3 spanVector1;
		vec3 spanVector2;

		float worldX = float(getWorldX());
		float worldY = float(getWorldY());
		float worldZ = float(getWorldZ());



		// front
		set(supportVector,	worldX, worldY, worldZ);
		set(spanVector1, 		getWidth(), 0, 0);
		set(spanVector2,  		0, getHeight(), 0);
		intersectionDistances.push_back(
			std::make_pair(
				checkIntersectionForPlane(supportVector, spanVector2, spanVector1, startVector, directionVector),
				FRONT
			)
		);

		// back
		set(supportVector, 	worldX, worldY, worldZ + getDepth());
		set(spanVector1, 		getWidth(), 0, 0);
		set(spanVector2,  		0, getHeight(), 0);
		intersectionDistances.push_back(
			std::make_pair(
				checkIntersectionForPlane(supportVector, spanVector1, spanVector2, startVector, directionVector),
				BACK
			)
		);

		// right
		set(supportVector, 	worldX, worldY, worldZ);
		set(spanVector1,  		0, 0, getDepth());
		set(spanVector2,  		0, getHeight(), 0);
		intersectionDistances.push_back(
			std::make_pair(
				checkIntersectionForPlane(supportVector, spanVector1, spanVector2, startVector, directionVector),
				RIGHT
			)
		);

		// left
		set(supportVector, 	worldX + getWidth(), worldY, worldZ);
		set(spanVector1,  		0, 0, getDepth());
		set(spanVector2,  		0, getHeight(), 0);
		intersectionDistances.push_back(
			std::make_pair(
				checkIntersectionForPlane(supportVector, spanVector2, spanVector1, startVector, directionVector),
				LEFT
			)
		);

		// bottom
		set(supportVector, 	worldX, worldY, worldZ);
		set(spanVector1,  		getWidth(), 0, 0);
		set(spanVector2,  		0, 0, getDepth());
		intersectionDistances.push_back(
			std::make_pair(
				checkIntersectionForPlane(supportVector, spanVector1, spanVector2, startVector, directionVector),
				BOTTOM
			)
		);

		// top
		set(supportVector, 	worldX, worldY + getHeight(), worldZ);
		set(spanVector1,  		getWidth(), 0, 0);
		set(spanVector2,  		0, 0, getDepth());
		intersectionDistances.push_back(
			std::make_pair(
				checkIntersectionForPlane(supportVector, spanVector2, spanVector1, startVector, directionVector),
				TOP
			)
		);

		// sort list of intersection distances
		std::sort(intersectionDistances.begin(), intersectionDistances.end(),
			[](const std::pair<float, Face>& o1, const std::pair<float, Face>& o2) -> bool {
				// this lambda returns true if the first argument should before the second one
				// we want to sort the list in ascending order
				// => return true if a < b

				return o1.first < o2.first;
			}
		);

		if(intersectionDistances[0].first != std::numeric_limits<float>::infinity()) {
			return intersectionDistances[0];
		} else {
			return tl::nullopt;
		}
	}

	bool Block::isInFrustum() const {
		glm::ivec3 chunkPosition = chunk.getChunkCoordinates();

		float worldX = float(convertChunkToWorldValue(chunkPosition.x, inChunkX));
		float worldY = float(convertChunkToWorldValue(chunkPosition.y, inChunkY));
		float worldZ = float(convertChunkToWorldValue(chunkPosition.z, inChunkZ));
		return chunk.getLevel().getPlayer().getFrustum().isInFrustum_aabb(worldX, worldY, worldZ, getWidth(), getHeight(), getDepth());
	}

	int Block::getWorldX() const {
		return convertChunkToWorldValue(chunk.getChunkCoordinates().x, inChunkX);
	}

	int Block::getWorldY() const {
		return convertChunkToWorldValue(chunk.getChunkCoordinates().y, inChunkY);
	}

	int Block::getWorldZ() const {
		return convertChunkToWorldValue(chunk.getChunkCoordinates().z, inChunkZ);
	}

	void Block::onDestroy(BlockBreakEvent& evt) {
	}

	void Block::onFocusUpdate() {
		// empty method body
	}

	void Block::onUpdate() {
		// empty method body
	}

}
