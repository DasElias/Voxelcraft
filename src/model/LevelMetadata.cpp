#include "LevelMetadata.h"

#include "utils/TimeUtils.h"
#include <stdexcept>

namespace vc {
	const char* const LevelMetadata::LEVEL_NAME_KEY = "levelName";
	const char* const LevelMetadata::SEED_KEY = "seed";
	const char* const LevelMetadata::SEED_TEXT_KEY = "seedText";
	const char* const LevelMetadata::LAST_LOADED_KEY = "lastLoaded";

	LevelMetadata::LevelMetadata(std::string levelFolderPath, std::string configFileName) :
			levelFolderPath(levelFolderPath),
			configFilePath(levelFolderPath + "\\" + configFileName) {

		SI_Error err = config.LoadFile(configFilePath.c_str());
		if(err < 0) throw std::runtime_error("Couldn't load level metadata file.");

		levelName = config.GetValue("voxelcraft", LEVEL_NAME_KEY, "Neue Welt");
		config.SetValue("voxelcraft", LEVEL_NAME_KEY, levelName.c_str());

		std::string const defaultSeed = "17.3";
		seed = float(config.GetDoubleValue("voxelcraft", SEED_KEY, hashSeed(defaultSeed)));
		config.SetDoubleValue("voxelcraft", SEED_KEY, seed);

		seedText = config.GetValue("voxelcraft", SEED_TEXT_KEY, defaultSeed.c_str());
		config.SetValue("voxelcraft", SEED_TEXT_KEY, seedText.c_str());

		lastLoaded = config.GetLongValue("voxelcraft", LAST_LOADED_KEY, long(getMilliseconds() / 1000));
		config.SetLongValue("voxelcraft", LAST_LOADED_KEY, lastLoaded);

		config.SaveFile(configFilePath.c_str());
	}

	LevelMetadata::LevelMetadata(std::string levelFolderPath, std::string configFileName, std::string levelName, std::string seed) :
			levelFolderPath(levelFolderPath),
			configFilePath(levelFolderPath + "\\" + configFileName),
			levelName(levelName),
			seedText(seed),
			lastLoaded(long(getMilliseconds() / 1000)) {

		this->seed = hashSeed(seed);

		config.SetValue("voxelcraft", LEVEL_NAME_KEY, levelName.c_str());
		config.SetDoubleValue("voxelcraft", SEED_KEY, this->seed);
		config.SetValue("voxelcraft", SEED_TEXT_KEY, seed.c_str());
		config.SetLongValue("voxelcraft", LAST_LOADED_KEY, lastLoaded);

		config.SaveFile(configFilePath.c_str());

	}

	LevelMetadata::~LevelMetadata() {
	}

	float LevelMetadata::hashSeed(std::string seedString) const {
		// compute float value from seed string
		static std::hash<std::string> hasher;
		size_t seedHash = hasher(seedString);
		
		double seed = seedHash / 10000.0f;
		while(seed > 100000) {
			seed /= 10000.0f;
		}

		return seed;
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
