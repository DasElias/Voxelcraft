#pragma once


#include <optional>
#include <string>

namespace vc {
	std::optional<std::string> openLevelExportDialog();
	std::optional<std::string> openLevelImportDialog();
}