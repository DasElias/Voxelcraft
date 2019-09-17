#pragma once

#include <input/KeyAssignments.h>
#include <SimpleIni.h>

namespace vc {
	class KeyAssignmentsImpl : public egui::KeyAssignments {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			CSimpleIniA properties;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			KeyAssignmentsImpl();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::string getFile();
			void save();

		public:
			int getProperty(std::string key) override;
			bool setProperty(std::string key, int value, bool preventReplace);

	};

}
