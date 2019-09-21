#include "BlockType.h"

#include <unordered_map>
#include "Bedrock.h"
#include "Block.h"


namespace vc {
	std::map<int, std::shared_ptr<BlockType>> BlockType::ALL_VALUES = {};

	std::map<std::string, int> TextureFile::allTextureIds = {};

	namespace {
		const std::function<const TextureOrientation&(Face&)> getAllTopTexOrientation = [](Face&) -> const TextureOrientation& {
			static TextureOrientation TEX_ORIENTATION = TextureOrientation::getTextureOrientation(TOP, TOP, TOP, TOP, TOP, TOP);
			return TEX_ORIENTATION;
		};
		const std::function<const TextureOrientation&(Face&)> getLogTexOrientation = [](Face& placedOn) -> const TextureOrientation& {
			switch(placedOn) {
				case TOP:
				case BOTTOM:
					return TextureOrientation::getTextureOrientation(TOP, FRONT, TOP, FRONT, FRONT, FRONT);
				case LEFT:
				case RIGHT:
					return TextureOrientation::getTextureOrientation(FRONT, FRONT, FRONT, FRONT, TOP, TOP);
				case FRONT:
				case BACK:
					return TextureOrientation::getTextureOrientation(FRONT, TOP, FRONT, TOP, FRONT, FRONT);

			}

			// this position is never reached
			throw std::logic_error("");
		};
		const constructor blockConstructor = [](const uint8_t& inChunkX, const uint8_t& inChunkY, const uint8_t& inChunkZ, const BlockType* blockType, Chunk& c, const TextureOrientation& texOrientation, const int8_t metadata) -> Block* {
			return new Block(inChunkX, inChunkY, inChunkZ, blockType, c, texOrientation, metadata);
		};
	}
	const std::shared_ptr<BlockType> BlockType::COBBLESTONE(new BlockType(
			0,
			"cobblestone",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"cobblestone"}
	));
	const std::shared_ptr<BlockType> BlockType::STONE(new BlockType(
			1,
			"stone",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"stone"}
	));
	const std::shared_ptr<BlockType> BlockType::BEDROCK(new BlockType(
			2,
			"bedrock",
			[](const uint8_t& inChunkX, const uint8_t& inChunkY, const uint8_t& inChunkZ, const BlockType* blockType, Chunk& c, const TextureOrientation& texOrientation, const int8_t& metadata) -> Block* {
				return new Bedrock(inChunkX, inChunkY, inChunkZ, c, texOrientation, metadata);
			},
			getAllTopTexOrientation,
			false,
			false,
			{"bedrock"}
	));
	const std::shared_ptr<BlockType> BlockType::DIRT(new BlockType(
			3,
			"dirt",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"dirt"}
	));
	const std::shared_ptr<BlockType> BlockType::GRASS (new BlockType(
			4,
			"grass",
			blockConstructor,
			[](Face&) -> const TextureOrientation& {
				return TextureOrientation::getTextureOrientation(TOP, FRONT, BOTTOM, FRONT, FRONT, FRONT);
			},
			false,
			true,
			{{"grass_block_top"}}, {{"grass_block_side"}}, {{"dirt"}}
	));
	const std::shared_ptr<BlockType> BlockType::STONE_BRICKS (new BlockType(
			5,
			"stoneBricks",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"stone_bricks"}
	));
	const std::shared_ptr<BlockType> BlockType::DIORITE (new BlockType(
			6,
			"diorite",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"diorite"}
	));
	const std::shared_ptr<BlockType> BlockType::SMOOTH_DIORITE (new BlockType(
			7,
			"smoothDiorite",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"diorite_smooth"}
	));
	const std::shared_ptr<BlockType> BlockType::GRANITE (new BlockType(
			8,
			"granite",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"granite"}
	));
	const std::shared_ptr<BlockType> BlockType::SMOOTH_GRANITE (new BlockType(
			9,
			"smoothGranite",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"granite_smooth"}
	));
	const std::shared_ptr<BlockType> BlockType::ANDESITE (new BlockType(
			10,
			"andesite",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"andesite"}
	));
	const std::shared_ptr<BlockType> BlockType::SMOOTH_ANDESITE (new BlockType(
			11,
			"smoothAndesite",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"andesite_smooth"}
	));

	const std::shared_ptr<BlockType> BlockType::OAK_LOG (new BlockType(
			12,
			"oakLog",
			blockConstructor,
			getLogTexOrientation,
			false,
			true,
			{{"oak_log_top"}}, {{"oak_log"}}
	));
	const std::shared_ptr<BlockType> BlockType::OAK_PLANKS (new BlockType(
			13,
			"oakPlanks",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"oak_planks"}
	));
	const std::shared_ptr<BlockType> BlockType::DARK_OAK_LOG (new BlockType(
			14,
			"darkOakLog",
			blockConstructor,
			getLogTexOrientation,
			false,
			true,
			{{"dark_oak_log_top"}}, {{"oak_log"}}
	));
	const std::shared_ptr<BlockType> BlockType::DARK_OAK_PLANKS (new BlockType(
			15,
			"darkOakPlanks",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"dark_oak_planks"}
	));
	const std::shared_ptr<BlockType> BlockType::BIRCH_LOG (new BlockType(
			16,
			"birchLog",
			blockConstructor,
			getLogTexOrientation,
			false,
			true,
			{{"birch_log_top"}}, {{"birch_log"}}
	));
	const std::shared_ptr<BlockType> BlockType::BIRCH_PLANKS (new BlockType(
			17,
			"birchPlanks",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"birch_planks"}
	));
	const std::shared_ptr<BlockType> BlockType::JUNGLE_LOG (new BlockType(
			18,
			"jungleLog",
			blockConstructor,
			getLogTexOrientation,
			false,
			true,
			{{"jungle_log_top"}}, {{"jungle_log"}}
	));
	const std::shared_ptr<BlockType> BlockType::JUNGLE_PLANKS (new BlockType(
			19,
			"junglePlanks",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"jungle_planks"}
	));
	const std::shared_ptr<BlockType> BlockType::ACACIA_LOG (new BlockType(
			20,
			"acaciaLog",
			blockConstructor,
			getLogTexOrientation,
			false,
			true,
			{{"acacia_log_top"}}, {{"acacia_log"}}
	));
	const std::shared_ptr<BlockType> BlockType::ACACIA_PLANKS (new BlockType(
			21,
			"acaciaPlanks",
			blockConstructor,
			getAllTopTexOrientation,
			false,
			true,
			{"acacia_planks"}
	));

	STATIC_INIT(BlockType) {
		BlockType::addBlockType(COBBLESTONE);
		BlockType::addBlockType(STONE);
		BlockType::addBlockType(BEDROCK);
		BlockType::addBlockType(DIRT);
		BlockType::addBlockType(GRASS);
		BlockType::addBlockType(STONE_BRICKS);
		BlockType::addBlockType(DIORITE);
		BlockType::addBlockType(SMOOTH_DIORITE);
		BlockType::addBlockType(GRANITE);
		BlockType::addBlockType(SMOOTH_GRANITE);
		BlockType::addBlockType(ANDESITE);
		BlockType::addBlockType(SMOOTH_ANDESITE);
		
		BlockType::addBlockType(OAK_LOG);
		BlockType::addBlockType(OAK_PLANKS);
		BlockType::addBlockType(DARK_OAK_LOG);
		BlockType::addBlockType(DARK_OAK_PLANKS);
		BlockType::addBlockType(BIRCH_LOG);
		BlockType::addBlockType(BIRCH_PLANKS);
		BlockType::addBlockType(JUNGLE_LOG);
		BlockType::addBlockType(JUNGLE_PLANKS);
		BlockType::addBlockType(ACACIA_LOG);
		BlockType::addBlockType(ACACIA_PLANKS);

	}


	TextureFile::TextureFile(std::string fileName) :
			path(fileName) {
		static int texIdCounter = 0;

		if(allTextureIds.find(fileName) == allTextureIds.end()) {
			int texId = texIdCounter++;
			allTextureIds.insert(std::make_pair(fileName, texId));
			this->globalTextureId = texId;
		} else {
			this->globalTextureId = allTextureIds.at(fileName);
		}
	}

	int TextureFile::getGlobalTextureId() const {
		return globalTextureId;
	}

	std::string TextureFile::getPath() const {
		return path;
	}

	void BlockType::addBlockType(const std::shared_ptr<BlockType>& blockType) {
		if(ALL_VALUES.find(blockType->getId()) != ALL_VALUES.end()) {
			throw std::runtime_error("this id does already exist.");
		} else {
			ALL_VALUES.insert(std::make_pair(blockType->getId(), blockType));
		}
	}

	const std::shared_ptr<BlockType>& BlockType::getBlockTypeById(int id) {
		if(ALL_VALUES.find(id) == BlockType::ALL_VALUES.end()) {
			throw std::runtime_error("this id doesn't exist");
		} else {
			return ALL_VALUES.at(id);
		}
	}


	const std::map<int, std::shared_ptr<BlockType>>& BlockType::getAll() {
		return ALL_VALUES;
	}

	BlockType::BlockType(uint32_t id, std::string name, constructor constructorFunction,
			std::function<const TextureOrientation& (Face&)> getTexOrientation,
			bool canBeReplaced, bool canPlayerPlace,
			tl::optional<TextureFile> top, tl::optional<TextureFile> front, tl::optional<TextureFile> bottom, tl::optional<TextureFile> back, tl::optional<TextureFile> left, tl::optional<TextureFile> right) :
				id(id),
				name(name),
				field_canBeReplaced(canBeReplaced),
				field_canPlayerPlace(canPlayerPlace),
				field_constructorFunction(constructorFunction),
				field_getTexOrientation(getTexOrientation) {

		this->textureFiles[0] = top;
		this->textureFiles[1] = front;
		this->textureFiles[2] = bottom;
		this->textureFiles[3] = back;
		this->textureFiles[4] = left;
		this->textureFiles[5] = right;

	}

	BlockType::BlockType(uint32_t id, std::string name, constructor constructorFunction,
			std::function<const TextureOrientation& (Face&)> getTexOrientation,
			bool canBeReplaced, bool canPlayerPlace,
			TextureFile top) :
				id(id),
				name(name),
				field_canBeReplaced(canBeReplaced),
				field_canPlayerPlace(canPlayerPlace),
				field_constructorFunction(constructorFunction),
				field_getTexOrientation(getTexOrientation) {

		this->textureFiles[0].emplace(top);

	}

	uint32_t BlockType::getId() const {
		return id;
	}

	const TextureOrientation& BlockType::getTextureOrientation(Face placedOn) const {
		return field_getTexOrientation(placedOn);
	}

	std::string BlockType::getName() const {
		return name;
	}

	bool BlockType::canBeReplaced() const {
		return field_canBeReplaced;
	}

	bool BlockType::canPlayerPlace() const {
		return field_canPlayerPlace;
	}

	bool BlockType::isBlock() const {
		return true;
	}

	int BlockType::getMaxStackSize() const {
		return 64;
	}

	const std::array<tl::optional<TextureFile>, 6>& BlockType::getTextureFiles() const {
		return textureFiles;
	}


	Block* BlockType::instantiate(const uint8_t& inChunkX, const uint8_t& inChunkY, const uint8_t& inChunkZ, Chunk& c, const TextureOrientation& texOrientation, const uint8_t& metadata) const {
		return field_constructorFunction(inChunkX, inChunkY, inChunkZ, this, c, texOrientation, metadata);
	}


}
