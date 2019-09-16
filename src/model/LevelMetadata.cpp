#include "LevelMetadata.h"

#include "utils/TimeUtils.h"
#include <stdexcept>

namespace vc::model {
	const char* const LevelMetadata::LEVEL_NAME_KEY = "levelName";
	const char* const LevelMetadata::SEED_KEY = "seed";
	const char* const LevelMetadata::LAST_LOADED_KEY = "lastLoaded";

	LevelMetadata::LevelMetadata(std::string levelFile) :
			fileName(levelFile) {

		SI_Error err = config.LoadFile(levelFile.c_str());
		if(err < 0) throw std::runtime_error("Couldn't load level metadata file.");

		levelName = config.GetValue("voxelcraft", LEVEL_NAME_KEY, "Neue Welt");
		config.SetValue("voxelcraft", LEVEL_NAME_KEY, levelName.c_str());

		seed = float(config.GetDoubleValue("voxelcraft", SEED_KEY, 17));
		config.SetDoubleValue("voxelcraft", SEED_KEY, seed);

		lastLoaded = config.GetLongValue("voxelcraft", LAST_LOADED_KEY, long(getMilliseconds() / 1000));
		config.SetLongValue("voxelcraft", LAST_LOADED_KEY, lastLoaded);

		config.SaveFile(levelFile.c_str());
	}

	LevelMetadata::LevelMetadata(std::string configFile, std::string levelName, float seed) :
			fileName(configFile),
			levelName(levelName),
			seed(seed),
			lastLoaded(long(getMilliseconds() / 1000)) {


		config.SetValue("voxelcraft", LEVEL_NAME_KEY, levelName.c_str());
		config.SetDoubleValue("voxelcraft", SEED_KEY, seed);
		config.SetLongValue("voxelcraft", LAST_LOADED_KEY, lastLoaded);

		config.SaveFile(configFile.c_str());

	}

	LevelMetadata::~LevelMetadata() {
	}

	std::string LevelMetadata::getLevelName() const {
		return levelName;
	}

	float LevelMetadata::getSeed() const {
		return seed;
	}

	long LevelMetadata::getLastLoaded() const {
		return lastLoaded;
	}



	void LevelMetadata::setLoadedNow() {
		long s = long(getMilliseconds() / 1000);
		config.SetLongValue("voxelcraft", LAST_LOADED_KEY, s);

		config.SaveFile(fileName.c_str());
	}

}
