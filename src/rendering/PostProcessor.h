#pragma once
#include <array>
#include <cstdint>
#include "../renderModel/GeneralVertexArrayObject.h"

namespace vc {
	namespace rendering {
		class PostProcessor {
			// ----------------------------------------------------------------------
			// ----------------------------STATIC-FIELDS-----------------------------
			// ----------------------------------------------------------------------
			private:
				static std::vector<float> const POSITIONS;

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				vc::renderingModel::GeneralVertexArrayObject quad;

			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			protected:
				PostProcessor();

			public:
				PostProcessor(const PostProcessor&) = delete;
				virtual ~PostProcessor() = default;

			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				virtual void render(uint32_t textureId, bool updateFbos);
		};
	}
}
