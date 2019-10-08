#include "LevelMetadata.h"

#include "utils/TimeUtils.h"
#include <stdexcept>

namespace vc {
	const char* const LevelMetadata::LEVEL_NAME_KEY = "levelName";
	const char* const LevelMetadata::SEED_KEY = "seed";
	const char* const LevelMetadata::LAST_LOADED_KEY = "lastLoaded";

	LevelMetadata::LevelMetadata(std::string levelFolderPath, std::string configFileName) :
			levelFolderPath(levelFolderPath),
			configFilePath(levelFolderPath + "\\" + configFileName) {

		SI_Error err = config.LoadFile(configFilePath.c_str());
		if(err < 0) throw std::runtime_error("Couldn't load level metadata file.");

		levelName = config.GetValue("voxelcraft", LEVEL_NAME_KEY, "Neue Welt");
		config.SetValue("voxelcraft", LEVEL_NAME_KEY, levelName.c_str());

		seed = float(config.GetDoubleValue("voxelcraft", SEED_KEY, 17));
		config.SetDoubleValue("voxelcraft", SEED_KEY, seed);

		lastLoaded = config.GetLongValue("voxelcraft", LAST_LOADED_KEY, long(getMilliseconds() / 1000));
		config.SetLongValue("voxelcraft", LAST_LOADED_KEY, lastLoaded);

		config.SaveFile(configFilePath.c_str());
	}

	LevelMetadata::LevelMetadata(std::string levelFolderPath, std::string configFileName, std::string levelName, float seed) :
			levelFolderPath(levelFolderPath),
			configFilePath(levelFolderPath + "\\" + configFileName),
			levelName(levelName),
			seed(seed),
			lastLoaded(long(getMilliseconds() / 1000)) {


		config.SetValue("voxelcraft", LEVEL_NAME_KEY, levelName.c_str());
		config.SetDoubleValue("voxelcraft", SEED_KEY, seed);
		config.SetLongValue("voxelcraft", LAST_LOADED_KEY, lastLoaded);

		config.SaveFile(configFilePath.c_str());

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

		config.SaveFile(configFilePath.c_str());
	}

	std::string LevelMetadata::getLevelFolderPath() const {
		return levelFolderPath;
	}

}
