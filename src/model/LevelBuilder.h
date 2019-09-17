#pragma once
#include <string>
#include <ctpl_stl.h>

#include "../renderModel/AbstractChunkVaoManager.h"
#include "Level.h"


namespace vc {
	Level* loadLevel(std::string levelFolderPath, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool);
	Level* createLevel(std::string levelFolderPath, float seed, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool);
	bool doesLevelExist(std::string levelFolderPath);
	void deleteLevel(std::string levelFolderPath);
}
