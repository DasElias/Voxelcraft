#pragma once
#include <mutex>
#include <glm/vec2.hpp>
#include <ctpl_stl.h>
#include <unordered_map>

#include "ChunkSaver.h"
#include "EdenGenerator.h"

#include "utils/EigenVectorMapUtils.h"


namespace vc {
	class Level;
	class ChunkSaver;
	class ChunkStack;
	class ChunkIO {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			Level& level;
			EdenGenerator chunkStackGenerator;
			ChunkSaver chunkSaver;
			ctpl::thread_pool& threadPool;

			mutable std::mutex loadedChunks_mutex;
			std::vector<ChunkStack*> loadedChunks;

			mutable std::mutex toLoad_mutex;
			std::unordered_map<glm::ivec2, std::future<ChunkStack*>, vec2i_hashType, vec2i_equalsType> toLoad;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ChunkIO(Level& level, EdenGenerator chunkStackGenerator, ChunkSaver chunkSaver, ctpl::thread_pool& threadPool);
			ChunkIO(const ChunkIO&) = delete;


		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ChunkStack* createChunkStack(glm::ivec2 coords);
			ChunkStack* loadFromFile(std::string pathToFile);
			ChunkStack* generateChunkStackImmediately(glm::ivec2 coords);

		public:
			void load(glm::ivec2 coords);
			ChunkStack* loadImmediately(glm::ivec2 coords);
			void saveChunkStack(ChunkStack& chunkStack);
			bool hasNewChunkStacksLoaded() const;
			std::size_t getAmountOfFinishedChunkStacks() const;
			void loadChunkStacksInOpenglThread();

	};
}
