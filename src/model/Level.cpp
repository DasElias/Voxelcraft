#include "Level.h"

#include <array>
#include <string>
#include <utility>
#include <memory>
#include <vector>

#include "BlockCreator.h"
#include "BlockType.h"
#include "Chunk.h"
#include "ChunkStack.h"
#include "utils/MathUtils.h"
#include "utils/VectorUtils.h"

namespace vc {
	const glm::vec4 Level::DAY_SKY_COLOR = {0.494f, 0.671f, 1, 1};
	const glm::vec4 Level::NIGHT_SKY_COLOR = {0, 0, 0, 1};
	const long Level::AUTOSAVE_INTERVAL = 2 * 60;
	const int Level::RENDER_RANGE = 5;
	const int Level::DELETE_RANGE = RENDER_RANGE + 1;
	const float Level::DAY_NIGHT_LENGTH = 1 * 60;

	Level::Level(std::unique_ptr<LevelMetadata> metadata, std::string levelFolder, AbstractChunkVaoManager& vaoManager, ctpl::thread_pool& helperThreadPool) :
			levelMetadata(std::move(metadata)),
			levelFolder(levelFolder),
			allChunkStacks(23, VECTOR2I_HASH, VECTOR2I_EQUAL),
			player({0, 100, 0}, *this),
			chunkLoader(*this, {}, {}, helperThreadPool),
			vaoManager(vaoManager),
			threadPool(helperThreadPool) {
	}

	Level::~Level() {
		for(auto& entry : allChunkStacks) {
			delete entry.second;
		}
	}

	void Level::loadChunkStacks() {
		do {
			if(! toLoad.empty()) {
				ChunkStack* p_c = toLoad.front();
				toLoad.erase(toLoad.begin());

				for(auto& singleChunk : p_c->getChunks()) {
					singleChunk->updateVao();
				}
				p_c->setFirstInitialized({});
			}
		} while(toLoad.size() > 200);
	}

	void Level::updateChunkSystem() {
		visibleChunks.clear();

		for(auto& it : allChunkStacks) {
			ChunkStack* p_c = it.second;

			if(p_c->isDisabled()) continue;

			for(auto& singleChunk : p_c->getChunks()) {
				if(singleChunk->getAmountOfPlacedBlocks() > 0 && singleChunk->isInFrustum()) {
					singleChunk->updateDistanceToPlayer();

					// Insertion Sort
					bool shouldContinue = false;
					for(unsigned int c = 0; c < visibleChunks.size(); c++) {
						if(! shouldContinue && visibleChunks[c]->getSquaredDistanceToPlayer() >= singleChunk->getSquaredDistanceToPlayer()) {
							visibleChunks.insert(visibleChunks.begin() + c, singleChunk);
							shouldContinue = true;
						}
					}

					// if the chunk hasn't been inserted before
					if(! shouldContinue) {
						visibleChunks.push_back(singleChunk);
					}
				}
			}
		}
	}

	void Level::updateLoadedChunks() {
		glm::ivec3 playerChunk = player.getChunkCoordinates();

		for(int x = playerChunk.x - RENDER_RANGE; x < playerChunk.x + RENDER_RANGE; x++) {
			for(int z = playerChunk.z - RENDER_RANGE; z < playerChunk.z + RENDER_RANGE; z++) {
				glm::ivec2 chunkStackPosition(x, z);
				if(allChunkStacks.find(chunkStackPosition) == allChunkStacks.end()) {
					// chunk stack doesn't exist yet
					chunkLoader.load(chunkStackPosition);
				}
			}
		}
	}

	void Level::deleteUnusedChunks() {
		const int deleteRange = DELETE_RANGE * Chunk::CHUNK_SIZE;

		auto it = allChunkStacks.begin();
		while(it != allChunkStacks.end()) {
			if(it->second->computeDistanceToPoint(player.getPosition().x, player.getPosition().z) > deleteRange) {
				delete it->second;

				it = allChunkStacks.erase(it);
			} else {
				++it;
			}
		}
	}

	void Level::update(float delta) {
		// update daytime
		if(day) daytime += delta;
		else daytime -= delta;

		if(daytime < 0) day = true;
		else if (daytime > DAY_NIGHT_LENGTH) day = false;

		// compute sky color
		set(backgroundColor,
				float(interpolateLinear(daytime, 0, DAY_SKY_COLOR.x, DAY_NIGHT_LENGTH, NIGHT_SKY_COLOR.x)),
				float(interpolateLinear(daytime, 0, DAY_SKY_COLOR.y, DAY_NIGHT_LENGTH, NIGHT_SKY_COLOR.y)),
				float(interpolateLinear(daytime, 0, DAY_SKY_COLOR.z, DAY_NIGHT_LENGTH, NIGHT_SKY_COLOR.z)),
				1);

		// update player
		player.computeMove(delta);

		timeLastDeleted += delta;
		if(timeLastDeleted > 60) {
			timeLastDeleted = 0;

			deleteUnusedChunks();
		}

		// autosave
		timeSinceAutoSave += delta;
		if(timeSinceAutoSave > AUTOSAVE_INTERVAL) {
			timeSinceAutoSave = 0;

			for(auto& it : allChunkStacks) {
				it.second->possiblySavePersistant();
			}
		}

		// collect the just loaded ChunkStacks
		chunkLoader.loadChunkStacksInOpenglThread();

		// load all the chunks, for which all four neighbours have been loaded
		loadChunkStacks();

		updateLoadedChunks();
		updateChunkSystem();
	}

	void Level::updateOnlyFrustum(float delta) {
		player.getFrustum().update();
	}

	void Level::addChunkStackToLoad(Level_AccessKey key, ChunkStack& c) {
		toLoad.push_back(&c);
	}

	void Level::removeFromLoadingList(Level_AccessKey key, ChunkStack& c) {
		// remove element from loading list
		toLoad.erase(std::remove(toLoad.begin(), toLoad.end(), &c), toLoad.end());
	}

	void Level::putChunkStack(Level_AccessKey key, ChunkStack* p_c) {
		glm::ivec2 pos = p_c->getChunkStackPosition();
		// check if key already exists
		auto it = allChunkStacks.find(pos);
		if(it != allChunkStacks.end()) {
			// key already exists, log error

		} else {
			// key doesn't exist, hence we can use insert
			allChunkStacks.insert(std::make_pair(pos, p_c));
		}

		ChunkStack* p_leftNeighbor = getChunkStackWithoutLoading({pos.x - 1, pos.y});
		ChunkStack* p_rightNeighbor = getChunkStackWithoutLoading({pos.x + 1, pos.y});
		ChunkStack* p_frontNeighbor = getChunkStackWithoutLoading({pos.x, pos.y - 1});
		ChunkStack* p_backNeighbor = getChunkStackWithoutLoading({pos.x, pos.y + 1});

		if(p_leftNeighbor != nullptr) {
			p_leftNeighbor->setRightNeighbor({}, p_c);
			p_c->setLeftNeighbor({}, p_leftNeighbor);
		}
		if(p_rightNeighbor != nullptr) {
			p_rightNeighbor->setLeftNeighbor({}, p_c);
			p_c->setRightNeighbor({}, p_rightNeighbor);
		}
		if(p_frontNeighbor != nullptr) {
			p_frontNeighbor->setBackNeighbor({}, p_c);
			p_c->setFrontNeighbor({}, p_frontNeighbor);
		}
		if(p_backNeighbor != nullptr) {
			p_backNeighbor->setFrontNeighbor({}, p_c);
			p_c->setBackNeighbor({}, p_backNeighbor);
		}
	}

	ChunkIO& Level::getChunkIO() {
		return chunkLoader;
	}

	AbstractChunkVaoManager& Level::getVaoManager() const {
		return vaoManager;
	}

	Player& Level::getPlayer() {
		return player;
	}

	glm::vec4 Level::getBackgroundColor() const {
		return backgroundColor;
	}

	float Level::getSeed() const {
		return levelMetadata->getSeed();
	}

	std::string Level::getPathToLevelFolder() const {
		return levelFolder;
	}

	std::string Level::getLevelName() const {
		return levelMetadata->getLevelName();
	}

	egui::EventManager<BlockBreakEvent>& Level::getBlockBreakEventManager() {
		return blockBreakEventManager;
	}

	egui::EventManager<BlockPlaceEvent>& Level::getBlockPlaceEventManager() {
		return blockPlaceEventManager;
	}

	egui::EventManager<ToggleSprintingEvent>& Level::getToggleSprintingEventManager() {
		return toggleSprintingEventManager;
	}

	float Level::getSecondsUntilAutosave() const {
		return AUTOSAVE_INTERVAL - timeSinceAutoSave;
	}

	ctpl::thread_pool& Level::getThreadPool() {
		return threadPool;
	}

	/*void Level::addParticle(Particle* p) {
		particles.add(*p);
	}

	ParticleList& Level::getParticleList() {
		return particles;
	}*/

	std::vector<Chunk*>& Level::getVisibleChunks() {
		return visibleChunks;
	}

	namespace {
		struct Intersection {
			Block* p_block;
			float range;
			Face face;
		};
	}

	tl::optional<Face> Level::getIntersectedBlock(float range, Block*& result_block) {
		PlayerPosition playerPos = player.getPosition();
		
		ChunkStack* playerChunkStack = getChunkStackWithoutLoading({ convertWorldToChunkValue(playerPos.x), convertWorldToChunkValue(playerPos.z) });
		ChunkStack* chunkStacksToCheck[5] = { playerChunkStack, playerChunkStack->getLeftNeighbor(), playerChunkStack->getRightNeighbor(), playerChunkStack->getFrontNeighbor(), playerChunkStack->getBackNeighbor() };

		int const rangeInt = int(ceilf(range));
		int const playerPosIntX = int(ceilf(playerPos.x));
		int const playerPosIntZ = int(ceilf(playerPos.z));


		std::vector<Intersection> intersections;
		for(int y = playerPos.getIntY() - rangeInt; y < playerPos.getIntY() + rangeInt; y++) {
			if(y < 0 || y >= Chunk::CHUNK_SIZE * ChunkStack::AMOUNT_OF_CHUNKS) continue;

			int chunkY = convertWorldToChunkValue(y);
			int inChunkY = convertWorldToInChunkValue(y);

			for(int x = playerPosIntX - rangeInt - 1; x < playerPosIntX + rangeInt; x++) {
				int inChunkX = convertWorldToInChunkValue(x);

				for(int z = playerPosIntZ - rangeInt - 1; z < playerPosIntZ + rangeInt; z++) {
					int inChunkZ = convertWorldToInChunkValue(z);

					Block* p_b = nullptr;
					for(auto& chunkStack : chunkStacksToCheck) {
						if(chunkStack == nullptr || (! chunkStack->isInside(x, z))) continue;

						p_b = chunkStack->getChunk(chunkY)->getBlock(inChunkX, inChunkY, inChunkZ);
						break;
					}

					if(p_b == nullptr || ! (p_b->isInFrustum())) continue;
					tl::optional<std::pair<float, Face>> r = p_b->checkIntersectionAndGetFace();
					if(r.has_value()) {
						intersections.push_back({p_b, float(r.value().first), r.value().second});
					}
				}
			}
		}

		std::sort(intersections.begin(), intersections.end(), [](const Intersection& a, const Intersection& b) -> bool {
			// this lambda returns true if the first argument should before the second one
			// we want to sort the list in ascending order
			// => return true if a < b

			return a.range < b.range;
		});

		if(! intersections.empty()) {
			result_block = intersections[0].p_block;

			return intersections[0].face;
		} 

		return tl::nullopt;
	}

	Block* Level::getBlockAt(const glm::ivec3& worldPosition) {
		glm::ivec3 chunkPos = convertWorldToChunkCoordinates(worldPosition);
		if(worldPosition.y < 0 || chunkPos.y >= ChunkStack::AMOUNT_OF_CHUNKS) return nullptr;

		ChunkStack* p_cStack = getChunkStackAt({chunkPos.x, chunkPos.z});
		Chunk* p_c = p_cStack->getChunk(chunkPos.y);
		return p_c->getBlock(convertWorldToInChunkValue(float(worldPosition.x)), convertWorldToInChunkValue(float(worldPosition.y)), convertWorldToInChunkValue(float(worldPosition.z)));
	}

	Block* Level::getBlockAt(int worldX, int worldY, int worldZ) {
		return getBlockAt({worldX, worldY, worldZ});
	}

	bool Level::isBlockAir(const glm::ivec3& worldPosition) {
		return getBlockAt(worldPosition) == nullptr;
	}

	Block* Level::getBlockUnder(const glm::vec3& worldPosition) {
		int x = vcround(worldPosition.x);
		int y = vcroundY(worldPosition.y);
		int z = vcround(worldPosition.z);

		for(int computeY = y; computeY >= 0; computeY--) {
			Block* p_b = getBlockAt(x, computeY, z);
			if(! (p_b == nullptr)) return p_b;
		}

		return nullptr;
	}

	Block* Level::getBlockUnder(float worldX, float worldY, float worldZ) {
		return getBlockUnder({worldX, worldY, worldZ});
	}

	Chunk* Level::getChunkAt(const glm::ivec3& chunkCoordinates) {
		return getChunkStackAt({chunkCoordinates.x, chunkCoordinates.z})->getChunk(chunkCoordinates.y);
	}

	ChunkStack* Level::getChunkStackAt(const glm::ivec2& chunkStackCoordinates) {
		auto it = allChunkStacks.find(chunkStackCoordinates);
		if(it == allChunkStacks.end()) {
			return chunkLoader.loadImmediately(chunkStackCoordinates);
		} else {
			return it->second;
		}
	}

	ChunkStack* Level::getChunkStackWithoutLoading(const glm::ivec2& chunkStackCoordinates) const {
		auto it = allChunkStacks.find(chunkStackCoordinates);
		if(it == allChunkStacks.end()) {
			return nullptr;
		} else {
			return it->second;
		}
	}

	void Level::putBlockWithoutUpdate(Block* p_block, glm::ivec3 worldPosition, Player* p_triggeringPlayer) {
		glm::ivec3 chunkCoords = convertWorldToChunkCoordinates(worldPosition);
		glm::ivec3 inChunkCoords = convertWorldToInChunkCoordinates(worldPosition);

		Chunk* p_c = getChunkAt(chunkCoords);
		p_c->placeBlock(p_block, inChunkCoords.x, inChunkCoords.y, inChunkCoords.z, p_triggeringPlayer, true, false);
	}

	void Level::putBlockWithUpdate(Block* p_block, glm::ivec3 worldPosition, Player* p_triggeringPlayer) {
		glm::ivec3 chunkCoords = convertWorldToChunkCoordinates(worldPosition);
		glm::ivec3 inChunkCoords = convertWorldToInChunkCoordinates(worldPosition);

		Chunk* p_c = getChunkAt(chunkCoords);
		p_c->placeBlock(p_block, inChunkCoords.x, inChunkCoords.y, inChunkCoords.z, p_triggeringPlayer);

	}

	bool Level::tryToPlaceBlock(glm::ivec3 motherWorldPosition, Face face, Face placedOnFace, const std::shared_ptr<BlockType>& toPlace, Player* p_triggeringPlayer) {
		switch(face) {
			case FRONT:
				motherWorldPosition.z--;
				break;
			case BACK:
				motherWorldPosition.z++;
				break;
			case LEFT:
				motherWorldPosition.x++;
				break;
			case RIGHT:
				motherWorldPosition.x--;
				break;
			case BOTTOM:
				motherWorldPosition.y--;
				break;
			case TOP:
				motherWorldPosition.y++;
				break;
			default:
				throw std::logic_error("invalid facing");
		}

		return tryToPlaceBlock(motherWorldPosition, placedOnFace, toPlace, p_triggeringPlayer);
	}

	bool Level::tryToPlaceBlock(glm::ivec3 worldPosition, Face placedOnFace, const std::shared_ptr<BlockType>& toPlace, Player* p_triggeringPlayer) {
		if(worldPosition.y >= Chunk::CHUNK_SIZE * ChunkStack::AMOUNT_OF_CHUNKS) return false;

		Block* p_current = getBlockAt(worldPosition);
		Chunk* p_chunk = getChunkAt(convertWorldToChunkCoordinates(worldPosition));

		if(! player.doesPlayerCollidateWithBlock(worldPosition) && (p_current == nullptr || p_current->getBlockType().canBeReplaced())) {
			putBlockWithUpdate(createBlock(worldPosition.x, worldPosition.y, worldPosition.z, toPlace, *p_chunk, placedOnFace), worldPosition, p_triggeringPlayer);
			return true;
		}
		return false;
	}

}
