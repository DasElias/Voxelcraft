#pragma once
#include <string>
#include "SimpleIni.h"


namespace vc {
	class LevelMetadata {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static const char* const LEVEL_NAME_KEY;
			static const char* const SEED_KEY;
			static const char* const SEED_TEXT_KEY;
			static const char* const LAST_LOADED_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			CSimpleIniA config;
			std::string levelFolderPath;
			std::string configFilePath;

			std::string levelName;
			float seed;
			std::string seedText;
			long lastLoaded;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LevelMetadata(std::string levelFolderPath, std::string configFileName);
			LevelMetadata(std::string levelFolderPath, std::string configFileName, std::string levelName, std::string seed);
			LevelMetadata(const LevelMetadata&) = delete;
			~LevelMetadata();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			float hashSeed(std::string) const;

		public:
			std::string getLevelName() const;
			float getSeed() const;
			std::string getSeedText() const;
			long getLastLoaded() const;
			void setLoadedNow();

			std::string getLevelFolderPath() const;
	};
}
