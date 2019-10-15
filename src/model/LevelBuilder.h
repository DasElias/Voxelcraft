#pragma once
#include <string>
#include <ctpl_stl.h>

#include "Level.h"


namespace vc {
	class LevelBuilder {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static std::string const LEVEL_CONFIG_FILENAME;

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static Level* loadLevel(std::shared_ptr<LevelMetadata> metadata, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool);
			static Level* createLevel(std::string levelFolderPath, std::string levelName, std::string seed, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool);
			static bool doesLevelExist(std::string levelFolderPath);
			static void deleteLevel(std::string levelFolderPath);
	};
}
