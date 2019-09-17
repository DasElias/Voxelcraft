#include "LevelBuilder.h"

#include "LevelIOException.h"
#include "utils/FileUtils.h"

namespace vc {
	namespace {
		const std::string LEVEL_CONFIG_NAME = "level.config";
	}

	Level* loadLevel(std::string levelFolderPath, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool) {
		if(! doesLevelExist(levelFolderPath)) throw LevelIOException("The given folder doesn't contain a valid level.");
		if(! isDirectory(levelFolderPath)) throw LevelIOException("The given path is not a directory.");

		std::string configFile = std::string(levelFolderPath).append("\\").append(LEVEL_CONFIG_NAME.c_str());
		std::unique_ptr<LevelMetadata> metadata(new LevelMetadata(configFile));
		metadata->setLoadedNow();

		Level* p_level = new Level(std::move(metadata), levelFolderPath, vaoCreator, threadPool);
		return p_level;
	}

	Level* createLevel(std::string levelFolderPath, float seed, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool) {
		if(doesLevelExist(levelFolderPath)) throw LevelIOException("The given folder does already contain a valid level.");

		if(! doesExist(levelFolderPath)) createDirectory(levelFolderPath);

		std::string configFile = std::string(levelFolderPath).append("\\").append(LEVEL_CONFIG_NAME.c_str());
		createFile(configFile);
		std::unique_ptr<LevelMetadata> metadata(new LevelMetadata(configFile, "Welt", seed));
		metadata->setLoadedNow();

		Level* p_level = new Level(std::move(metadata), levelFolderPath, vaoCreator, threadPool);
		return p_level;
	}

	bool doesLevelExist(std::string levelFolderPath) {
		return doesExist(levelFolderPath.append("\\").append(LEVEL_CONFIG_NAME.c_str()));
	}

	void deleteLevel(std::string levelFolderPath) {
		if(! doesLevelExist(levelFolderPath)) throw LevelIOException("The given folder doesn't contain a valid level.");
		if(! isDirectory(levelFolderPath)) throw LevelIOException("The given path is not a directory.");

		deleteDirectory(levelFolderPath);
	}
}

