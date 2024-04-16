#pragma once
#include <string>
#include <vector>

namespace lve
{
	class LvePipeline
	{
	public:
		LvePipeline(const std::string& vert_file_path, const std::string& frag_file_path);

	private:
		static std::vector<char> readFile(const std::string& file_path);

		void createGraphicsPipeline(const std::string& vert_file_path, const std::string& frag_file_path);
	};
}