#pragma once

#include <string>
#include <vector>

namespace vc {
	namespace model {
		class ChunkStack;
		class Level;

		class AbstractChunkSaver {
			// ----------------------------------------------------------------------
			// ------------------------FORWARD-DECLARATIONS--------------------------
			// ----------------------------------------------------------------------

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				virtual ~AbstractChunkSaver() = default;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				virtual void saveChunkStack(ChunkStack& c, std::string fileName) =0;
				virtual ChunkStack* readChunkStack(std::string fileName, Level& lvl) =0;
		};
	}
}
