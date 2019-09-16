#define _CRT_SECURE_NO_WARNINGS
#include "FileUtils.h"
#include <sys/stat.h>

#include <stdexcept>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

using namespace boost;

namespace vc::model {
	bool doesExist(std::string path) {
		return filesystem::exists(path);
	}

	std::string getApplicationFolder() {
		std::string applFolder = std::string(getenv("APPDATA"));
		applFolder = applFolder.append("\\Voxelcraft");

		if(! doesExist(applFolder)) createDirectory(applFolder);

		return applFolder;
	}

	bool isDirectory(std::string path) {
		return filesystem::is_directory(path);
	}

	void createDirectory(std::string path) {
		filesystem::create_directory(path);
	}

	void createFile(std::string path) {
		std::ofstream file {path};
	}

	void deleteDirectory(std::string path) {
		filesystem::remove_all(path);
	}


}

