#pragma once

#include <tl/optional.hpp>
#include <string>

#include "../../renderModel/SimpleTexture.h"

namespace vc {
	namespace model {
		class ParticleTexture : public Cleanable {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				vc::renderingModel::SimpleTexture backboneTexture;
				int stages;
				bool field_useAdditiveBlending;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				ParticleTexture(std::string path, int stages, bool isAdditive = false);

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				int getTextureId() const;
				int getStages() const;
				int getWidth() const;
				int getHeight() const;
				bool useAdditiveBlending() const;

				void cleanUp() override;
		};
	}
}
