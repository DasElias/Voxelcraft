#pragma once
#include <string>
#include <vector>

#include "Texture.h"

namespace vc {
	namespace renderingModel {
		class TextureArray : public Texture {
			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				uint32_t texId;
				int32_t width;
				int32_t height;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				TextureArray(std::vector<std::string>& textures);

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				uint32_t getTexId() const override;
				int32_t getWidth() const;
				int32_t getHeight() const;

				void cleanUp() override;
		};
	}
}
