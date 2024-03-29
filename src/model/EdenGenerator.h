#pragma once

#include "ChunkStackGenerator.h"
#include <glm/vec2.hpp>

namespace vc {
	class EdenGenerator : public ChunkStackGenerator {
		public:
			ChunkStack* generateChunkStack(Level& level, glm::ivec2 chunkStackCoordinates) override;
	};
}
