#include "ChunkIO.h"

#include <array>
#include <future>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Chunk.h"
#include "ChunkStack.h"
#include "Level.h"
#include "utils/FileUtils.h"
#include <gzstream.h>

namespace vc {
	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	ChunkIO::ChunkIO(Level& level, EdenGenerator chunkStackGenerator, ChunkSaver chunkSaver, ctpl::thread_pool& threadPool) :
			level(level),
			chunkStackGenerator(chunkStackGenerator),
			chunkSaver(chunkSaver),
			loadedChunks_mutex(),
			threadPool(threadPool),
			toLoad_mutex(),
			toLoad(0, VECTOR2I_HASH, VECTOR2I_EQUAL) {
	}

	// ----------------------------------------------------------------------
	// -------------------------------METHODS--------------------------------
	// ----------------------------------------------------------------------
	ChunkStack* ChunkIO::createChunkStack(glm::ivec2 coords) {
		std::string p = level.getPathToLevelFolder();
		std::stringstream fileNameBuilder;
		fileNameBuilder << p << "\\chunkStack_" << coords.x << "_" << coords.y;
		std::string fileName = fileNameBuilder.str();

		return (doesExist(fileName) ? loadFromFile(fileName) : chunkStackGenerator.generateChunkStack(level, coords));
	}

	ChunkStack* ChunkIO::loadFromFile(std::string pathToFile) {
		return chunkSaver.readChunkStack(pathToFile, level);
	}

	void ChunkIO::load(glm::ivec2 coords) {
		std::lock_guard<std::mutex> lock1(toLoad_mutex);

		if(toLoad.find(coords) == toLoad.end()) {
			// key not present
			toLoad.insert(std::make_pair(coords, threadPool.push([this, coords](int) -> ChunkStack* {
				ChunkStack* generated = createChunkStack(coords);

				std::lock_guard<std::mutex> lock2(loadedChunks_mutex);
				loadedChunks.push_back(generated);
				return generated;
			})));
		}
	}

	ChunkStack* ChunkIO::loadImmediately(glm::ivec2 coords) {
		std::lock_guard<std::mutex> lock1(toLoad_mutex);

		ChunkStack* p_c = nullptr;

		if(toLoad.find(coords) == toLoad.end()) {
			//case 1: load() wasn't called for coords
			p_c = createChunkStack(coords);

		} else {
			std::future<ChunkStack*>* task = nullptr;

			/*
			 * We can't put all the code in one lock_guard-block, since task->get will need loadedChunks_mutex too,
			 * what is going to cause a deadlock.
			 */
			{
				std::lock_guard<std::mutex> lock2(loadedChunks_mutex);
				task = &toLoad.at(coords);
			}

			p_c = task->get();

			{
				std::lock_guard<std::mutex> lock2(loadedChunks_mutex);
				loadedChunks.erase(loadedChunks.begin());
			}

			toLoad.erase(p_c->getChunkStackPosition());
		}

		ChunkStack* p_leftNeighbor = level.getChunkStackWithoutLoading({coords.x - 1, coords.y});
		ChunkStack* p_rightNeighbor = level.getChunkStackWithoutLoading({coords.x + 1, coords.y});
		ChunkStack* p_frontNeighbor = level.getChunkStackWithoutLoading({coords.x, coords.y - 1});
		ChunkStack* p_backNeighbor = level.getChunkStackWithoutLoading({coords.x, coords.y + 1});

		if(p_leftNeighbor == nullptr) {
			p_leftNeighbor = createChunkStack({coords.x - 1, coords.y});
			level.putChunkStack({}, p_leftNeighbor);
		}
		if(p_rightNeighbor == nullptr) {
			p_rightNeighbor = createChunkStack({coords.x + 1, coords.y});
			level.putChunkStack({}, p_rightNeighbor);
		}
		if(p_frontNeighbor == nullptr) {
			p_frontNeighbor = createChunkStack({coords.x, coords.y - 1});
			level.putChunkStack({}, p_frontNeighbor);
		}
		if(p_backNeighbor == nullptr) {
			p_backNeighbor = createChunkStack({coords.x, coords.y + 1});
			level.putChunkStack({}, p_backNeighbor);
		}

		level.putChunkStack({}, p_c);
		level.removeFromLoadingList({}, *p_c);

		for(Chunk* singleChunk : p_c->getChunks()) {
			if(singleChunk->getAmountOfPlacedBlocks() > 0) {
				singleChunk->updateVao();
			}
		}

		p_c->setFirstInitialized({});

		return p_c;

	}

	void ChunkIO::saveChunkStack(ChunkStack& chunkStack) {
		glm::ivec2 coords = chunkStack.getChunkStackPosition();
		std::string p = level.getPathToLevelFolder();
		std::stringstream fileNameBuilder;
		fileNameBuilder << p << "\\chunkStack_" << coords.x << "_" << coords.y;
		std::string fileName = fileNameBuilder.str();


		chunkSaver.saveChunkStack(chunkStack, fileName);
	}

	bool ChunkIO::hasNewChunkStacksLoaded() const {
		std::lock_guard<std::mutex> l(loadedChunks_mutex);
		return !loadedChunks.empty();
	}

	std::size_t ChunkIO::getAmountOfFinishedChunkStacks() const {
		std::lock_guard<std::mutex> lock(loadedChunks_mutex);
		return loadedChunks.size();
	}



	void ChunkIO::loadChunkStacksInOpenglThread() {
		std::lock_guard<std::mutex> lock1(loadedChunks_mutex);
		std::lock_guard<std::mutex> lock2(toLoad_mutex);

		while(! loadedChunks.empty()) {
			ChunkStack* p_cStack = loadedChunks.front();
			loadedChunks.erase(loadedChunks.begin());

			toLoad.erase(p_cStack->getChunkStackPosition());
			level.putChunkStack({}, p_cStack);
		}
	}

}
