#define NOMINMAX
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Player.h"

#include "utils/VectorUtils.h"
#include "utils/TimeUtils.h"
#include "Block.h"
#include <algorithm>
#include <math.h>

#include "input/IOHandler.h"

#include "utils/MathUtils.h"
#include "events/player/ItemInHandChangedEvent.h"

using namespace egui;

namespace vc {
	const int Player::REJUMP_ON_Y_CHANGE_DELAY_MS = 100;
	const float Player::COLLISION_TOLERANCE = 0.3f;
	const float Player::GET_BLOCK_UNDER_TOLERANCE = 0.2f;
	const float Player::BASIC_SPEED = 3.2f;
	const float Player::SPRINTING_SPEED = 5.6f;
	const float Player::ROTATION_SENSIBILITY = 0.5f;
	const float Player::GRAVITY = 21;
	const float Player::JUMP_POWER = 6.8f;
	const float Player::MAX_JUMP_HEIGHT = 1.4f;
	const int Player::NOT_MOVING_TO_SPRINT_MS = 100;
	const int Player::BLOCK_PLACE_DELAY_MS = 100;


	Player::Player(PlayerPosition playerPos, Level& level) :
			position(playerPos),
			yJumpStart(playerPos.y),
			frustum(*this),
			picker(*this),
			level(level),
			viewMatrix(1),
			scrollEventHandler(initScrollEventHandler()),
			mouseBtnEventHandler(initMouseBtnEventHandler()) {


		getInputHandler().getScrollEventManager().addEventHandler(scrollEventHandler);
		getInputHandler().getMouseBtnEventManager().addEventHandler(mouseBtnEventHandler);
	}

	Player::~Player() {
		getInputHandler().getScrollEventManager().removeEventHandler(scrollEventHandler);
		getInputHandler().getMouseBtnEventManager().removeEventHandler(mouseBtnEventHandler);
	}


	egui::FunctionWrapper<egui::ScrollEvent> Player::initScrollEventHandler() {
		return egui::FunctionWrapper<egui::ScrollEvent>([this](egui::ScrollEvent& event) {
			// if an inventory GUI is active or the current state is not IngameState, the selected hotbar item shouldn't change
			if(isInventoryGUIActive() || !isIngameStateActive_field) return;

			float yOffset = event.getYOffset();
			if(abs(yOffset) > 0.5) {
				int newIndex = activeHotbarIndex + (yOffset < 0 ? -1 : 1);
				if(newIndex >= 9) newIndex = 0;
				else if(newIndex < 0) newIndex = 8;

				ItemInHandChangedEvent event(this, inventory.get(activeHotbarIndex).getGameItem(), inventory.get(newIndex).getGameItem());
				itemInHandChangeEventManager.fireEvent(event);
				if(event.isCancelled()) return;

				activeHotbarIndex = newIndex;
			}
		});
	}

	egui::FunctionWrapper<egui::MouseEvent> Player::initMouseBtnEventHandler() {
		return egui::FunctionWrapper<egui::MouseEvent>([this](egui::MouseEvent& event) {
			// if the current state isn't IngameState, the player doesn't want to place/destroy a block
			if(! isIngameStateActive_field) return;
			if(isInventoryGUIActive()) {
				getInventoryGUI()->click(*this, event.getScreenX_percent(), event.getScreenY_percent(), event.isBtnDown());
				return;
			}

			if(event.getMouseButton() == egui::getKeyAssignments().getProperty("PLACE_BLOCK")) isButtonToPlaceBlockDown = event.isBtnDown();
			else if(event.getMouseButton() == egui::getKeyAssignments().getProperty("BREAK_BLOCK")) isButtonToDestroyBlockDown = event.isBtnDown();

			computePlacingBlocks();
		});
	}


	void Player::computeMove(float delta) {
		bool computeMovement = ! isInventoryGUIActive();

		/*
		 * When an inventory is open, we don't have to compute rotation and movement in XZ-direction
		 */
		if(computeMovement) computeRotation();
		if(computeMovement) computeMovementXZ(delta);

		/*
		 * The game shouldn't respond to the player's keypresses while an inventory is open, nevertheless we have to 
		 * apply the gravity to the player's y-position.
		 */
		computeMovementY(delta, computeMovement);

		/*
		 * We can compute the block placing even though the GUI inventory is active, since we discard
		 * an click event which is received while an inventory is opened instantly in the event handler.
		 */
		computePlacingBlocks();
		
		/*
		 * View matrix has to be recomputed, since the y-position may have changed.
		 */
		updateViewMatrix();

		picker.update();
		frustum.update();

	}

	

	void Player::computePlacingBlocks() {
		Block* p_focusedBlockResult = nullptr;

		if(isButtonToPlaceBlockDown && time_blockPlace + BLOCK_PLACE_DELAY_MS < getMilliseconds()) {
			tl::optional<Face> focusedFace = level.getIntersectedBlock(6, p_focusedBlockResult);
			std::shared_ptr<GameItem> gameItemInHand = getItemTypeInHand();

			if(p_focusedBlockResult != nullptr && focusedFace.has_value() && gameItemInHand && gameItemInHand->isBlock()) {
				std::shared_ptr<BlockType> blockTypeInHand = std::dynamic_pointer_cast<BlockType>(gameItemInHand);
				if(! blockTypeInHand) throw std::logic_error("isBlock() of GameItem returns true, but in fact, it isn't of type BlockType");

				glm::ivec3 focusedPosition(p_focusedBlockResult->getWorldX(), p_focusedBlockResult->getWorldY(), p_focusedBlockResult->getWorldZ());
				level.tryToPlaceBlock(focusedPosition, focusedFace.value(), focusedFace.value(), blockTypeInHand, this);
				time_blockPlace = getMilliseconds();
			}
		} else if (isButtonToDestroyBlockDown && time_blockPlace + BLOCK_PLACE_DELAY_MS < getMilliseconds()) {
			level.getIntersectedBlock(6, p_focusedBlockResult);

			if(p_focusedBlockResult != nullptr) {
				glm::ivec3 focusedPosition(p_focusedBlockResult->getWorldX(), p_focusedBlockResult->getWorldY(), p_focusedBlockResult->getWorldZ());
				level.putBlockWithUpdate(nullptr, focusedPosition, this);
				time_blockPlace = getMilliseconds();
			}
		}
	}

	void Player::updateViewMatrix() {
		viewMatrix = glm::mat4x4(1);

		viewMatrix = glm::rotate(viewMatrix, toRadiands(getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, toRadiands(getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, toRadiands(getRotation().z), glm::vec3(0, 0, 1));

		// the scene should move in the opposite direction that it looks like the camera is moving
		glm::vec3 pos = getPosition();
		glm::vec3 negativeCameraPosition = ((pos + glm::vec3(0, 1.5f, 0)) * -1.0f);
		viewMatrix = glm::translate(viewMatrix, negativeCameraPosition);
	}

	int Player::getBlockUnder() {
		float x = position.x;
		float y = position.y;
		float z = position.z;

		int blockUnder = getPosYOr0(level.getBlockUnder(position));

		float xDecimalDigits = abs(x - ceilf(x));
		float zDecimalDigits = abs(z - ceilf(z));
		float collisionTolerance = COLLISION_TOLERANCE;

		// @formatter:off
		if(xDecimalDigits < collisionTolerance) blockUnder = std::max(getPosYOr0(level.getBlockUnder(x + 1, y, z)), blockUnder);
		if(xDecimalDigits > 1 - collisionTolerance) blockUnder = std::max(getPosYOr0(level.getBlockUnder(x - 1, y, z)), blockUnder);

		if(zDecimalDigits < collisionTolerance) blockUnder = std::max(getPosYOr0(level.getBlockUnder(x, y, z + 1)), blockUnder);
		if(zDecimalDigits > 1 - collisionTolerance) blockUnder = std::max(getPosYOr0(level.getBlockUnder(x, y, z - 1)), blockUnder);

		if(xDecimalDigits < collisionTolerance && zDecimalDigits < collisionTolerance) blockUnder = std::max(getPosYOr0(level.getBlockUnder(x + 1, y, z + 1)), blockUnder);
		if(xDecimalDigits > 1 - collisionTolerance && zDecimalDigits > 1 - collisionTolerance) blockUnder = std::max(getPosYOr0(level.getBlockUnder(x - 1, y, z - 1)), blockUnder);
		if(xDecimalDigits < collisionTolerance && zDecimalDigits > 1 - collisionTolerance) blockUnder = std::max(getPosYOr0(level.getBlockUnder(x + 1, y, z - 1)), blockUnder);
		if(xDecimalDigits > 1 - collisionTolerance && zDecimalDigits < collisionTolerance) blockUnder = std::max(getPosYOr0(level.getBlockUnder(x - 1, y, z + 1)), blockUnder);
		// @formatter:on

		return blockUnder;

	}

	int Player::getPosYOr0(Block* b) {
		return (b == nullptr) ? -1 : b->getWorldY();
	}

	void Player::computeMovementY(float delta, bool respondToKeyPress) {
		yAcceleration -= GRAVITY * delta;


		int yBlockUnderPlayerOnJumpStart = this->getBlockUnder();
		if(yBlockUnderPlayerOnJumpStart != this->lastFrame_blockBelowPosY) {
			this->lastFrame_blockBelowPosY = yBlockUnderPlayerOnJumpStart;
			this->time_blockBelowPosYChanged = getMilliseconds();
		}

		bool isInAir = true;
		// @formatter:off
		if (((time_blockBelowPosYChanged + REJUMP_ON_Y_CHANGE_DELAY_MS <= getMilliseconds()) && (
				level.getBlockAt(vcround(position.x), vcroundY(position.y - GET_BLOCK_UNDER_TOLERANCE), vcround(position.z)) != nullptr ||
				level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y - GET_BLOCK_UNDER_TOLERANCE), vcround(position.z)) != nullptr ||
				level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y - GET_BLOCK_UNDER_TOLERANCE), vcround(position.z)) != nullptr ||
				level.getBlockAt(vcround(position.x), vcroundY(position.y - GET_BLOCK_UNDER_TOLERANCE), vcround(position.z + COLLISION_TOLERANCE)) != nullptr ||
				level.getBlockAt(vcround(position.x), vcroundY(position.y - GET_BLOCK_UNDER_TOLERANCE), vcround(position.z - COLLISION_TOLERANCE)) != nullptr ||

				level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y - GET_BLOCK_UNDER_TOLERANCE), vcround(position.z + COLLISION_TOLERANCE)) != nullptr ||
				level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y - GET_BLOCK_UNDER_TOLERANCE), vcround(position.z - COLLISION_TOLERANCE)) != nullptr ||
				level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y - GET_BLOCK_UNDER_TOLERANCE), vcround(position.z - COLLISION_TOLERANCE)) != nullptr ||
				level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y - GET_BLOCK_UNDER_TOLERANCE), vcround(position.z + COLLISION_TOLERANCE)) != nullptr)
			)) {
			isInAir = false;
			yAcceleration = 0;
		}
		// @formatter:on



		if (respondToKeyPress && (!isInAir || position.y < 0.1) && getInputHandler().isKeyDown(KEY_SPACE)) {
			yAcceleration = JUMP_POWER;
			yJumpStart = position.y;
		}



		float positionAddition = yAcceleration * delta;
		int n = int(ceilf(abs(positionAddition) / GET_BLOCK_UNDER_TOLERANCE));
		float additionPart = positionAddition / n;

		// @formatter:off
		for (int counter = 0; counter < n; counter++) {
			float currentAddition = (additionPart > positionAddition) ? positionAddition : additionPart;
			positionAddition -= currentAddition;

			float newYPosition = position.y + currentAddition;

			if (level.getBlockAt(vcround(position.x), vcroundY(newYPosition), vcround(position.z)) == nullptr &&
					level.getBlockAt(vcround(position.x), vcroundY(newYPosition + 1), vcround(position.z)) == nullptr &&
					level.getBlockAt(vcround(position.x), vcroundY(newYPosition + 2 - 0.2f), vcround(position.z)) == nullptr &&

					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(newYPosition), vcround(position.z)) == nullptr &&
					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(newYPosition + 1), vcround(position.z)) == nullptr &&
					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(newYPosition + 2 - 0.2f), vcround(position.z)) == nullptr &&

					level.getBlockAt(vcround(position.x), vcroundY(newYPosition), vcround(position.z + COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x), vcroundY(newYPosition + 1), vcround(position.z + COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x), vcroundY(newYPosition + 2 - 0.2f), vcround(position.z + COLLISION_TOLERANCE)) == nullptr &&

					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(newYPosition), vcround(position.z)) == nullptr &&
					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(newYPosition + 1), vcround(position.z)) == nullptr &&
					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(newYPosition + 2 - 0.2f), vcround(position.z)) == nullptr &&

					level.getBlockAt(vcround(position.x), vcroundY(newYPosition), vcround(position.z - COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x), vcroundY(newYPosition + 1), vcround(position.z - COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x), vcroundY(newYPosition + 2 - 0.2f), vcround(position.z - COLLISION_TOLERANCE)) == nullptr &&

					// diagonal
					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(newYPosition), vcround(position.z + COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(newYPosition + 1), vcround(position.z + COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(newYPosition + 2 - 0.2f), vcround(position.z + COLLISION_TOLERANCE)) == nullptr &&

					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(newYPosition), vcround(position.z - COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(newYPosition + 1), vcround(position.z - COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(newYPosition + 2 - 0.2f), vcround(position.z - COLLISION_TOLERANCE)) == nullptr &&

					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(newYPosition), vcround(position.z - COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(newYPosition + 1), vcround(position.z - COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(newYPosition + 2 - 0.2f), vcround(position.z - COLLISION_TOLERANCE)) == nullptr &&

					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(newYPosition), vcround(position.z + COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(newYPosition + 1), vcround(position.z + COLLISION_TOLERANCE)) == nullptr &&
					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(newYPosition + 2 - 0.2f), vcround(position.z + COLLISION_TOLERANCE)) == nullptr &&

					newYPosition >= 0
			){
				position.y = newYPosition;
			}
		}
		// @formatter:on

		/*
		 * player can't jump higher than MAX_JUMP_HIGH
		 * this fixes a bug that the player jumps at very low FPS (< 10) can jump more than 2 blocks
		 */
		if (position.y > yJumpStart + MAX_JUMP_HEIGHT) position.y = yJumpStart + MAX_JUMP_HEIGHT;

	}

	void Player::computeMovementXZ(float delta) {
		glm::vec2 planeSpeed(0);

		bool movingForward = getInputHandler().isKeyDown(getKeyAssignments().getProperty("MOVE_FORWARD"));
		bool movingBackward = getInputHandler().isKeyDown(getKeyAssignments().getProperty("MOVE_BACKWARD"));
		bool movingLeft = getInputHandler().isKeyDown(getKeyAssignments().getProperty("MOVE_LEFT"));
		bool movingRight = getInputHandler().isKeyDown(getKeyAssignments().getProperty("MOVE_RIGHT"));

		long long timeNotMovingForward = time_notMovingForward - time_movingForward;

		if (movingForward && (timeNotMovingForward > 0 && timeNotMovingForward < NOT_MOVING_TO_SPRINT_MS)) {
			if(fireToggleSprintingEvent(true)) isSprinting = true;
		}

		if ((! (movingForward || movingBackward || movingLeft || movingRight)) && isSprinting) {
			if(fireToggleSprintingEvent(false)) isSprinting = false;
		}

		if(movingForward) {
			float computedX = cos(toRadiands(rotation.y - 90));
			float computedZ = sin(toRadiands(rotation.y - 90));

			planeSpeed.x += computedX;
			planeSpeed.y += computedZ;

			time_movingForward = getMilliseconds();

		} else time_notMovingForward = getMilliseconds();

		if(movingBackward) {
			float computedX = cos(toRadiands(rotation.y - 90));
			float computedZ = sin(toRadiands(rotation.y - 90));

			planeSpeed.x -= computedX;
			planeSpeed.y -= computedZ;
		}

		if(movingLeft) {
			float computedX = cos(toRadiands(rotation.y));
			float computedZ = sin(toRadiands(rotation.y));

			planeSpeed.x -= computedX;
			planeSpeed.y -= computedZ;
		}

		if(movingRight) {
			float computedX = cos(toRadiands(rotation.y));
			float computedZ = sin(toRadiands(rotation.y));

			planeSpeed.x += computedX;
			planeSpeed.y += computedZ;
		}

		if(glm::length(planeSpeed) != 0) {
			planeSpeed = glm::normalize(planeSpeed);

			if(isSprinting) planeSpeed *= (SPRINTING_SPEED * delta);
			else planeSpeed *= (BASIC_SPEED * delta);

			if (level.getBlockAt(vcround(position.x + planeSpeed.x), vcroundY(position.y), vcround(position.z)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x), vcroundY(position.y
							+ 1), vcround(position.z)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x), vcroundY(position.y + 2
							- 0.2f), vcround(position.z)) == nullptr
					&&

					level.getBlockAt(vcround(position.x + planeSpeed.x
							+ COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x + COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x + COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z)) == nullptr
					&&

					level.getBlockAt(vcround(position.x + planeSpeed.x
							- COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x - COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x - COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z)) == nullptr
					&&

					level.getBlockAt(vcround(position.x + planeSpeed.x), vcroundY(position.y), vcround(position.z
							+ COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x), vcroundY(position.y + 1), vcround(position.z
							+ COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x), vcroundY(position.y + 2
							- 0.2f), vcround(position.z + COLLISION_TOLERANCE)) == nullptr
					&&

					level.getBlockAt(vcround(position.x + planeSpeed.x), vcroundY(position.y), vcround(position.z
							- COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x), vcroundY(position.y + 1), vcround(position.z
							- COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x), vcroundY(position.y + 2
							- 0.2f), vcround(position.z - COLLISION_TOLERANCE)) == nullptr
					&&

					// diagonal
					level.getBlockAt(vcround(position.x + planeSpeed.x
							- COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z + COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x - COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z + COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x - COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z + COLLISION_TOLERANCE)) == nullptr
					&&

					level.getBlockAt(vcround(position.x + planeSpeed.x
							+ COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z + COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x + COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z + COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x + COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z + COLLISION_TOLERANCE)) == nullptr
					&&

					level.getBlockAt(vcround(position.x + planeSpeed.x
							- COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z - COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x - COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z - COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x - COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z - COLLISION_TOLERANCE)) == nullptr
					&&

					level.getBlockAt(vcround(position.x + planeSpeed.x
							+ COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z - COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x + COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z - COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + planeSpeed.x + COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z - COLLISION_TOLERANCE)) == nullptr) {
				position.x += planeSpeed.x;
			}

			if (level.getBlockAt(vcround(position.x), vcroundY(position.y), vcround(position.z + planeSpeed.y)) == nullptr
					&& level.getBlockAt(vcround(position.x), vcroundY(position.y + 1), vcround(position.z
							+ planeSpeed.y)) == nullptr
					&& level.getBlockAt(vcround(position.x), vcroundY(position.y + 2 - 0.2f), vcround(position.z
							+ planeSpeed.y)) == nullptr
					&&

					level.getBlockAt(vcround(position.x), vcroundY(position.y), vcround(position.z + planeSpeed.y
							+ COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x), vcroundY(position.y + 1), vcround(position.z + planeSpeed.y
							+ COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x), vcroundY(position.y + 2 - 0.2f), vcround(position.z
							+ planeSpeed.y + COLLISION_TOLERANCE)) == nullptr
					&&

					level.getBlockAt(vcround(position.x), vcroundY(position.y), vcround(position.z + planeSpeed.y
							- COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x), vcroundY(position.y + 1), vcround(position.z + planeSpeed.y
							- COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x), vcroundY(position.y + 2 - 0.2f), vcround(position.z
							+ planeSpeed.y - COLLISION_TOLERANCE)) == nullptr
					&&

					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z
							+ planeSpeed.y)) == nullptr
					&& level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z + planeSpeed.y)) == nullptr
					&& level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z + planeSpeed.y)) == nullptr
					&&

					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z
							+ planeSpeed.y)) == nullptr
					&& level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z + planeSpeed.y)) == nullptr
					&& level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z + planeSpeed.y)) == nullptr
					&&

					// diagonal
					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z
							+ planeSpeed.y - COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z + planeSpeed.y - COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z + planeSpeed.y - COLLISION_TOLERANCE)) == nullptr
					&&

					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z
							+ planeSpeed.y - COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z + planeSpeed.y - COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z + planeSpeed.y - COLLISION_TOLERANCE)) == nullptr
					&&

					level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z
							+ planeSpeed.y + COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z + planeSpeed.y + COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x + COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z + planeSpeed.y + COLLISION_TOLERANCE)) == nullptr
					&&

					level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y), vcround(position.z
							+ planeSpeed.y + COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y
							+ 1), vcround(position.z + planeSpeed.y + COLLISION_TOLERANCE)) == nullptr
					&& level.getBlockAt(vcround(position.x - COLLISION_TOLERANCE), vcroundY(position.y + 2
							- 0.2f), vcround(position.z + planeSpeed.y + COLLISION_TOLERANCE)) == nullptr) {
				position.z += planeSpeed.y;
			}
		}
	}

	void Player::computeRotation() {
		float dx = getCursorHandler().getDeltaX();
		float dy = getCursorHandler().getDeltaY();

		rotation.y += dx * ROTATION_SENSIBILITY;
		rotation.x += dy * ROTATION_SENSIBILITY;

		if(rotation.x > 90) rotation.x = 90;
		if(rotation.x < -90) rotation.x = -90;
	}

	bool Player::fireToggleSprintingEvent(bool newState) {
		ToggleSprintingEvent event(this, newState);
		level.getToggleSprintingEventManager().fireEvent(event);

		return !event.isCancelled();
	}


	PlayerPosition Player::getPosition() const {
		return position;
	}

	void Player::setPosition(float x, float y, float z) {
		set(position, x, y, z);
	}

	glm::ivec3 Player::getChunkCoordinates() const {
		return convertWorldToChunkCoordinates(position);
	}

	bool Player::doesPlayerCollidateWithBlock(int worldX, int worldY, int worldZ) const {
		if(worldX == position.getIntX() && worldZ == position.getIntZ() && (worldY == position.getIntY() || worldY == position.getIntY() + 1 || worldY == position.getIntY() + 2)) {
			return true;
		}

		if(position.x < worldX + 1.0 + COLLISION_TOLERANCE && position.x > worldX - COLLISION_TOLERANCE &&
			position.z < worldZ + 1.0 + COLLISION_TOLERANCE && position.z > worldZ - COLLISION_TOLERANCE &&
			(worldY == position.getIntY() || worldY == position.getIntY() + 1 || worldY == position.getIntY() + 2)) {
			return true;
		}

		return false;
	}

	bool Player::doesPlayerCollidateWithBlock(const glm::ivec3& position) const {
		return doesPlayerCollidateWithBlock(position.x, position.y, position.z);
	}

	glm::vec3 Player::getRotation() const {
		return rotation;
	}

	void Player::setRotation(glm::vec3 rotation) {
		this->rotation = rotation;
	}

	glm::mat4x4 Player::computeViewMatrix() const {
		return viewMatrix;
	}

	std::shared_ptr<GameItem> Player::getItemTypeInHand() const {
		return inventory.get(activeHotbarIndex).getGameItem();
	}

	PlayerInventory& Player::getInventory() {
		return inventory;
	}

	Slot& Player::getItemClipboard() {
		return itemClipboard;
	}

	int Player::getActiveHotbarIndex() const {
		return activeHotbarIndex;
	}

	bool Player::isInventoryGUIActive() const {
		// we can't use getInventoryGUI()
		return (displayedInventoryGui) ? true : false;
	}

	std::shared_ptr<InventoryGUI> Player::getInventoryGUI() {
		return displayedInventoryGui;
	}

	void Player::setInventoryGUI(const std::shared_ptr<InventoryGUI>& invGui) {
		this->displayedInventoryGui = invGui;

		if(invGui) {
			egui::getInputHandler().setCursorInputMode(CURSOR_NORMAL);
		} else {
			egui::getInputHandler().setCursorInputMode(CURSOR_DISABLED);
		}
	}

	void Player::setIngameStateActive(bool flag) {
		isIngameStateActive_field = flag;
	}

	bool Player::isIngameStateActive() const {
		return isIngameStateActive_field;
	}

	Frustum& Player::getFrustum() {
		return frustum;
	}

	MousePicker& Player::getMousePicker() {
		return picker;
	}

	EventManager<ItemInHandChangedEvent>& Player::getItemInHandChangeManager() {
		return itemInHandChangeEventManager;
	}

}
