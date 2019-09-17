#pragma once

#include <stdexcept>

namespace vc {
	class LevelIOException : public std::runtime_error {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LevelIOException(std::string what);
	};
}
