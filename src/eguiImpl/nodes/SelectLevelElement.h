#pragma once

#include <array>
#include <model\nodes\Button.h>
#include <model\nodes\HBox.h>
#include <model\nodes\UnorganizedParentElement.h>
#include "..//..//model/LevelMetadata.h"

namespace egui {
	typedef std::function<void(std::shared_ptr<vc::LevelMetadata>)> selectLevelFunct;
	typedef std::function<void(std::shared_ptr<vc::LevelMetadata>)> deleteLevelFunct;
	typedef std::function<void(void)> createLevelFunct;

	class SelectLevelElement : public UnorganizedParentElement {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const NUMBER_OF_BTNS = 9;
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::vector<std::shared_ptr<vc::LevelMetadata>>& levelMetadatas;

			std::array<std::shared_ptr<HBox>, NUMBER_OF_BTNS> buttonBoxes;
			std::array<std::shared_ptr<Button>, NUMBER_OF_BTNS> selectButtons;
			std::array<std::shared_ptr<Button>, NUMBER_OF_BTNS> deleteButtons;

			selectLevelFunct selectFunction;
			deleteLevelFunct deleteFunction;
			createLevelFunct createFunction;

			std::shared_ptr<Button> backBtn;
			std::shared_ptr<Button> createNewGameScoreBtn;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			SelectLevelElement(std::vector<std::shared_ptr<vc::LevelMetadata>>& levelMetadatas, selectLevelFunct selectFunction, deleteLevelFunct deleteFunction, createLevelFunct createFunction);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void rebuildElem();

		public:
			void setOnBackBtn(FunctionWrapper<ActionEvent> eventHandler);
	};
}