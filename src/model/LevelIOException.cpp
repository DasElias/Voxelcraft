#include "LevelIOException.h"

namespace vc {
	LevelIOException::LevelIOException(std::string what) :
			std::runtime_error(what) {
	}
}
