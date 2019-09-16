#pragma once
#include <string>
#include <ctpl_stl.h>

#include "../renderModel/AbstractChunkVaoManager.h"
#include "Level.h"


namespace vc {
	namespace model {
		Level* loadLevel(std::string levelFolderPath, vc::renderingModel::AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool);
		Level* createLevel(std::string levelFolderPath, float seed, vc::renderingModel::AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool);
		bool doesLevelExist(std::string levelFolderPath);
		void deleteLevel(std::string levelFolderPath);

		/**
		class LevelBuilder {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			public:
				static std::string const LEVEL_CONFIG_FILE_NAME = "level.config";
				static std::string const PLAYER_FILE_NAME = "player.config";

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			private:
				LevelBuilder();

			// ----------------------------------------------------------------------
			// ---------------------------STATIC-METHODS-----------------------------
			// ----------------------------------------------------------------------
			public:

		};*/
	}
}
