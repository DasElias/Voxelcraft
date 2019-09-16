#pragma once

#include <glm/vec2.hpp>

namespace vc {
	namespace model {
		class AbstractChunkStack {
			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			protected:
				AbstractChunkStack() = default;

			public:
				AbstractChunkStack(const AbstractChunkStack&) = delete;
				virtual ~AbstractChunkStack() = default;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				virtual bool isDisabled() const =0;
				virtual glm::ivec2 getChunkStackPosition() const =0;
				virtual float computeDistanceToPoint(float x, float z) const =0;
				virtual float computeDistanceToPoint(const glm::vec2& point) const =0;

		};
	}
}
