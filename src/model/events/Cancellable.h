#pragma once

namespace vc {
	namespace model {
		class Cancellable {

			// ----------------------------------------------------------------------
			// --------------------------------FIELDS--------------------------------
			// ----------------------------------------------------------------------
			private:
				bool field_isCancelled = false;


			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			protected:
				Cancellable() = default;

			public:
				Cancellable(const Cancellable&) = delete;


			// ----------------------------------------------------------------------
			// -------------------------------METHODS--------------------------------
			// ----------------------------------------------------------------------
			public:
				void cancel();
				bool isCancelled() const;
		};
	}
}

