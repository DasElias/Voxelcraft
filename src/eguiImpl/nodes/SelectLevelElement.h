#pragma once

#include <array>
#include <model\nodes\Button.h>
#include <model\nodes\HBox.h>
#include <model\nodes\Label.h>
#include <model\nodes\UnorganizedParentElement.h>
#include "..//..//model/LevelMetadata.h"
#include "CreateLevelOverlayElement.h"

namespace egui {
	typedef std::function<void(std::shared_ptr<vc::LevelMetadata>)> selectLevelFunct;
	typedef std::function<void(std::shared_ptr<vc::LevelMetadata>)> deleteLevelFunct;
	typedef std::function<void(std::shared_ptr<vc::LevelMetadata>)> exportFunct;
	typedef std::function<void(void)> importFuct;

	class SelectLevelElement : public UnorganizedParentElement {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const NUMBER_OF_BTNS = 7;
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::vector<std::shared_ptr<vc::LevelMetadata>>& levelMetadatas;

			std::array<std::shared_ptr<HBox>, NUMBER_OF_BTNS> buttonBoxes;
			std::array<std::shared_ptr<Button>, NUMBER_OF_BTNS> selectButtons;
			std::array<std::shared_ptr<Button>, NUMBER_OF_BTNS> deleteButtons;
			std::array<std::shared_ptr<Button>, NUMBER_OF_BTNS> exportButtons;


			selectLevelFunct selectFunction;
			deleteLevelFunct deleteFunction;
			exportFunct exportFunction;
			importFuct importFunction;

			std::shared_ptr<Button> backBtn;
			std::shared_ptr<Button> createNewGameScoreBtn;
			std::shared_ptr<Button> importBtn;
			std::shared_ptr<Label> noLevelFoundWarning;

			std::shared_ptr<CreateLevelOverlayElement> createLevelOverlayElement;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			SelectLevelElement(std::vector<std::shared_ptr<vc::LevelMetadata>>& levelMetadatas, selectLevelFunct selectFunction, deleteLevelFunct deleteFunction, createLevelFunct createFunction, exportFunct exportFunction, importFuct importFunction);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void rebuildElem();

		public:
			void setOnBackBtn(FunctionWrapper<ActionEvent> eventHandler);
	};
}