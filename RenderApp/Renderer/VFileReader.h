#pragma once
namespace VEE {
	class VFileReader {
	public:
		static std::vector<char> readFile(const std::string& filename);
	};
}