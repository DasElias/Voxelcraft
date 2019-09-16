#pragma once

#include "AbstractChunkSaver.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace vc {
	namespace model {

		class ChunkSaver : public AbstractChunkSaver {
			// ----------------------------------------------------------------------
			// -----------------------------CHILD-CLASS------------------------------
			// ----------------------------------------------------------------------
			private:
				struct LexerResult {
					std::unordered_map<std::string, std::string> stringProperties;
					std::unordered_map<std::string, std::vector<int>> arrayProperties;
				};
			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			private:
				void lexer(std::string data, LexerResult& result_lexerResult);
				ChunkStack* parser(LexerResult& lexedGroup, Level& lvl);

			public:
				void saveChunkStack(ChunkStack& c, std::string fileName) override;
				ChunkStack* readChunkStack(std::string fileName, Level& lvl) override;

			
		};
	}
}
