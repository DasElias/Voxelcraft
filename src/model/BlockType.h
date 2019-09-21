#pragma once
#include <memory>
#include <string>
#include <array>
#include "TextureOrientation.h"
#include "GameItem.h"
#include <optional>
#include <tl/optional.hpp>
#include <functional>
#include <unordered_map>
#include <vector>

#include "Face.h"
#include <iostream>

#include <static_init.h>


namespace vc {
	class Block;
	class BlockType;
	class Chunk;
	typedef std::function<Block*(const uint8_t& inChunkX, const uint8_t& inChunkY, const uint8_t& inChunkZ, const BlockType* blockType, Chunk& c, const TextureOrientation& texOrientation, const int8_t& metadata)> constructor;

	// ----------------------------------------------------------------------
	// -----------------------------CHILD-CLASS------------------------------
	// ----------------------------------------------------------------------

		class TextureFile {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			private:
				static std::map<std::string, int> allTextureIds;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				int globalTextureId;
				std::string path;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				TextureFile(std::string fileName);

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				int getGlobalTextureId() const;
				std::string getPath() const;

		};

	class BlockType : public GameItem {
		

		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::map<int, std::shared_ptr<BlockType>> ALL_VALUES;

		public:
			static std::shared_ptr<BlockType> const COBBLESTONE;
			static std::shared_ptr<BlockType> const STONE;
			static std::shared_ptr<BlockType> const BEDROCK;
			static std::shared_ptr<BlockType> const DIRT;
			static std::shared_ptr<BlockType> const GRASS;
			static std::shared_ptr<BlockType> const STONE_BRICKS;
			static std::shared_ptr<BlockType> const DIORITE;
			static std::shared_ptr<BlockType> const SMOOTH_DIORITE;
			static std::shared_ptr<BlockType> const GRANITE;
			static std::shared_ptr<BlockType> const SMOOTH_GRANITE;
			static std::shared_ptr<BlockType> const ANDESITE;
			static std::shared_ptr<BlockType> const SMOOTH_ANDESITE;

			static std::shared_ptr<BlockType> const OAK_LOG;
			static std::shared_ptr<BlockType> const OAK_PLANKS;
			static std::shared_ptr<BlockType> const DARK_OAK_LOG;
			static std::shared_ptr<BlockType> const DARK_OAK_PLANKS;
			static std::shared_ptr<BlockType> const BIRCH_LOG;
			static std::shared_ptr<BlockType> const BIRCH_PLANKS;
			static std::shared_ptr<BlockType> const JUNGLE_LOG;
			static std::shared_ptr<BlockType> const JUNGLE_PLANKS;
			static std::shared_ptr<BlockType> const ACACIA_LOG;
			static std::shared_ptr<BlockType> const ACACIA_PLANKS;

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static void addBlockType(const std::shared_ptr<BlockType>& blockType);
			static const std::shared_ptr<BlockType>& getBlockTypeById(int id);
			static const std::map<int, std::shared_ptr<BlockType>>& getAll();

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			uint32_t id;
			std::string name;
			bool field_canBeReplaced;
			bool field_canPlayerPlace;

			constructor field_constructorFunction;
			std::function<const TextureOrientation&(Face&)> field_getTexOrientation;
			std::array<tl::optional<TextureFile>, 6> textureFiles;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			BlockType(uint32_t id, std::string name, constructor constructorFunction, std::function<const TextureOrientation&(Face&)> getTexOrientation, bool canBeReplaced, bool canPlayerPlace, tl::optional<TextureFile> top, tl::optional<TextureFile> front, tl::optional<TextureFile> bottom = {}, tl::optional<TextureFile> back = {}, tl::optional<TextureFile> left = {}, tl::optional<TextureFile> right = {});
			BlockType(uint32_t id, std::string name, constructor constructorFunction, std::function<const TextureOrientation&(Face&)> getTexOrientation, bool canBeReplaced, bool canPlayerPlace, TextureFile top);
			BlockType(const BlockType&) = delete;
			BlockType& operator=(const BlockType&) = delete;
			DECLARE_STATIC_INIT(BlockType);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			uint32_t getId() const;
			const TextureOrientation& getTextureOrientation(Face placedOn = TOP) const;
			std::string getName() const override;
			bool canBeReplaced() const;
			bool canPlayerPlace() const;
			bool isBlock() const override;
			int getMaxStackSize() const override;
			const std::array<tl::optional<TextureFile>, 6>& getTextureFiles() const;

			Block* instantiate(const uint8_t& inChunkX, const uint8_t& inChunkY, const uint8_t& inChunkZ, Chunk& c, const TextureOrientation& texOrientation, const uint8_t& metadata) const;
	};


}
