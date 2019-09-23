#pragma once

#include "input/IOHandler.h"
#include <model/events/EventManager.h>
#include "model/events/ScrollEvent.h"
#include "model/events/MouseEvent.h"
#include "model/events/FunctionWrapper.h"
#include "BlockType.h"
#include "Frustum.h"
#include "MousePicker.h"
#include "PlayerPosition.h"
#include "AbstractPlayer.h"
#include "Slot.h"
#include "PlayerInventory.h"

namespace vc {
	class Level;
	class Block;
	class ItemInHandChangedEvent;

	class Player : public AbstractPlayer {

		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const REJUMP_ON_Y_CHANGE_DELAY_MS;
			static float const COLLISION_TOLERANCE;
			static float const GET_BLOCK_UNDER_TOLERANCE;
			static float const BASIC_SPEED;
			static float const SPRINTING_SPEED;
			static float const ROTATION_SENSIBILITY;
			static float const GRAVITY;
			static float const JUMP_POWER;
			static float const MAX_JUMP_HEIGHT;
			static int const NOT_MOVING_TO_SPRINT_MS;

		public:
			static int const BLOCK_PLACE_DELAY_MS;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			bool isSprinting = false;
			long long time_movingForward = 0;
			long long time_notMovingForward = 0;
			long long time_blockPlace = 0;
			PlayerPosition position;
			float yJumpStart;
			glm::vec3 rotation = glm::vec3(0);
			float yAcceleration = 0;
			Frustum frustum;
			MousePicker picker;
			Level& level;
			glm::mat4x4 viewMatrix;
			std::shared_ptr<BlockType> blockInHand;
			bool isButtonToPlaceBlockDown = false, isButtonToDestroyBlockDown = false;
			int lastFrame_blockBelowPosY = 0;
			long long time_blockBelowPosYChanged = 0;

			PlayerInventory inventory;
			Slot itemClipboard;

			egui::EventManager<ItemInHandChangedEvent> itemInHandChangeEventManager;

			egui::FunctionWrapper<egui::ScrollEvent> scrollEventHandler = egui::FunctionWrapper<egui::ScrollEvent>([this](egui::ScrollEvent& event) {
				float yOffset = event.getYOffset();
				if (abs(yOffset) > 0.5) {
					const std::map<int, std::shared_ptr<BlockType>>& allTypes = BlockType::getAll();
					int currentId = blockInHand->getId();

					// iterator to the BlockType, which is held by the player at the moment
					std::map<int, std::shared_ptr<BlockType>>::const_iterator startIterator = allTypes.find(currentId);

					// clone iterator
					std::map<int, std::shared_ptr<BlockType>>::const_iterator it(startIterator);
					do {
						// check if it does exist in the map
						if(it == allTypes.end()) {
							throw std::logic_error("Element doesn't exist in map!");
						}

						// get next element
						it = std::next(it);

						// check if we are at the end of the map
						if(it == allTypes.end()) {
							it = allTypes.begin();
						}

						if(startIterator == it) break;
					} while(! it->second->canPlayerPlace());

					setBlockTypeInHand(it->second);
				}
			});
			egui::FunctionWrapper<egui::MouseEvent> mouseBtnEventHandler = egui::FunctionWrapper<egui::MouseEvent>([this](egui::MouseEvent& event) {
				if(event.getMouseButton() == egui::getKeyAssignments().getProperty("PLACE_BLOCK")) isButtonToPlaceBlockDown = event.isBtnDown();
				else if (event.getMouseButton() == egui::getKeyAssignments().getProperty("BREAK_BLOCK")) isButtonToDestroyBlockDown = event.isBtnDown();

				computePlacingBlocks();
			});


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Player(PlayerPosition playerPos, Level& level);
			~Player();
			Player(const Player&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void computePlacingBlocks();
			void updateViewMatrix();
			int getBlockUnder();
			int getPosYOr0(Block* b);
			void computeMovementY(float delta);
			void computeMovementXZ(float delta);
			void computeRotation();
			bool fireToggleSprintingEvent(bool newState);

		public:
			void computeMove(float delta);
			PlayerPosition getPosition() const override;
			void setPosition(float x, float y, float z) override;
			glm::ivec3 getChunkCoordinates() const override;
			bool doesPlayerCollidateWithBlock(int worldX, int worldY, int worldZ) const override;
			bool doesPlayerCollidateWithBlock(const glm::ivec3& position) const override;

			glm::vec3 getRotation() const override;
			void setRotation(glm::vec3 rotation) override;
			glm::mat4x4 computeViewMatrix() const override;
			std::shared_ptr<BlockType> getBlockTypeInHand() const override;
			void setBlockTypeInHand(const std::shared_ptr<BlockType>& blockInHand) override;

			PlayerInventory& getInventory();
			Slot& getItemClipboard();

			Frustum& getFrustum();
			MousePicker& getMousePicker();

			egui::EventManager<ItemInHandChangedEvent>& getItemInHandChangeManager();


	};


}
