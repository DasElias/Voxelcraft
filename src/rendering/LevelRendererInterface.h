#pragma once
#include "../model/Level.h"

namespace vc {
	namespace rendering {
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
				virtual void setCurrentLevel(vc::model::Level* level) =0;
		};
	}
}
