#pragma once
#include "../model/Level.h"

namespace vc {
	class LevelRendererInterface {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			LevelRendererInterface() = default;
			virtual ~LevelRendererInterface() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void setCurrentLevel(Level* level) =0;
	};
}
