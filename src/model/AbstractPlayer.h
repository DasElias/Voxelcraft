#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "GameItem.h"
#include "PlayerPosition.h"
#include <memory>

namespace vc {
	class AbstractPlayer {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			AbstractPlayer() = default;

		public:
			AbstractPlayer(const AbstractPlayer&) = delete;
			virtual ~AbstractPlayer() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual PlayerPosition getPosition() const =0;
			virtual void setPosition(float x, float y, float z) =0;
			virtual glm::ivec3 getChunkCoordinates() const =0;
			virtual bool doesPlayerCollidateWithBlock(const glm::ivec3& pos) const =0;
			virtual bool doesPlayerCollidateWithBlock(int worldX, int worldY, int worldZ) const =0;
			virtual glm::vec3 getRotation() const =0;
			virtual void setRotation(glm::vec3 rotation) =0;
			virtual glm::mat4x4 computeViewMatrix() const =0;
			virtual std::shared_ptr<GameItem> getItemTypeInHand() const =0;
	};
}