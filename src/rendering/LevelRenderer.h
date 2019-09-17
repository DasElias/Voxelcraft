#pragma once
#include "LevelRendererInterface.h"
#include "BlockRenderer.h"
#include "BlockInHandRenderer.h"
#include <cstdint>
#include <memory>
#include <climits>

#include "../model/Level.h"
#include "../renderModel/FrameBufferObject.h"
#include "../renderModel/TextureArray.h"
#include "GaussianBlurPostProcessor.h"

namespace vc {
	class LevelRenderer : public LevelRendererInterface {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const MULTISAMPLES = 2;
			static int const RESIZING_FBO_DELAY_MS = 250;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			TextureArray blockTextureArray;
			BlockRenderer blockRenderer;
			//ParticleRenderer particleRenderer;
			BlockInHandRenderer blockInHandRenderer;

			Level* p_currentLevel = nullptr;

			std::unique_ptr<FrameBufferObject> multisampleFbo;
			std::unique_ptr<FrameBufferObject> outputFbo;
			GaussianBlurPostProcessor postProcessor;

			int lastFrameWindowWidth;
			int lastFrameWindowHeight;
			long long lastTimeResizing;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LevelRenderer();
			LevelRenderer(const LevelRenderer&) = delete;
			virtual ~LevelRenderer();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			bool updateFbos();
			void render_impl(float delta, bool updateOutputFbo);


		public:
			void setCurrentLevel(Level* p_currentLevel) override;
			void render(float delta);
			void renderWithGaussianBlur(float delta);
	};
}
