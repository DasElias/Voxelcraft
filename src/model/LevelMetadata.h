#pragma once
#include <string>
#include "SimpleIni.h"


namespace vc {
	namespace model {
		class LevelMetadata {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			private:
				static const char* const LEVEL_NAME_KEY;
				static const char* const SEED_KEY;
				static const char* const LAST_LOADED_KEY;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				CSimpleIniA config;
				std::string fileName;
				std::string levelName;
				float seed;
				long lastLoaded;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				LevelMetadata(std::string configFile);
				LevelMetadata(std::string configFile, std::string levelName, float seed);
				LevelMetadata(const LevelMetadata&) = delete;
				~LevelMetadata();

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				std::string getLevelName() const;
				float getSeed() const;
				long getLastLoaded() const;
				void setLoadedNow();
		};
	}
}
