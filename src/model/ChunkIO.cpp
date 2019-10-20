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

#include <boost/log/trivial.hpp>


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

	ChunkStack* vc::ChunkIO::generateChunkStackImmediately(glm::ivec2 coords) {
		ChunkStack* p_c = nullptr;
		if(toLoad.find(coords) == toLoad.end()) {
			//case 1: load() wasn't called for coords yet
			p_c = createChunkStack(coords);

		} else {
			std::future<ChunkStack*>* task = &toLoad.at(coords);
			p_c = task->get();

			// erase loaded chunk from loadedChunks and toLoad
			std::lock_guard<std::mutex> lock2(loadedChunks_mutex);
			loadedChunks.erase(std::find(loadedChunks.begin(), loadedChunks.end(), p_c));
			toLoad.erase(p_c->getChunkStackPosition());
		}

		return p_c;
	}

	ChunkStack* ChunkIO::loadImmediately(glm::ivec2 coords) {
		std::lock_guard<std::mutex> lock1(toLoad_mutex);	
		BOOST_LOG_TRIVIAL(info) << "Chunk Stack is loaded immediately: (" << coords.x << ", " << coords.y << ")";

		// generate chunkStack
		ChunkStack* p_c = generateChunkStackImmediately(coords);
		level.putChunkStack({}, p_c);

		// load neighbours
		ChunkStack* p_leftNeighbor = level.getChunkStackWithoutLoading({coords.x - 1, coords.y});
		ChunkStack* p_rightNeighbor = level.getChunkStackWithoutLoading({coords.x + 1, coords.y});
		ChunkStack* p_frontNeighbor = level.getChunkStackWithoutLoading({coords.x, coords.y - 1});
		ChunkStack* p_backNeighbor = level.getChunkStackWithoutLoading({coords.x, coords.y + 1});

		if(p_leftNeighbor == nullptr) {
			p_leftNeighbor = generateChunkStackImmediately({coords.x - 1, coords.y});
			level.putChunkStack({}, p_leftNeighbor);
		}
		if(p_rightNeighbor == nullptr) {
			p_rightNeighbor = generateChunkStackImmediately({coords.x + 1, coords.y});
			level.putChunkStack({}, p_rightNeighbor);
		}
		if(p_frontNeighbor == nullptr) {
			p_frontNeighbor = generateChunkStackImmediately({coords.x, coords.y - 1});
			level.putChunkStack({}, p_frontNeighbor);
		}
		if(p_backNeighbor == nullptr) {
			p_backNeighbor = generateChunkStackImmediately({coords.x, coords.y + 1});
			level.putChunkStack({}, p_backNeighbor);
		}

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
