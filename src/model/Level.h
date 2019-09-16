#pragma once
#include <map>
#include <ctpl_stl.h>
#include <model/events/EventManager.h>

#include "../renderModel/AbstractChunkVaoManager.h"
#include "ChunkIO.h"
#include "LevelMetadata.h"
#include "events/chunkStack/ChunkStackUnloadEvent.h"
#include "events/block/BlockBreakEvent.h"
#include "events/block/BlockPlaceEvent.h"
#include "events/player/ItemInHandChangedEvent.h"
#include "events/player/ToggleSprintingEvent.h"
#include "Face.h"
#include "Player.h"

#include "utils/EigenVectorMapUtils.h"



namespace vc {
	namespace model {
		class Chunk;
		class ChunkStack;
		class Block;
		class ToggleSprintingEvent;

		class Level {
			// ----------------------------------------------------------------------
			// -----------------------------CHILD-CLASS------------------------------
			// ----------------------------------------------------------------------
			public:
				class Level_AccessKey {
					private:
						Level_AccessKey() = default;

					friend class ChunkIO;
					friend class ChunkStack;
				};

			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			private:
				static glm::vec4 const DAY_SKY_COLOR;
				static glm::vec4 const NIGHT_SKY_COLOR;
				static long const AUTOSAVE_INTERVAL;
				static int const RENDER_RANGE;
				static int const DELETE_RANGE;
				static float const DAY_NIGHT_LENGTH;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				float daytime = 0;
				bool day = false;
				std::unique_ptr<LevelMetadata> levelMetadata;
				std::string const levelFolder;
				//ParticleList particles;
				Player player;
				std::map<glm::ivec2, ChunkStack*, vec2i_compareType> allChunkStacks;
				std::vector<Chunk*> visibleChunks;
				ChunkIO chunkLoader;
				vc::renderingModel::AbstractChunkVaoManager& vaoManager;
				std::vector<ChunkStack*> toLoad;
				glm::vec4 backgroundColor = glm::vec4(0);
				float timeSinceAutoSave = 0;
				ctpl::thread_pool& threadPool;
				float timeLastDeleted = 0;

				egui::EventManager<BlockBreakEvent> blockBreakEventManager;
				egui::EventManager<BlockPlaceEvent> blockPlaceEventManager;
				egui::EventManager<ToggleSprintingEvent> toggleSprintingEventManager;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				Level(std::unique_ptr<LevelMetadata> metadata, std::string levelFolder, vc::renderingModel::AbstractChunkVaoManager& vaoManager, ctpl::thread_pool& helperThreadPool);
				Level(const Level&) = delete;
				~Level();


			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			private:
				void loadChunkStacks();
				void updateChunkSystem();
				void updateLoadedChunks();
				void deleteUnusedChunks();

			public:
				void update(float delta);
				void updateOnlyFrustum(float delta);

				void addChunkStackToLoad(Level_AccessKey key, ChunkStack& c);
				void removeFromLoadingList(Level_AccessKey key, ChunkStack& c);
				void putChunkStack(Level_AccessKey key, ChunkStack* p_c);

				ChunkIO& getChunkIO();
				vc::renderingModel::AbstractChunkVaoManager& getVaoManager() const;
				Player& getPlayer();
				glm::vec4 getBackgroundColor() const;
				float getSeed() const;
				std::string getPathToLevelFolder() const;
				std::string getLevelName() const;
				egui::EventManager<BlockBreakEvent>& getBlockBreakEventManager();
				egui::EventManager<BlockPlaceEvent>& getBlockPlaceEventManager();
				egui::EventManager<ToggleSprintingEvent>& getToggleSprintingEventManager();
				float getSecondsUntilAutosave() const;
				ctpl::thread_pool& getThreadPool();

				/*void addParticle(Particle* p);
				ParticleList& getParticleList();*/

				std::vector<Chunk*>& getVisibleChunks();

				tl::optional<Face> getIntersectedBlock(float range, Block*& result_block);
				Block* getBlockAt(const glm::ivec3& worldPosition);
				Block* getBlockAt(int worldX, int worldY, int worldZ);
				bool isBlockAir(const glm::ivec3& worldPosition);
				Block* getBlockUnder(const glm::vec3& worldPosition);
				Block* getBlockUnder(float worldX, float worldY, float worldZ);
				Chunk* getChunkAt(const glm::ivec3& chunkCoordinates);
				ChunkStack* getChunkStackAt(const glm::ivec2& chunkStackCoordinates);
				ChunkStack* getChunkStackWithoutLoading(const glm::ivec2& chunkStackCoordinates) const;

				void putBlockWithoutUpdate(Block* p_block, glm::ivec3 worldPosition, Player* p_triggeringPlayer);
				void putBlockWithUpdate(Block* p_block, glm::ivec3 worldPosition, Player* p_triggeringPlayer);
				void tryToPlaceBlock(glm::ivec3 motherWorldPosition, Face face, Face placedOnFace, const std::shared_ptr<BlockType>& toPlace, Player* p_triggeringPlayer);
				void tryToPlaceBlock(glm::ivec3 worldPosition, Face placedOnFace, const std::shared_ptr<BlockType>& toPlace, Player* p_triggeringPlayer);
		};
	}
}
