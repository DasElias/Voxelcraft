#pragma once

#include <model/nodes/HBox.h>
#include <model/nodes/Button.h>

#include <model/nodes/Slider.h>

#include <model/nodes/Label.h>


namespace egui {
	class OptionsStateElement : public HBox {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<Slider> fovSlider;
			std::shared_ptr<Button> renderDistanceSlider;
			std::shared_ptr<Button> maxFramerateSlider;
			std::shared_ptr<Button> useVSyncBtn;

			std::shared_ptr<Button> controls;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			OptionsStateElement(EGuiContext& ctx);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void setOnFovSliderMove(FunctionWrapper<ActionEvent> eventHandler);
			void setOnRenderDistanceSliderMove(FunctionWrapper<ActionEvent> eventHandler);
			void setOnMaxFramerateSliderMove(FunctionWrapper<ActionEvent> eventHandler);
	};
}