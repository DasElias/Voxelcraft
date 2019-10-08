#include "LevelBuilder.h"

#include "LevelIOException.h"
#include "utils/FileUtils.h"

namespace vc {
	std::string const LevelBuilder::LEVEL_CONFIG_FILENAME = "level.config";

	Level* LevelBuilder::loadLevel(std::shared_ptr<LevelMetadata> metadata, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool) {
		metadata->setLoadedNow();

		Level* p_level = new Level(metadata, vaoCreator, threadPool);
		return p_level;
	}

	Level* LevelBuilder::createLevel(std::string levelFolderPath, float seed, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool) {
		if(doesLevelExist(levelFolderPath)) throw LevelIOException("The given folder does already contain a valid level.");

		if(! doesExist(levelFolderPath)) createDirectory(levelFolderPath);

		// create file
		std::string configFile = std::string(levelFolderPath).append("\\").append(LEVEL_CONFIG_FILENAME.c_str());
		createFile(configFile);

		// create LevelMetadata
		std::shared_ptr<LevelMetadata> metadata(new LevelMetadata(levelFolderPath, LevelBuilder::LEVEL_CONFIG_FILENAME, "Welt", seed));
		metadata->setLoadedNow();

		Level* p_level = new Level(metadata, vaoCreator, threadPool);
		return p_level;
	}

	bool LevelBuilder::doesLevelExist(std::string levelFolderPath) {
		return doesExist(levelFolderPath.append("\\").append(LEVEL_CONFIG_FILENAME.c_str()));
	}

	void LevelBuilder::deleteLevel(std::string levelFolderPath) {
		if(! doesLevelExist(levelFolderPath)) throw LevelIOException("The given folder doesn't contain a valid level.");
		if(! isDirectory(levelFolderPath)) throw LevelIOException("The given path is not a directory.");

		deleteDirectory(levelFolderPath);
	}
}

