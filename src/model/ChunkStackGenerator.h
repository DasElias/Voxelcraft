#pragma once

#include <glm/vec2.hpp>

namespace vc {
	namespace model {
		class ChunkStack;
		class Level;

		class ChunkStackGenerator {
			public:
				ChunkStackGenerator() = default;
				virtual ~ChunkStackGenerator() = default;

				virtual ChunkStack* generateChunkStack(Level& level, glm::ivec2 chunkStackCoordinates) =0;
		};
	}
}
