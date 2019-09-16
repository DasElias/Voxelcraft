#pragma once

#include <model/events/FunctionWrapper.h>
#include <functional>
#include "LevelRendererInterface.h"
#include "../model/events/block/BlockPlaceEvent.h"
#include "../model/events/player/ItemInHandChangedEvent.h"
#include "../model/ProjectionMatrix.h"



namespace vc {
	namespace rendering {
		class InHandRenderer : public LevelRendererInterface {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			static float const MODEL_SCALE;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				vc::model::ProjectionMatrix blockInHandProjectionMatrix;
				vc::model::Level* p_currentLevel = nullptr;
				egui::FunctionWrapper<vc::model::BlockPlaceEvent> blockPlaceEventHandler = {[this](vc::model::BlockPlaceEvent&) {
						onBlockPlaced();
					}};
				egui::FunctionWrapper<vc::model::BlockBreakEvent> blockBreakEventHandler = {[this](vc::model::BlockBreakEvent&) {
						onBlockPlaced();
					}};
				egui::FunctionWrapper<vc::model::ItemInHandChangedEvent> itemInHandChangeEventHandler = {[this](vc::model::ItemInHandChangedEvent&) {
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
				vc::model::Level* getCurrentLevel();

			public:
				void setCurrentLevel(vc::model::Level* p_currentLevel) override;
				float getBlockPlace_percent();
				float getItemChange_percent();

				glm::mat4x4 getModelProjectionMatrix();

		};
	}
}
