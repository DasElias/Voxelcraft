#pragma once

namespace vc {
	class Cleanable {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			bool isAlreadyCleaned = false;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			virtual ~Cleanable() = default;

		public:
			virtual void cleanUp();
	};
}
