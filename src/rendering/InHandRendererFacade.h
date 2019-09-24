#pragma once
#include "BlockInHandRenderer.h"
#include "LevelRendererInterface.h"

namespace vc {
	class InHandRendererFacade : public LevelRendererInterface {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			BlockInHandRenderer blockRenderer;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			InHandRendererFacade(TextureArray& texArray);
			InHandRendererFacade(const InHandRendererFacade&) = delete;
			InHandRendererFacade& operator=(const InHandRendererFacade&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:	
			void render(std::shared_ptr<GameItem> item);
			void setCurrentLevel(Level* level) override;
	};
}