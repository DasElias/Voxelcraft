#pragma once
#include <string>


namespace vc {
	bool doesExist(std::string path);
	bool isDirectory(std::string path);
	void createDirectory(std::string path);
	void createFile(std::string path);
	std::string getApplicationFolder();

	void deleteDirectory(std::string path);
}

