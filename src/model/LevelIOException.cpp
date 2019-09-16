#include "LevelIOException.h"

namespace vc::model {
	vc::model::LevelIOException::LevelIOException(std::string what) :
			std::runtime_error(what) {
	}
}
