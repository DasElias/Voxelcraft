
#include "State.h"
#include <model/scene/Scene.h>
#include "../../eguiImpl/nodes/SelectLevelElement.h"
#include "../LevelBuilder.h"


namespace vc {
	class SelectLevelState : public State {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::string const WORLD_SAVE_PATH;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			AbstractChunkVaoManager& vaoCreator;
			ctpl::thread_pool& threadPool;

			egui::MasterRenderer& renderer;
			std::shared_ptr<egui::SelectLevelElement> elem;
			egui::Scene scene;

			std::vector<std::shared_ptr<LevelMetadata>> levelMetadataArr;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			SelectLevelState(StateManager& stateManager, egui::MasterRenderer& renderer, AbstractChunkVaoManager& vaoCreator, ctpl::thread_pool& threadPool);
			~SelectLevelState();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			egui::selectLevelFunct getLambda_selectFunc();
			egui::deleteLevelFunct getLambda_deleteFunc();
			egui::createLevelFunct getLambda_createFunc();
			egui::exportFunct getLambda_exportFunc();
			egui::importFuct getLambda_importFunc();

			void updateLevelMetadataArr();
			std::string generateLevelFolderName(std::string levelFolderName) const;

		public:
			void updateAndRender(float delta) override;
			void onStateEnter() override;
	};
}