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
#include "..//renderModel/inventoryGui/CreativeInventoryGUI.h"

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
			bool isButtonToPlaceBlockDown = false, isButtonToDestroyBlockDown = false;
			int lastFrame_blockBelowPosY = 0;
			long long time_blockBelowPosYChanged = 0;

			std::shared_ptr<InventoryGUI> displayedInventoryGui;

			PlayerInventory inventory;
			int activeHotbarIndex = 0;
			Slot itemClipboard;

			egui::EventManager<ItemInHandChangedEvent> itemInHandChangeEventManager;

			egui::FunctionWrapper<egui::ScrollEvent> scrollEventHandler;
			egui::FunctionWrapper<egui::MouseEvent> mouseBtnEventHandler;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			Player(PlayerPosition playerPos, Level& level);
			~Player();
			Player(const Player&) = delete;

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		private:
			

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::FunctionWrapper<egui::ScrollEvent> initScrollEventHandler();
			egui::FunctionWrapper<egui::MouseEvent> initMouseBtnEventHandler();

			void computePlacingBlocks();
			void updateViewMatrix();
			int getBlockUnder();
			int getPosYOr0(Block* b);
			void computeMovementY(float delta, bool respondToKeyPress);
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
			std::shared_ptr<GameItem> getItemTypeInHand() const override;

			PlayerInventory& getInventory();
			Slot& getItemClipboard();
			int getActiveHotbarIndex() const;

			bool isInventoryGUIActive() const;
			std::shared_ptr<InventoryGUI> getInventoryGUI();
			void setInventoryGUI(const std::shared_ptr<InventoryGUI>& invGui);

			Frustum& getFrustum();
			MousePicker& getMousePicker();

			egui::EventManager<ItemInHandChangedEvent>& getItemInHandChangeManager();


	};


}
