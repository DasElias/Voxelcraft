#pragma once

#include <model/events/FunctionWrapper.h>
#include <functional>
#include "LevelRendererInterface.h"
#include "../model/events/block/BlockPlaceEvent.h"
#include "../model/events/player/ItemInHandChangedEvent.h"
#include "../model/ProjectionMatrix.h"



namespace vc {
	class InHandRenderer : public LevelRendererInterface {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		static float const MODEL_SCALE;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ProjectionMatrix blockInHandProjectionMatrix;
			Level* p_currentLevel = nullptr;
			egui::FunctionWrapper<BlockPlaceEvent> blockPlaceEventHandler = {[this](BlockPlaceEvent&) {
					onBlockPlaced();
				}};
			egui::FunctionWrapper<BlockBreakEvent> blockBreakEventHandler = {[this](BlockBreakEvent&) {
					onBlockPlaced();
				}};
			egui::FunctionWrapper<ItemInHandChangedEvent> itemInHandChangeEventHandler = {[this](ItemInHandChangedEvent&) {
					onItemChanged();
				}};


			long long blockPlacedStartTime;
			long long itemInHandChangedTime;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			InHandRenderer();
			InHandRenderer(const InHandRenderer&) = delete;
			~InHandRenderer();
		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void onBlockPlaced();
			void onItemChanged();

		protected:
			Level* getCurrentLevel();

		public:
			void setCurrentLevel(Level* p_currentLevel) override;
			float getBlockPlace_percent();
			float getItemChange_percent();

			glm::mat4x4 getModelProjectionMatrix();

	};
}
