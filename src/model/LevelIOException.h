#pragma once

#include <stdexcept>

namespace vc {
	namespace model {
		class LevelIOException : public std::runtime_error {
			// ----------------------------------------------------------------------
			// -----------------------------CONSTRUCTORS-----------------------------
			// ----------------------------------------------------------------------
			public:
				LevelIOException(std::string what);
		};
	}
}
