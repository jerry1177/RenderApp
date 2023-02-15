#include "Renderpch.h"
#include "VFileReader.h"
#include <fstream>

namespace VEE {
	std::vector<char> VFileReader::readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		ASSERT(file.is_open(), "Could not open file!");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}
}
