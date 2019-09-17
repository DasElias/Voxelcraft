#pragma once

#include <glm/vec3.hpp>
#include "../renderModel/ChunkVertexArrayObject.h"

namespace vc {
	class AbstractChunk {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			AbstractChunk() = default;

		public:
			AbstractChunk(const AbstractChunk&) = delete;
			virtual ~AbstractChunk() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual bool wasLastTimeVisible() const =0;
			virtual void setLastTimeVisible(bool flag) =0;

			virtual float getSquaredDistanceToPlayer() const =0;
			virtual void updateDistanceToPlayer() =0;

			virtual int getMaxHeight() const =0;
			virtual ChunkVertexArrayObject* const getVao() const =0;
			virtual int getAmountOfPlacedBlocks() const =0;

			virtual void updateVao() =0;

			virtual bool isInFrustum() const =0;
			virtual float computeDistanceToPointSquared(float x, float y, float z) const =0;
			virtual float computeDistanceToPointSquared(const glm::vec3& point) const =0;
			virtual float computeDistanceToPoint(float x, float y, float z) const =0;
			virtual float computeDistanceToPoint(const glm::vec3& point) const =0;
			virtual glm::ivec3 getChunkCoordinates() const =0;
	};
}
