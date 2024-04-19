#include "lve_pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace lve
{
	LvePipeline::LvePipeline(LveDevice& device, const std::string& vert_file_path, const std::string& frag_file_path, const PipelineConfigInfo& config)
		: lve_device(device)
	{
		createGraphicsPipeline(vert_file_path, frag_file_path, config);
	}

	LvePipeline::~LvePipeline()
	{
		vkDestroyShaderModule(lve_device.device(), vert_shader_module, nullptr);
		vkDestroyShaderModule(lve_device.device(), frag_shader_module, nullptr);
		vkDestroyPipeline(lve_device.device(), graphics_pipeline, nullptr);
	}

	PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
	{
		PipelineConfigInfo info{};

		info.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		info.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		info.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
		info.inputAssemblyInfo.pNext = nullptr;

		info.viewport.x = 0.0f;
		info.viewport.y = 0.0f;
		info.viewport.width = static_cast<float>(width);
		info.viewport.height = static_cast<float>(height);
		info.viewport.minDepth = 0.0f;
		info.viewport.maxDepth = 1.0f;

		info.scissor.offset = { 0, 0 };
		info.scissor.extent = { width, height };

		

		info.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		info.rasterizationInfo.depthClampEnable = VK_FALSE;
		info.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		info.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		info.rasterizationInfo.lineWidth = 1.0f;
		//info.rasterizationInfo.flags = 0;
		info.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		info.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		info.rasterizationInfo.depthBiasEnable = VK_FALSE;
		info.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		info.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		info.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		info.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		info.multisampleInfo.sampleShadingEnable = VK_FALSE;
		info.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		info.multisampleInfo.minSampleShading = 1.0f;           // Optional
		info.multisampleInfo.pSampleMask = nullptr;             // Optional
		info.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		info.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		info.colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		info.colorBlendAttachment.blendEnable = VK_FALSE;
		info.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		info.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		info.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		info.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		info.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		info.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		info.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		info.depthStencilInfo.depthTestEnable = VK_TRUE;
		info.depthStencilInfo.depthWriteEnable = VK_TRUE;
		info.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		info.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		info.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
		info.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		info.depthStencilInfo.stencilTestEnable = VK_FALSE;
		info.depthStencilInfo.front = {};  // Optional
		info.depthStencilInfo.back = {};   // Optional

		return info;
	}

	void LvePipeline::bind(VkCommandBuffer command_buffer)
	{
		vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);
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

	void LvePipeline::createGraphicsPipeline(const std::string& vert_file_path, const std::string& frag_file_path, const PipelineConfigInfo& config)
	{
		assert(config.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no pipelineLayout provided in configuration");
		assert(config.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no pipelineRenderPass provided in configuration");

		auto vert_code = readFile(vert_file_path);
		auto frag_code = readFile(frag_file_path);

		createShaderModule(vert_code, &vert_shader_module);
		createShaderModule(frag_code, &frag_shader_module);

		VkPipelineShaderStageCreateInfo shader_stages[2];

		shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = vert_shader_module;
		shader_stages[0].pName = "main";
		shader_stages[0].flags = 0;
		shader_stages[0].pNext = nullptr;
		shader_stages[0].pSpecializationInfo = nullptr;
		
		shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = frag_shader_module;
		shader_stages[1].pName = "main";
		shader_stages[1].flags = 0;
		shader_stages[1].pNext = nullptr;
		shader_stages[1].pSpecializationInfo = nullptr;


		auto binding_descriptions = LveModel::Vertex::getBindingDescriptions();
		auto attribute_descriptions = LveModel::Vertex::getAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertex_input_info{};
		vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
		vertex_input_info.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
		vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();
		vertex_input_info.pVertexBindingDescriptions = binding_descriptions.data();

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = &config.viewport;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = &config.scissor;
		viewportInfo.pNext = nullptr;

		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.logicOpEnable = VK_FALSE;
		colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		colorBlendInfo.attachmentCount = 1;
		colorBlendInfo.pAttachments = &config.colorBlendAttachment;
		colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		colorBlendInfo.blendConstants[3] = 0.0f;  // Optional


		VkGraphicsPipelineCreateInfo pipeline_info{};
		pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_info.stageCount = 2;
		pipeline_info.pStages = shader_stages;
		pipeline_info.pVertexInputState = &vertex_input_info;
		pipeline_info.pInputAssemblyState = &config.inputAssemblyInfo;
		pipeline_info.pColorBlendState = &colorBlendInfo;
		pipeline_info.pDepthStencilState = &config.depthStencilInfo;
		pipeline_info.pMultisampleState = &config.multisampleInfo;
		pipeline_info.pRasterizationState = &config.rasterizationInfo;
		pipeline_info.pViewportState = &viewportInfo;
		pipeline_info.pDynamicState = nullptr;

		pipeline_info.layout = config.pipelineLayout;
		pipeline_info.renderPass = config.renderPass;
		pipeline_info.subpass = config.subpass;

		pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
		pipeline_info.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(lve_device.device(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &graphics_pipeline) != VK_SUCCESS)
			throw std::runtime_error("Failed to create graphics pipeline");
	}

	void LvePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shader_module)
	{
		VkShaderModuleCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = code.size();
		info.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(lve_device.device(), &info, nullptr, shader_module) != VK_SUCCESS)
			throw std::runtime_error("Failed to create shader module");
	}
}