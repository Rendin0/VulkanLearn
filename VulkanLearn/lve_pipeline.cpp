#include "lve_pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve
{
	LvePipeline::LvePipeline(const std::string& vert_file_path, const std::string& frag_file_path)
	{
		createGraphicsPipeline(vert_file_path, frag_file_path);
	}

	std::vector<char> LvePipeline::readFile(const std::string& file_path)
	{
		std::ifstream file_in(file_path, std::ios::ate | std::ios::binary);
		if (!file_in.is_open())
			throw std::runtime_error("Failed to open file: " + file_path);


		size_t file_size = static_cast<size_t>(file_in.tellg());
		std::vector<char> buffer(file_size);

		file_in.seekg(0);
		file_in.read(buffer.data(), file_size);
		file_in.close();
		return buffer;
	}
	void LvePipeline::createGraphicsPipeline(const std::string& vert_file_path, const std::string& frag_file_path)
	{
		auto vert_code = readFile(vert_file_path);
		auto frag_code = readFile(frag_file_path);

		std::cout << "Vert file size: " << vert_code.size() << std::endl;
		std::cout << "\nFrag file size: " << frag_code.size() << std::endl;
	}
}