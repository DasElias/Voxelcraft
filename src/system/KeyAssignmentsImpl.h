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
			bool setProperty_impl(std::string key, int value, bool preventReplace = false);

		public:
			int getProperty(std::string key) override;	
			void setProperty(std::string key, int value) override;

	};

}
