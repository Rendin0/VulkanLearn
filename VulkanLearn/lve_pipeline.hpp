#pragma once
#include <string>
#include <vector>
#include "lve_device.hpp"

namespace lve
{
	struct PipelineConfigInfo
	{
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class LvePipeline
	{
	public:
		LvePipeline(LveDevice& device, const std::string& vert_file_path, const std::string& frag_file_path, const PipelineConfigInfo& config);

		~LvePipeline();

		LvePipeline(const LvePipeline&) = delete;
		LvePipeline& operator=(const LvePipeline&) = delete;

		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

		void bind(VkCommandBuffer command_buffer);

	private:
		static std::vector<char> readFile(const std::string& file_path);

		void createGraphicsPipeline(const std::string& vert_file_path, const std::string& frag_file_path, const PipelineConfigInfo& config);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shader_module);

		LveDevice& lve_device;
		VkPipeline graphics_pipeline;
		VkShaderModule vert_shader_module;
		VkShaderModule frag_shader_module;
	};
}